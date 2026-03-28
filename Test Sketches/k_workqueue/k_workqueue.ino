/*
 * Example: Using a custom k_work_queue in Zephyr
 * Demonstrates immediate and delayed work submission.
 */

#include <zephyr/kernel.h>
#include <LibPrintf.h>

/* Stack size and priority for the custom work queue thread */
#define MY_WORKQ_STACK_SIZE 1024
#define MY_WORKQ_PRIORITY   5

/* Declare stack memory for the work queue thread */
K_THREAD_STACK_DEFINE(my_workq_stack, MY_WORKQ_STACK_SIZE);

/* Declare the work queue object */
static struct k_work_q my_workq;

/* Define a work item (immediate execution) */
static struct k_work my_work;

/* Define a delayed work item */
static struct k_work_delayable my_delayed_work;

/* Immediate work handler */
static void my_work_handler(struct k_work *work)
{
    ARG_UNUSED(work);
    printf("Immediate work executed at %lld ms\n", k_uptime_get());
}

/* Delayed work handler */
static void my_delayed_work_handler(struct k_work *work)
{
    ARG_UNUSED(work);
    printk("Delayed work executed at %lld ms\n", k_uptime_get());
}

void setup(void)
{
    Serial.begin(115200);
    
    printf("Zephyr custom k_work_queue example\n");

    /* Start the custom work queue */
    k_work_queue_start(&my_workq, my_workq_stack,
                       K_THREAD_STACK_SIZEOF(my_workq_stack),
                       MY_WORKQ_PRIORITY, NULL);

    /* Initialize work items */
    k_work_init(&my_work, my_work_handler);
    k_work_init_delayable(&my_delayed_work, my_delayed_work_handler);

    /* Submit immediate work to the custom queue */
    k_work_submit_to_queue(&my_workq, &my_work);

    /* Submit delayed work (2 seconds delay) */
    k_work_schedule_for_queue(&my_workq, &my_delayed_work, K_SECONDS(2));

    printf("Work items submitted at %lld ms\n", k_uptime_get());
}

void loop() {}