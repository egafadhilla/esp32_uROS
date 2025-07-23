#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/string.h>
#include <rmw_microros/rmw_microros.h>

static rcl_publisher_t publisher;
static std_msgs__msg__String msg;

void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{
    (void) last_call_time;
    if (timer != NULL) {
        msg.data.data = "Hello World from micro-ROS!";
        msg.data.size = strlen(msg.data.data);
        msg.data.capacity = msg.data.size + 1;
        rcl_publish(&publisher, &msg, NULL);
    }
}

void app_main(void)
{
    // micro-ROS transport: UART, WiFi, atau Ethernet. Contoh UART:
    rmw_uros_set_custom_transport(
        true,
        (void *) &uart_num,
        micro_ros_serial_transport_open,
        micro_ros_serial_transport_close,
        micro_ros_serial_transport_write,
        micro_ros_serial_transport_read
    );

    rcl_allocator_t allocator = rcl_get_default_allocator();
    rclc_support_t support;
    rcl_node_t node;
    rcl_timer_t timer;
    rclc_executor_t executor;

    // Inisialisasi micro-ROS
    rclc_support_init(&support, 0, NULL, &allocator);
    rclc_node_init_default(&node, "uros_node", "", &support);
    rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), "uros_chatter");
    rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(1000), timer_callback);
    rclc_executor_init(&executor, &support.context, 1, &allocator);
    rclc_executor_add_timer(&executor, &timer);

    // Loop executor
    while (1) {
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    // (opsional) cleanup jika diperlukan
    // rcl_publisher_fini(&publisher, &node);
    // rcl_node_fini(&node);
}
