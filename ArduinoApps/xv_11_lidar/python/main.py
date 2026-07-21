from arduino.app_utils import App

import streamlit as st
import numpy as np
import pandas as pd
import requests
import time
import plotly.graph_objects as go
from arduino.app_utils import *

REFRESH_RATE = 0.5  # seconds

def fetch_lidar():
    """
    Calls UNO Q get_range RPC.
    Expected format per line:
    A,<angle>,<range>,<quality>,<flag>
    """
    try:
        angles, flags, quality, ranges = Bridge.call("get_range")
        #print(f"Received {len(angles)} points")
        #print(f"First point: angle={angles[0]}, flag={flags[0]}, "
        #            f"quality={quality[0]}, range={ranges[0]}")

        # Ensure everything is a numpy array
        angles = np.array(angles, dtype=np.int32)
        ranges = np.array(ranges, dtype=np.int32)
        quality = np.array(quality, dtype=np.int32)
        flags = np.array(flags, dtype=np.int32)

        # Validate shape
        if angles.ndim != 1 or len(angles) == 0:
            raise ValueError("Invalid LIDAR data: angles not 1‑D")

        return angles, ranges, quality, flags

    except Exception as e:
        st.error(f"Error fetching LIDAR data: {e}")
        return None, None, None, None

def polar_plot(angles, ranges, flags):
    """Plotly polar scatter plot."""
    # Convert degrees to radians
    theta = np.radians(angles)

    # Color by flag
    flag_to_color = np.array(["cyan", "red", "pink", "gray"])
    colors = flag_to_color[np.clip(flags, 0, 3)]


    fig = go.Figure()

    fig.add_trace(go.Scatterpolar(
        r=ranges,
        theta=angles,
        mode="markers",
        marker=dict(size=6, color=colors),
    ))

    fig.update_layout(
        polar=dict(
            radialaxis=dict(range=[0, max(2000, np.max(ranges))])
        ),
        showlegend=False,
        height=600,
    )

    return fig

# -----------------------------
# STREAMLIT UI
# -----------------------------
st.title("🔭 Real‑Time LIDAR Viewer (UNO Q)")

col1, col2 = st.columns([2, 1])

# Live update checkbox
auto_refresh = st.sidebar.checkbox("Auto‑refresh", value=True)
refresh_rate = st.sidebar.slider("Refresh rate (seconds)", 0.25, 1.0, REFRESH_RATE)

# Main loop
placeholder_plot = col1.empty()
placeholder_table = col2.empty()

def main():
    t = 0
    while True:
        #start_time = time.perf_counter()
        angles, ranges, quality, flags = fetch_lidar()
        #end_time =  time.perf_counter()
        #duration_ms = (end_time - start_time) * 1000
        #print(duration_ms)
        
        if angles is not None:
            # --- POLAR PLOT ---
            fig = polar_plot(angles, ranges, flags)
            placeholder_plot.plotly_chart(fig, width='content', key=t)
    
            # --- TABLE ---
            df = pd.DataFrame({
                "Angle": angles,
                "Distance (mm)": ranges,
                "Quality": quality,
                "Flag": flags
            })
    
            placeholder_table.dataframe(df, height=600)
    
        if not auto_refresh:
            return
    
        time.sleep(refresh_rate)
        if t>1000000:
            t=0
        t=t+1


if __name__ == "__main__":
    time.sleep(5)
    main()
