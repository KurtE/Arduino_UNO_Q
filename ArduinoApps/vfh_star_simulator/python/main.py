import time
import math
import heapq
import io
import json
import copy
import tempfile
from dataclasses import dataclass
from typing import List, Dict, Any

import imageio.v2 as imageio
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap

import numpy as np

from arduino.app_bricks.streamlit_ui import st
from arduino.app_utils import *

st.set_page_config(page_title="VFH* Real-Time Visualization", layout="wide")

# =========================
# Core data structures
# =========================

@dataclass
class Pose2D:
    x: float = 0.0
    y: float = 0.0
    theta: float = 0.0  # radians


@dataclass
class OccupancyGrid:
    width: int
    height: int
    resolution: float
    origin_x: float
    origin_y: float
    data: List[int]

    def index(self, ix: int, iy: int) -> int:
        return iy * self.width + ix


@dataclass
class VFHStarNode:
    pose: Pose2D
    steering_cmd: float
    g: float
    h: float
    f: float
    depth: int
    parent_idx: int


# =========================
# VFH+ and VFH*
# =========================

class VFHPlus:
    def __init__(self, num_bins, max_range, safety_dist, robot_radius,
                 threshold_low, threshold_high):
        self.num_bins = num_bins
        self.max_range = max_range
        self.safety_dist = safety_dist
        self.robot_radius = robot_radius
        self.threshold_low = threshold_low
        self.threshold_high = threshold_high
        self.histogram = [0.0] * num_bins
        self.last_valleys = []
        self.smoothing_kernel = 2

    @staticmethod
    def normalize_angle(a: float) -> float:
        while a > math.pi:
            a -= 2.0 * math.pi
        while a < -math.pi:
            a += 2.0 * math.pi
        return a

    def angle_to_bin(self, angle: float) -> int:
        two_pi = 2.0 * math.pi
        angle = math.fmod(angle + two_pi, two_pi)
        bin_width = two_pi / self.num_bins
        return int(angle / bin_width)

    def bin_to_angle(self, bin_idx: int) -> float:
        bin_width = (2.0 * math.pi) / self.num_bins
        return bin_idx * bin_width + bin_width / 2.0

    def clear_histogram(self):
        for i in range(self.num_bins):
            self.histogram[i] = 0.0

    def accumulate_obstacles(self, pose: Pose2D, grid: OccupancyGrid):
        radius_cells = int(self.max_range / grid.resolution)

        cx = int((pose.x - grid.origin_x) / grid.resolution)
        cy = int((pose.y - grid.origin_y) / grid.resolution)

        for dy in range(-radius_cells, radius_cells + 1):
            for dx in range(-radius_cells, radius_cells + 1):
                ix = cx + dx
                iy = cy + dy

                if ix < 0 or iy < 0 or ix >= grid.width or iy >= grid.height:
                    continue

                if grid.data[grid.index(ix, iy)] < 50:
                    continue

                wx = grid.origin_x + (ix + 0.5) * grid.resolution
                wy = grid.origin_y + (iy + 0.5) * grid.resolution

                dxw = wx - pose.x
                dyw = wy - pose.y
                r = math.sqrt(dxw * dxw + dyw * dyw)
                if r < 1e-3 or r > self.max_range:
                    continue

                angle = math.atan2(dyw, dxw) - pose.theta
                angle = self.normalize_angle(angle)

                bin_idx = self.angle_to_bin(angle)
                if 0 <= bin_idx < self.num_bins:
                    self.histogram[bin_idx] += 100.0 / (r * r)

    def smooth_histogram(self):
        tmp = self.histogram[:]
        kernel = getattr(self, "smoothing_kernel", 2)

        for i in range(self.num_bins):
            s = 0.0
            count = 0
            for k in range(-kernel, kernel + 1):
                idx = (i + k + self.num_bins) % self.num_bins
                s += tmp[idx]
                count += 1
            self.histogram[i] = s / count

    def find_valleys(self):
        valleys = []
        in_valley = False
        start = 0

        for i in range(self.num_bins):
            free = self.histogram[i] < self.threshold_low
            if free and not in_valley:
                in_valley = True
                start = i
            elif not free and in_valley:
                in_valley = False
                valleys.append((start, i - 1))

        if in_valley:
            valleys.append((start, self.num_bins - 1))

        return valleys

    def compute_candidate_bins(self, pose: Pose2D, grid: OccupancyGrid, target_angle: float):
        self.clear_histogram()
        self.accumulate_obstacles(pose, grid)
        self.smooth_histogram()

        valleys = self.find_valleys()
        self.last_valleys = valleys

        if not valleys:
            return []

        target_rel = self.normalize_angle(target_angle - pose.theta)
        target_bin = self.angle_to_bin(target_rel)

        candidate_bins = []
        for start, end in valleys:
            if start <= end:
                if start <= target_bin <= end:
                    candidate_bins.append(target_bin)
                else:
                    dist_to_start = (target_bin - start + self.num_bins) % self.num_bins
                    dist_to_end = (end - target_bin + self.num_bins) % self.num_bins
                    dist_end_wrapped = (target_bin - end + self.num_bins) % self.num_bins
                    dist_start_wrapped = (start - target_bin + self.num_bins) % self.num_bins

                    if dist_to_start <= self.num_bins / 2 and dist_to_end <= self.num_bins / 2:
                        if abs(target_bin - start) < abs(target_bin - end):
                            candidate_bins.append(start)
                        else:
                            candidate_bins.append(end)
                    elif dist_end_wrapped < self.num_bins / 2 or dist_start_wrapped < self.num_bins / 2:
                        if dist_to_start < dist_end_wrapped:
                            candidate_bins.append(start)
                        else:
                            candidate_bins.append(end)
            else:
                is_target_in_wrapped_valley = (target_bin >= start) or (target_bin <= end)
                if is_target_in_wrapped_valley:
                    candidate_bins.append(target_bin)
                else:
                    dist_to_start = (target_bin - start + self.num_bins) % self.num_bins
                    dist_to_end = (end - target_bin + self.num_bins) % self.num_bins
                    if dist_to_start < dist_to_end:
                        candidate_bins.append(start)
                    else:
                        candidate_bins.append(end)

        return candidate_bins

    def bin_to_steering_angle(self, bin_idx: int, target_angle: float) -> float:
        ang = self.bin_to_angle(bin_idx)
        return self.normalize_angle(ang)


class VFHStar:
    def __init__(self,
                 vfh: VFHPlus,
                 ng: int,
                 step_dist: float,
                 lambda_: float):
        self.vfh = vfh
        self.ng = ng
        self.step_dist = step_dist
        self.lambda_ = lambda_
        self.nodes: List[VFHStarNode] = []

    def compute_steering_angle(self,
                               start_pose: Pose2D,
                               grid: OccupancyGrid,
                               target_angle: float) -> (bool, float):
        self.nodes.clear()
        open_list: List[tuple[float, int]] = []

        root = VFHStarNode(
            pose=start_pose,
            steering_cmd=0.0,
            g=0.0,
            h=0.0,
            f=0.0,
            depth=0,
            parent_idx=-1
        )
        self.nodes.append(root)
        heapq.heappush(open_list, (root.f, 0))

        best_leaf = -1

        while open_list:
            cost, idx = heapq.heappop(open_list)
            n = self.nodes[idx]

            if cost > n.f and n.depth > 0:
                continue

            if n.depth == self.ng:
                best_leaf = idx
                break

            cand_bins = self.vfh.compute_candidate_bins(n.pose, grid, target_angle)
            if not cand_bins:
                continue

            children_to_consider = []
            for bin_idx in cand_bins:
                steer = self.vfh.bin_to_steering_angle(bin_idx, target_angle)
                is_forward = (abs(steer) <= math.pi / 2.0)
                children_to_consider.append((steer, is_forward))

            forward_possible = [s for s, is_f in children_to_consider if is_f]

            if not forward_possible:
                chosen_steers = [s for s, _ in children_to_consider]
            else:
                chosen_steers = forward_possible

            for steer in chosen_steers:
                next_pose = self.simulate_step(n.pose, steer, self.step_dist)
                g_inc = self.local_cost(steer, target_angle)
                g = n.g + (self.lambda_ ** n.depth) * g_inc
                h = self.heuristic(next_pose, target_angle)
                f = g + h

                child = VFHStarNode(
                    pose=next_pose,
                    steering_cmd=steer,
                    g=g,
                    h=h,
                    f=f,
                    depth=n.depth + 1,
                    parent_idx=idx
                )
                self.nodes.append(child)
                child_idx = len(self.nodes) - 1
                heapq.heappush(open_list, (f, child_idx))

        if best_leaf < 0:
            return False, 0.0

        cur = best_leaf
        while self.nodes[cur].parent_idx != 0:
            cur = self.nodes[cur].parent_idx
            if cur == -1:
                return False, 0.0

        steering_out = self.nodes[cur].steering_cmd
        return True, steering_out

    def simulate_step(self, pose: Pose2D, steering: float, dist: float) -> Pose2D:
        new_theta = VFHPlus.normalize_angle(pose.theta + steering)
        return Pose2D(
            x=pose.x + dist * math.cos(new_theta),
            y=pose.y + dist * math.sin(new_theta),
            theta=new_theta
        )

    def local_cost(self, steer: float, target_angle: float) -> float:
        return abs(steer)

    def heuristic(self, pose: Pose2D, target_angle: float) -> float:
        diff = VFHPlus.normalize_angle(target_angle - pose.theta)
        return abs(diff)


# =========================
# World / lidar utilities
# =========================

def simulate_lidar_scan_servo(
    pose: Pose2D,
    world: OccupancyGrid,
    servo_min_deg: float,
    servo_max_deg: float,
    beam_count: int,
    max_range: float,
) -> List[float]:
    ranges = [max_range] * beam_count
    step = world.resolution * 0.5

    for i in range(beam_count):
        if beam_count > 1:
            servo_deg = servo_min_deg + i * (servo_max_deg - servo_min_deg) / (beam_count - 1)
        else:
            servo_deg = (servo_min_deg + servo_max_deg) * 0.5

        servo_rad = math.radians(servo_deg)
        beam_angle_robot = servo_rad - math.pi / 2.0
        world_angle = pose.theta + beam_angle_robot

        r = 0.0
        while r < max_range:
            wx = pose.x + r * math.cos(world_angle)
            wy = pose.y + r * math.sin(world_angle)

            ix = int((wx - world.origin_x) / world.resolution)
            iy = int((wy - world.origin_y) / world.resolution)

            if ix < 0 or iy < 0 or ix >= world.width or iy >= world.height:
                r = max_range
                break

            cell = world.data[world.index(ix, iy)]
            if cell >= 100:
                break

            r += step

        ranges[i] = r

    return ranges


def mark_free_cells(grid: OccupancyGrid, x0: int, y0: int, x1: int, y1: int):
    dx = abs(x1 - x0)
    sx = 1 if x0 < x1 else -1
    dy = -abs(y1 - y0)
    sy = 1 if y0 < y1 else -1
    err = dx + dy

    while True:
        if 0 <= x0 < grid.width and 0 <= y0 < grid.height:
            idx = grid.index(x0, y0)
            # Only clear cells that are NOT inflated (80) or walls (100)
            if grid.data[idx] < 80:
                grid.data[idx] = 0

        if x0 == x1 and y0 == y1:
            break

        e2 = 2 * err
        if e2 >= dy:
            err += dy
            x0 += sx
        if e2 <= dx:
            err += dx
            y0 += sy


def update_grid_from_lidar(
    pose: Pose2D,
    ranges: List[float],
    servo_min_deg: float,
    servo_increment_deg: float,
    max_range: float,
    grid: OccupancyGrid
):
    robot_ix = int((pose.x - grid.origin_x) / grid.resolution)
    robot_iy = int((pose.y - grid.origin_y) / grid.resolution)

    for i, r in enumerate(ranges):
        if r <= 0.01 or r > max_range:
            continue

        servo_deg = servo_min_deg + i * servo_increment_deg
        servo_rad = math.radians(servo_deg)

        beam_angle_robot = servo_rad - math.pi / 2.0
        world_angle = pose.theta + beam_angle_robot

        wx = pose.x + r * math.cos(world_angle)
        wy = pose.y + r * math.sin(world_angle)

        ix = int((wx - grid.origin_x) / grid.resolution)
        iy = int((wy - grid.origin_y) / grid.resolution)

        if ix < 0 or iy < 0 or ix >= grid.width or iy >= grid.height:
            continue

        # Clear free space along the ray (but protect inflation/walls)
        mark_free_cells(grid, robot_ix, robot_iy, ix, iy)

        idx = grid.index(ix, iy)

        # Only mark hit as 100 if it is NOT inflated or wall
        if grid.data[idx] < 80:
            grid.data[idx] = 100


def recenter_grid_around_robot(grid: OccupancyGrid, pose: Pose2D):
    robot_ix = int((pose.x - grid.origin_x) / grid.resolution)
    robot_iy = int((pose.y - grid.origin_y) / grid.resolution)

    cx = grid.width // 2
    cy = grid.height // 2

    shift_x = cx - robot_ix
    shift_y = cy - robot_iy

    if abs(shift_x) < 2 and abs(shift_y) < 2:
        return

    new_data = [0] * (grid.width * grid.height)

    for y in range(grid.height):
        for x in range(grid.width):
            old_x = x - shift_x
            old_y = y - shift_y

            if 0 <= old_x < grid.width and 0 <= old_y < grid.height:
                new_data[y * grid.width + x] = grid.data[old_y * grid.width + old_x]

    grid.data = new_data

    grid.origin_x += -shift_x * grid.resolution
    grid.origin_y += -shift_y * grid.resolution


def decay_grid(grid: OccupancyGrid):
    for i, c in enumerate(grid.data):
        if c == 100:
            continue
        if c == 80:
            continue
        if c > 0:
            grid.data[i] = max(0, c - 1)
            continue
        grid.data[i] = 0


def inflate_obstacles(grid: OccupancyGrid,
                      static_data: List[int],
                      robot_radius: float,
                      safety_dist: float):
    inflation_cells = int((robot_radius + safety_dist) / grid.resolution)
    inflated = grid.data[:]

    for y in range(grid.height):
        for x in range(grid.width):
            idx0 = grid.index(x, y)

            # Only inflate around TRUE static obstacles (walls)
            if static_data[idx0] != 100:
                continue

            for dy in range(-inflation_cells, inflation_cells + 1):
                for dx in range(-inflation_cells, inflation_cells + 1):
                    nx = x + dx
                    ny = y + dy

                    if 0 <= nx < grid.width and 0 <= ny < grid.height:
                        dist = math.sqrt(dx * dx + dy * dy) * grid.resolution

                        if dist <= robot_radius + safety_dist:
                            idx = grid.index(nx, ny)

                            # Do NOT overwrite walls
                            if inflated[idx] != 100:
                                inflated[idx] = 80

    grid.data = inflated


# =========================
# Visualization
# =========================
def plot_grid_matplotlib(ax: plt.Axes,
                         grid: OccupancyGrid,
                         pose: Pose2D,
                         ranges: List[float],
                         servo_min_deg: float,
                         servo_increment_deg: float,
                         vfh_star_nodes: List[VFHStarNode],
                         steering_angle: float = float('nan'),
                         title: str = ""):
    """
    Clean, corrected version:
    - Uses integer class labels for colormap indexing
    - Walls = black
    - Inflated obstacles = orange
    - Decay = light gray
    - Free = white
    """

    # --- Convert grid to numpy ---
    grid_data_np = np.array(grid.data).reshape(grid.height, grid.width)

    # --- Build class-labeled plot_data ---
    # 0 = free
    # 1 = decay
    # 2 = inflated
    # 3 = wall
    plot_data = np.zeros_like(grid_data_np, dtype=int)

    plot_data[(grid_data_np > 0) & (grid_data_np < 80)] = 1     # decay
    plot_data[(grid_data_np >= 80) & (grid_data_np < 100)] = 2  # inflated
    plot_data[grid_data_np >= 100] = 3                          # walls

    # --- Custom colormap ---
    from matplotlib.colors import ListedColormap
    cmap = ListedColormap([
        "white",       # 0 free
        "lightgray",   # 1 decay
        "orange",      # 2 inflated
        "black"        # 3 wall
    ])

    # --- Map extent ---
    map_extent = [
        grid.origin_x,
        grid.origin_x + grid.width * grid.resolution,
        grid.origin_y,
        grid.origin_y + grid.height * grid.resolution
    ]

    # --- Draw occupancy grid ---
    ax.imshow(
        plot_data,
        cmap=cmap,
        origin='lower',
        extent=map_extent,
        vmin=0,
        vmax=3
    )

    ax.set_title(title)
    ax.set_xlabel("X [m]")
    ax.set_ylabel("Y [m]")
    ax.set_aspect('equal', adjustable='box')

    # --- Robot pose ---
    robot_x, robot_y, robot_theta = pose.x, pose.y, pose.theta
    ax.plot(robot_x, robot_y, 'ro', markersize=8, label='Robot')
    ax.arrow(robot_x, robot_y,
             0.3 * math.cos(robot_theta), 0.3 * math.sin(robot_theta),
             width=0.02, head_width=0.1, head_length=0.1,
             fc='r', ec='r')

    # --- Lidar rays + hits ---
    lidar_xs, lidar_ys = [], []
    for i, r in enumerate(ranges):
        if r > 0.01:
            servo_deg = servo_min_deg + i * servo_increment_deg
            servo_rad = math.radians(servo_deg)
            beam_angle_robot = servo_rad - math.pi / 2.0
            world_angle = pose.theta + beam_angle_robot

            lx = pose.x + r * math.cos(world_angle)
            ly = pose.y + r * math.sin(world_angle)

            lidar_xs.append(lx)
            lidar_ys.append(ly)

            ax.plot([pose.x, lx], [pose.y, ly], 'g-', linewidth=0.3, alpha=0.3)

    if lidar_xs:
        ax.plot(lidar_xs, lidar_ys, 'g.', markersize=2, label='Lidar Hits')

    # --- VFH* search tree ---
    if vfh_star_nodes:
        for node in vfh_star_nodes:
            ax.plot(node.pose.x, node.pose.y, 'bx', markersize=3, alpha=0.5)
            if node.parent_idx != -1:
                parent = vfh_star_nodes[node.parent_idx]
                ax.plot([parent.pose.x, node.pose.x],
                        [parent.pose.y, node.pose.y],
                        'b-', linewidth=0.4, alpha=0.3)

    # --- Planned steering arrow ---
    if not math.isnan(steering_angle):
        dir_angle = VFHPlus.normalize_angle(pose.theta + steering_angle)
        ax.arrow(robot_x, robot_y,
                 0.5 * math.cos(dir_angle), 0.5 * math.sin(dir_angle),
                 width=0.02, head_width=0.1, head_length=0.1,
                 fc='b', ec='b', linestyle='--', label='Planned Steering')

    ax.grid(True)
    ax.legend(loc='upper right')


def plot_valley_overlay_into_axes(ax, vfh: VFHPlus, target_bin: int, chosen_bin: int, show_threshold_lines: bool):
    num_bins = vfh.num_bins
    bin_width = (2 * math.pi) / num_bins

    heights = np.array(vfh.histogram)
    angles = np.arange(num_bins) * bin_width

    # Histogram bars
    ax.bar(angles, heights, width=bin_width, color='gray', alpha=0.4)

    # Valley spans + labels
    for start, end in vfh.last_valleys:

        # Handle wrap-around
        if start <= end:
            valley_bins = np.arange(start, end + 1)
        else:
            valley_bins = np.concatenate([
                np.arange(start, num_bins),
                np.arange(0, end + 1)
            ])

        valley_angles = valley_bins * bin_width

        # Draw valley arc
        ax.bar(
            valley_angles,
            [max(heights) * 1.05] * len(valley_angles),
            width=bin_width,
            color='green',
            alpha=0.3
        )

        # Width label
        width_bins = len(valley_bins)
        width_deg = width_bins * (360.0 / num_bins)
        mid_bin = valley_bins[len(valley_bins) // 2]
        mid_angle = mid_bin * bin_width

        ax.text(
            mid_angle,
            max(heights) * 1.25,
            f"{width_bins} bins\n{width_deg:.1f}°",
            ha='center',
            va='center',
            fontsize=8,
            color='green',
            bbox=dict(facecolor='white', alpha=0.6, edgecolor='none')
        )

    # Threshold lines (optional)
    if show_threshold_lines:
        ax.plot([0, 2*math.pi], [vfh.threshold_low, vfh.threshold_low],
                color='orange', linestyle='--', linewidth=1, label="Low Threshold")

        ax.plot([0, 2*math.pi], [vfh.threshold_high, vfh.threshold_high],
                color='red', linestyle='--', linewidth=1, label="High Threshold")

    # Target bin
    if target_bin is not None:
        ax.bar(
            [target_bin * bin_width],
            [max(heights) * 1.2],
            width=bin_width,
            color='blue',
            alpha=0.8
        )

    # Chosen bin
    if chosen_bin is not None:
        ax.bar(
            [chosen_bin * bin_width],
            [max(heights) * 1.2],
            width=bin_width,
            color='red',
            alpha=0.8
        )

    ax.set_title("VFH Histogram + Valleys")


def render_debug_panel(ax, valleys, valley_widths, chosen_bin, steer, steering_cost, mode, vfh_star):
    ax.clear()
    ax.set_title("Real-Time Debug Info")
    ax.axis("off")

    lines = [
        f"Valleys: {valleys}",
        f"Valley widths: {valley_widths}",
        f"Chosen bin: {chosen_bin}",
        f"Steering angle (rad): {steer:.3f}",
        f"Steering cost: {steering_cost:.3f}",
    ]

    # Safe handling for playback mode (vfh_star=None)
    if mode == "VFH*" and vfh_star is not None:
        lines.append(f"Nodes explored: {len(vfh_star.nodes)}")
    else:
        lines.append("Nodes explored: N/A (playback mode)")

    text = "\n".join(lines)
    ax.text(
        0.01, 0.99, text,
        va="top", ha="left",
        fontsize=10, family="monospace"
    )



def render_frame(
    fig_placeholder,
    grid: OccupancyGrid,
    pose: Pose2D,
    ranges: List[float],
    steer: float,
    nodes_to_plot: List[VFHStarNode],
    valleys,
    valley_widths,
    chosen_bin,
    steering_cost,
    mode: str,
    step: int,
    show_debug: bool,
    show_threshold_lines: bool,
    vfh: VFHPlus = None,
    vfh_star: VFHStar = None
):
    """
    Renders a single simulation frame (live or playback).
    Fully safe even when vfh=None or vfh_star=None (playback mode).
    """

    # --- Figure Layout ---
    if show_debug:
        fig = plt.figure(figsize=(12, 10))
        ax_grid  = fig.add_subplot(2, 2, 1)
        ax_hist  = fig.add_subplot(2, 2, 2, projection='polar')
        ax_debug = fig.add_subplot(2, 1, 2)
    else:
        fig = plt.figure(figsize=(12, 6))
        ax_grid  = fig.add_subplot(1, 2, 1)
        ax_hist  = fig.add_subplot(1, 2, 2, projection='polar')
        ax_debug = None

    # --- Lidar servo geometry ---
    servo_min = 0.0
    servo_max = 180.0
    beam_count = len(ranges)
    servo_increment = (
        (servo_max - servo_min) / (beam_count - 1)
        if beam_count > 1 else 1.0
    )

    # --- Occupancy Grid + Robot + Lidar + VFH* Tree ---
    title = f"{mode} Step {step} | steer={steer:.3f} | cost={steering_cost:.3f}"

    plot_grid_matplotlib(
        ax=ax_grid,
        grid=grid,
        pose=pose,
        ranges=ranges,
        servo_min_deg=servo_min,
        servo_increment_deg=servo_increment,
        vfh_star_nodes=nodes_to_plot,
        steering_angle=steer,
        title=title
    )

    # --- Histogram + Valleys ---
    # Playback mode passes vfh=None → must guard
    if vfh is not None:
        try:
            target_bin = vfh.angle_to_bin(math.pi / 2)
        except Exception:
            target_bin = None
    else:
        target_bin = None

    # Only draw histogram if VFH object exists
    if vfh is not None:
        plot_valley_overlay_into_axes(
            ax=ax_hist,
            vfh=vfh,
            target_bin=target_bin,
            chosen_bin=chosen_bin,
            show_threshold_lines=show_threshold_lines
        )
    else:
        # Playback mode: draw empty histogram panel
        ax_hist.set_title("Histogram (not available in playback)")
        ax_hist.set_xticks([])
        ax_hist.set_yticks([])

    # --- Debug Panel ---
    if show_debug and ax_debug is not None:
        render_debug_panel(
            ax_debug,
            valleys=valleys,
            valley_widths=valley_widths,
            chosen_bin=chosen_bin,
            steer=steer,
            steering_cost=steering_cost,
            mode=mode,
            vfh_star=vfh_star
        )

    # --- Render to Streamlit ---
    fig_placeholder.pyplot(fig)
    plt.close(fig)


# =========================
# Recording import/export
# =========================

def frame_to_serializable(frame: Dict[str, Any]) -> Dict[str, Any]:
    pose: Pose2D = frame["pose"]
    grid: OccupancyGrid = frame["grid"]
    return {
        "pose": {"x": pose.x, "y": pose.y, "theta": pose.theta},
        "grid": {
            "width": grid.width,
            "height": grid.height,
            "resolution": grid.resolution,
            "origin_x": grid.origin_x,
            "origin_y": grid.origin_y,
            "data": grid.data,
        },
        "ranges": frame["ranges"],
        "histogram": frame["histogram"],
        "valleys": frame["valleys"],
        "chosen_bin": frame["chosen_bin"],
        "steer": frame["steer"],
        "nodes": [],  # omit nodes for export to keep JSON small
        "step": frame["step"],
    }


def serializable_to_frame(obj: Dict[str, Any]) -> Dict[str, Any]:
    g = obj["grid"]
    grid = OccupancyGrid(
        width=g["width"],
        height=g["height"],
        resolution=g["resolution"],
        origin_x=g["origin_x"],
        origin_y=g["origin_y"],
        data=g["data"],
    )
    p = obj["pose"]
    pose = Pose2D(p["x"], p["y"], p["theta"])
    return {
        "pose": pose,
        "grid": grid,
        "ranges": obj["ranges"],
        "histogram": obj["histogram"],
        "valleys": obj["valleys"],
        "chosen_bin": obj["chosen_bin"],
        "steer": obj["steer"],
        "nodes": [],  # we don't reconstruct nodes
        "step": obj["step"],
    }


def export_recording(frames: List[Dict[str, Any]]):
    if not frames:
        st.warning("No frames to export.")
        return
    serializable = [frame_to_serializable(f) for f in frames]
    data = json.dumps(serializable)
    st.download_button(
        "Download Recording JSON",
        data=data,
        file_name="vfh_recording.json",
        mime="application/json",
    )


def import_recording(uploaded_file) -> List[Dict[str, Any]]:
    content = uploaded_file.read()
    objs = json.loads(content.decode("utf-8"))
    return [serializable_to_frame(o) for o in objs]


# =========================
# MP4 export
# =========================

def render_frame_to_image(frame: Dict[str, Any]) -> np.ndarray:
    fig, ax = plt.subplots(figsize=(6, 6))
    grid: OccupancyGrid = frame["grid"]
    pose: Pose2D = frame["pose"]
    ranges = frame["ranges"]
    steer = frame["steer"]
    nodes = frame.get("nodes", [])
    step = frame["step"]

    servo_min = 0.0
    servo_max = 180.0
    beam_count = len(ranges)
    servo_increment = (servo_max - servo_min) / (beam_count - 1) if beam_count > 1 else 1.0

    plot_grid_matplotlib(
        ax=ax,
        grid=grid,
        pose=pose,
        ranges=ranges,
        servo_min_deg=servo_min,
        servo_increment_deg=servo_increment,
        vfh_star_nodes=nodes,
        steering_angle=steer,
        title=f"Step {step}"
    )

    fig.canvas.draw()
    img = np.frombuffer(fig.canvas.tostring_rgb(), dtype=np.uint8)
    img = img.reshape(fig.canvas.get_width_height()[::-1] + (3,))
    plt.close(fig)
    return img


def export_mp4(frames: List[Dict[str, Any]], fps: int = 10):
    if not frames:
        st.warning("No frames to export as MP4.")
        return

    images = [render_frame_to_image(f) for f in frames]

    with tempfile.NamedTemporaryFile(suffix=".mp4", delete=False) as tmp:
        imageio.mimsave(tmp.name, images, fps=fps)
        tmp.seek(0)
        video_bytes = tmp.read()

    st.download_button(
        "Download Playback as MP4",
        data=video_bytes,
        file_name="vfh_playback.mp4",
        mime="video/mp4",
    )


# =========================
# World builder
# =========================

def set_occ_world(grid: OccupancyGrid, wx: float, wy: float):
    ix = int((wx - grid.origin_x) / grid.resolution)
    iy = int((wy - grid.origin_y) / grid.resolution)

    if 0 <= ix < grid.width and 0 <= iy < grid.height:
        grid.data[grid.index(ix, iy)] = 100


def add_square(grid: OccupancyGrid, cx: float, cy: float, half: float):
    x = cx - half
    while x <= cx + half + 1e-9:
        y = cy - half
        while y <= cy + half + 1e-9:
            set_occ_world(grid, x, y)
            y += grid.resolution
        x += grid.resolution


def build_world_obstacles(grid: OccupancyGrid):
    # Walls
    for iy in range(grid.height):
        wy = grid.origin_y + (iy + 0.5) * grid.resolution
        set_occ_world(grid, -2.5, wy) # Left wall
        set_occ_world(grid, 2.5, wy)  # Right wall

    # Boxes
    add_square(grid, -0.6, 0.0, 0.2)
    add_square(grid, 0.6, -1.0, 0.2)

    # Horizontal barrier
    x = -2.5
    while x <= 0.0 + 1e-9:
        set_occ_world(grid, x, 1.5)
        x += grid.resolution


# =========================
# Main app
# =========================
def main():
    ss = st.session_state

    # --- 1. Initialize session state ---
    if "running" not in ss:
        ss.running = False
    if "paused" not in ss:
        ss.paused = False
    if "frames" not in ss:
        ss.frames = []
    if "playback_mode" not in ss:
        ss.playback_mode = False
    if "playback_index" not in ss:
        ss.playback_index = 0

    st.title("🤖 VFH* Real-Time Navigation Simulator")
    playback_fps = st.sidebar.slider("Playback Speed (FPS)", 1, 60, 10, 1)
    
    fig_placeholder = st.empty()

    # ============================================================
    # 2. PLAYBACK MODE
    # ============================================================
    if ss.playback_mode and ss.frames:
        ss.playback_index = max(0, min(ss.playback_index, len(ss.frames) - 1))
        frame = ss.frames[ss.playback_index]

        pose = frame["pose"]
        grid = frame["grid"]
        ranges = frame["ranges"]
        histogram = frame["histogram"]
        valleys = frame["valleys"]
        chosen_bin = frame["chosen_bin"]
        steer = frame["steer"]
        step = frame["step"]
        frame_robot_radius = frame.get("robot_radius", 0.12)
        frame_safety_dist  = frame.get("safety_dist", 0.10)
        frame_max_range    = frame.get("max_range", 4.0)


        # Per-frame playback params
        num_bins = len(histogram)
        frame_threshold_low  = frame.get("threshold_low",  0.5)
        frame_threshold_high = frame.get("threshold_high", 1.0)
        frame_kernel         = frame.get("smoothing_kernel", 2)

        vfh_playback = VFHPlus(
            num_bins=num_bins,
            max_range=frame_max_range,
            safety_dist=frame_safety_dist,
            robot_radius=frame_robot_radius,
            threshold_low=frame_threshold_low,
            threshold_high=frame_threshold_high,
        )

        vfh_playback.histogram = histogram
        vfh_playback.last_valleys = valleys
        vfh_playback.smoothing_kernel = frame_kernel

        valley_widths = [v[1] - v[0] + 1 for v in valleys]

        render_frame(
            fig_placeholder=fig_placeholder,
            grid=grid,
            pose=pose,
            ranges=ranges,
            steer=steer,
            nodes_to_plot=[],
            valleys=valleys,
            valley_widths=valley_widths,
            chosen_bin=chosen_bin,
            steering_cost=abs(steer),
            mode="VFH*",
            step=step,
            show_debug=True,
            show_threshold_lines=True,
            vfh=vfh_playback,
            vfh_star=None,
        )

        # Advance playback
        time.sleep(1.0 / playback_fps)
        
        if ss.playback_index < len(ss.frames) - 1:
            ss.playback_index += 1
            st.rerun()
        else:
            st.success("Playback finished.")
            ss.playback_mode = False
            ss.playback_index = 0
            # DO NOT return — allow UI to render


    # ============================================================
    # 3. UI CONTROLS (run only when NOT in playback)
    # ============================================================
    update_interval = st.sidebar.slider("Update interval (seconds)", 0.01, 1.0, 0.1)
    steps_to_run = st.sidebar.slider("Simulation steps", 10, 500, 100)
    show_debug = st.sidebar.checkbox("Show Debug Panel", value=True)

    mode = st.sidebar.radio("Select Navigation Mode", ["VFH+", "VFH*"], index=1)

    st.sidebar.markdown("### Robot Geometry")
    robot_radius = st.sidebar.slider("Robot Radius (m)", 0.05, 0.5, 0.12, 0.01)
    safety_dist = st.sidebar.slider("Safety Distance (m)", 0.05, 0.5, 0.10, 0.01)
    
    st.sidebar.markdown("### Lidar Settings")
    max_range = st.sidebar.slider("Lidar Max Range (m)", 1.0, 10.0, 4.0, 0.5)


    st.sidebar.markdown("### VFH Thresholds")
    threshold_low = st.sidebar.slider("Low Threshold", 0.0, 5.0, 0.5, 0.05)
    threshold_high = st.sidebar.slider("High Threshold", 0.0, 5.0, 1.0, 0.05)

    st.sidebar.markdown("### VFH Smoothing & Search Controls")
    kernel = st.sidebar.slider("Histogram smoothing kernel (± bins)", 0, 10, 2, 1)
    lambda_slider = st.sidebar.slider("VFH* λ (cost decay)", 0.1, 2.0, 0.7, 0.05)
    ng_slider = st.sidebar.slider("VFH* search depth (ng)", 1, 15, 5, 1)

    show_threshold_lines = st.sidebar.checkbox("Show threshold lines on histogram", True)

    col1, col2, col3 = st.sidebar.columns(3)
    start_btn = col1.button("Start")
    pause_btn = col2.button("Pause")
    reset_btn = col3.button("Reset")

    st.sidebar.markdown("### Recording Export / Import")

    if st.sidebar.button("Export Recording JSON"):
        export_recording(ss.frames)

    uploaded = st.sidebar.file_uploader("Import Recording", type=["json"])
    if uploaded is not None:
        ss.frames = import_recording(uploaded)
        ss.playback_mode = True
        ss.running = False
        ss.paused = False
        ss.playback_index = 0
        st.success("Recording imported. Playback ready.")
        st.rerun()

    st.sidebar.markdown("### Playback Controls")
    playback_mode_checkbox = st.sidebar.checkbox("Playback Mode", value=ss.playback_mode)
    ss.playback_mode = playback_mode_checkbox

    #playback_fps = st.sidebar.slider("Playback Speed (FPS)", 1, 60, 10, 1)

    if st.sidebar.button("Export playback as MP4"):
        export_mp4(ss.frames, fps=playback_fps)

    # --- Button logic ---
    if start_btn:
        ss.running = True
        ss.paused = False
        ss.playback_mode = False
        ss.frames = []
        ss.playback_index = 0

    if pause_btn:
        ss.paused = not ss.paused

    if reset_btn:
        ss.running = False
        ss.paused = False
        ss.frames = []
        ss.playback_mode = False
        ss.playback_index = 0
        st.info("Simulation reset. Adjust parameters and press Start.")
        return

    # ============================================================
    # 4. LIVE SIMULATION (only when running)
    # ============================================================
    if not ss.running:
        st.info("Adjust parameters, then click **Start** to begin.")
        return

    # --- Initialize world ---
    grid = OccupancyGrid(
        width=120,
        height=120,
        resolution=0.05,
        origin_x=-3.0,
        origin_y=-3.0,
        data=[0] * (120 * 120)
    )

    build_world_obstacles(grid)
    static_data = grid.data[:]

    pose = Pose2D(-1.0, -2.25, math.pi / 2)

    vfh = VFHPlus(
        num_bins=72,
        max_range=max_range,
        safety_dist=safety_dist,
        robot_radius=robot_radius,
        threshold_low=threshold_low,
        threshold_high=threshold_high
    )

    vfh.smoothing_kernel = kernel

    vfh_star = VFHStar(
        vfh=vfh,
        ng=ng_slider,
        step_dist=0.15,
        lambda_=lambda_slider
    )


    servo_min = 0.0
    servo_max = 180.0
    beam_count = 181
    max_range = 4.0
    servo_increment = (servo_max - servo_min) / (beam_count - 1)

    target_angle = math.pi / 2

    for step in range(steps_to_run):
        if ss.paused:
            st.warning("Simulation paused.")
            return

        vfh.threshold_low = threshold_low
        vfh.threshold_high = threshold_high
        vfh.smoothing_kernel = kernel
        vfh_star.lambda_ = lambda_slider
        vfh_star.ng = ng_slider

        ranges = simulate_lidar_scan_servo(
            pose, grid,
            servo_min_deg=servo_min,
            servo_max_deg=servo_max,
            beam_count=beam_count,
            max_range=max_range
        )

        update_grid_from_lidar(pose, ranges, servo_min, servo_increment, max_range, grid)
        decay_grid(grid)
        #inflate_obstacles(grid, static_data, robot_radius=0.12, safety_dist=0.1)
        inflate_obstacles(grid, static_data, robot_radius=robot_radius, safety_dist=safety_dist)

        if mode == "VFH+":
            cand_bins = vfh.compute_candidate_bins(pose, grid, target_angle)
            if cand_bins:
                chosen_bin = cand_bins[0]
                steer = vfh.bin_to_steering_angle(chosen_bin, target_angle)
                ok = True
            else:
                ok = False
                steer = 0.0
        else:
            ok, steer = vfh_star.compute_steering_angle(pose, grid, target_angle)

        if not ok:
            st.error("No steering solution — robot stuck.")
            ss.running = False
            return

        valleys = vfh.last_valleys
        valley_widths = [(e - s + 1) for s, e in valleys]

        if mode == "VFH+":
            steering_cost = abs(steer)
        else:
            chosen_bin = vfh.angle_to_bin(steer)
            steering_cost = vfh_star.local_cost(steer, target_angle)

        new_theta = VFHPlus.normalize_angle(pose.theta + steer)
        pose = Pose2D(
            pose.x + 0.1 * math.cos(new_theta),
            pose.y + 0.1 * math.sin(new_theta),
            new_theta
        )

        nodes_to_plot = vfh_star.nodes if mode == "VFH*" else []

        snapshot_grid = OccupancyGrid(
            width=grid.width,
            height=grid.height,
            resolution=grid.resolution,
            origin_x=grid.origin_x,
            origin_y=grid.origin_y,
            data=grid.data[:],
        )
        snapshot_pose = Pose2D(pose.x, pose.y, pose.theta)

        ss.frames.append({
            "pose": snapshot_pose,
            "grid": snapshot_grid,
            "ranges": ranges[:],
            "histogram": vfh.histogram[:],
            "valleys": valleys[:],
            "chosen_bin": chosen_bin,
            "steer": steer,
            "nodes": [],
            "step": step,
            "threshold_low": vfh.threshold_low,
            "threshold_high": vfh.threshold_high,
            "smoothing_kernel": vfh.smoothing_kernel,
            "num_bins": vfh.num_bins,
            "robot_radius": robot_radius,
            "safety_dist": safety_dist,
            "max_range": max_range,
        })

        render_frame(
            fig_placeholder=fig_placeholder,
            grid=grid,
            pose=pose,
            ranges=ranges,
            steer=steer,
            nodes_to_plot=nodes_to_plot,
            valleys=valleys,
            valley_widths=valley_widths,
            chosen_bin=chosen_bin,
            steering_cost=steering_cost,
            mode=mode,
            step=step,
            show_debug=show_debug,
            show_threshold_lines=show_threshold_lines,
            vfh=vfh,
            vfh_star=vfh_star
        )

        time.sleep(update_interval)

    ss.running = False
    st.success("Simulation complete.")


if __name__ == "__main__":
    main()
