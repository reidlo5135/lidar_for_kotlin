#include "lidar_for_kotlin/net_wavem_lidar_lidar_for_kotlin.hpp"

void rcl_scan_callback(const void *rcl_scan_ptr)
{
}

JNIEXPORT void JNICALL Java_net_wavem_lidar_LiDARNode_nativeNodeRun(JNIEnv *env, jclass)
{
    int argc = 0;
    char **argv = nullptr;

    rcl_allocator_t rcl_allocator = rcl_get_default_allocator();
    rclc_support_t rcl_support;
    rcl_ret_t rc;

    RCUTILS_LOGGING_AUTOINIT;
    rcutils_logging_set_logger_level(RCL_NODE_NAME, RCUTILS_LOG_SEVERITY_INFO);

    rc = rclc_support_init(&rcl_support, argc, argv, &rcl_allocator);

    RCCHECK(rc);
    if (rc != RCL_RET_OK)
    {
        RCUTILS_LOG_ERROR_NAMED(RCL_NODE_NAME, "Failed to initialize rclc, error code RC [%i]\n", rc);
        return;
    }
    else
    {
        RCUTILS_LOG_INFO_NAMED(RCL_NODE_NAME, "Succeeded to initialize rclc, success code RC [%i]\n", rc);
    }

    rcl_node_t rcl_node = rcl_get_zero_initialized_node();
    rc = rclc_node_init_default(&rcl_node, "tester", "", &rcl_support);

    RCCHECK(rc);
    if (rc != RCL_RET_OK)
    {
        RCUTILS_LOG_ERROR_NAMED(RCL_NODE_NAME, "Failed to initialized rclc_node, error code RC [%i]\n", rc);
        return;
    }
    else
    {
        RCUTILS_LOG_INFO_NAMED(RCL_NODE_NAME, "Succeeded to initialize rclc_node, success code RC [%i]\n", rc);
    }

    rcl_subscription_t rcl_scan_subscription = rcl_get_zero_initialized_subscription();
    const char *rcl_scan_topic = RCL_SCAN_TOPIC;
    const rosidl_message_type_support_t *rcl_scan_message_type = ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, LaserScan);

    rc = rclc_subscription_init_default(
        &rcl_scan_subscription,
        &rcl_node,
        rcl_scan_message_type,
        rcl_scan_topic);

    RCCHECK(rc);
    if (rc != RCL_RET_OK)
    {
        RCUTILS_LOG_ERROR_NAMED(RCL_NODE_NAME, "Failed to create %s subscription, error code RC [%i]\n", rcl_scan_topic, rc);
        return;
    }
    else
    {
        RCUTILS_LOG_INFO_NAMED(RCL_NODE_NAME, "Succeeded to create %s subscription, success code RC [%i]\n", rcl_scan_topic, rc);
    }

    rclc_executor_t rcl_executor;
    rcl_executor = rclc_executor_get_zero_initialized_executor();

    unsigned int rcl_dds_handle = 4;
    RCUTILS_LOG_INFO_NAMED(RCL_NODE_NAME, "number of DDS handles: %u\n", rcl_dds_handle);
    rclc_executor_init(&rcl_executor, &rcl_support.context, rcl_dds_handle, &rcl_allocator);

    sensor_msgs__msg__LaserScan rcl_laser_scan_msgs;
    sensor_msgs__msg__LaserScan__init(&rcl_laser_scan_msgs);
    rc = rclc_executor_add_subscription(&rcl_executor, &rcl_scan_subscription, &rcl_laser_scan_msgs, &rcl_scan_callback, ON_NEW_DATA);

    RCCHECK(rc);
    if (rc != RCL_RET_OK)
    {
        RCUTILS_LOG_ERROR_NAMED(RCL_NODE_NAME, "Failed to add %s subscription into rcl executor, error code RC [%i]\n", rcl_scan_topic, rc);
    }
    else
    {
        RCUTILS_LOG_INFO_NAMED(RCL_NODE_NAME, "Succeeded to add %s subscription into rcl executor, success code RC [%i]\n", rcl_scan_topic, rc);
    }

    while (1)
    {
        rc = rclc_executor_spin_some(&rcl_executor, RCL_MS_TO_NS(RCL_EXECUTOR_SPIN_RATE_MS));

        RCCHECK(rc);
        if (rc != RCL_RET_OK)
        {
            RCUTILS_LOG_ERROR_NAMED(RCL_NODE_NAME, "Failed running the rclc executor with RC [%i]\n", rc);
            return;
        }
        else
        {
            RCUTILS_LOG_INFO_NAMED(RCL_NODE_NAME, "manual move rclc executor is on running with RC [%i]\n", rc);
        }
    }

    rc = rclc_executor_fini(&rcl_executor);
    rc += rcl_subscription_fini(&rcl_scan_subscription, &rcl_node);
    rc += rcl_node_fini(&rcl_node);
    rc += rclc_support_fini(&rcl_support);

    sensor_msgs__msg__LaserScan__fini(&rcl_laser_scan_msgs);

    if (rc != RCL_RET_OK)
    {
        RCUTILS_LOG_ERROR_NAMED(RCL_NODE_NAME, "Failed to fini rcl, error code RC [%i]\n", rc);
        return;
    }
    else
    {
        RCUTILS_LOG_INFO_NAMED(RCL_NODE_NAME, "Succeeded to fini rcl, success code RC [%i]\n", rc);
    }
}