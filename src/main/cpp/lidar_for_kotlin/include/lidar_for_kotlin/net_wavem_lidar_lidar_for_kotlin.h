#ifndef LIDAR_FOR_KOTLIN__H
#define LIDAR_FOR_KOTLIN__H

#include <jni.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <rmw/rmw.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rcl/error_handling.h>
#include <rcutils/logging_macros.h>
#include <sensor_msgs/msg/laser_scan.h>

#ifndef _Included_JNI
#define _Included_JNI

#define nullptr 0

#define RCL_NODE_NAME "lidar_for_kotlin"
#define RCL_EXECUTOR_SPIN_RATE_MS 25
#define RCL_SCAN_TOPIC "/scan"
#define RCL_START_FLAG 1

#define RCCHECK(fn)                                                                                      \
    {                                                                                                    \
        rcl_ret_t temp_rc = fn;                                                                          \
        if ((temp_rc != RCL_RET_OK))                                                                     \
        {                                                                                                \
            RCUTILS_LOG_INFO_NAMED(RCL_NODE_NAME,                                                        \
                                   "Failed status on line %d: %d. Aborting.\n", __LINE__, (int)temp_rc); \
            return;                                                                                      \
        }                                                                                                \
    }
#define RCSOFTCHECK(fn)                                                                                    \
    {                                                                                                      \
        rcl_ret_t temp_rc = fn;                                                                            \
        if ((temp_rc != RCL_RET_OK))                                                                       \
        {                                                                                                  \
            RCUTILS_LOG_INFO_NAMED(RCL_NODE_NAME,                                                          \
                                   "Failed status on line %d: %d. Continuing.\n", __LINE__, (int)temp_rc); \
        }                                                                                                  \
    }


void rcl_scan_callback(const void * rcl_scan_ptr);

rcl_ret_t rc_kt;
rcl_allocator_t rcl_allocator_kt;
rclc_support_t *rclc_support_kt;
rclc_executor_t rcl_executor;

JNIEXPORT jlong JNICALL Java_net_wavem_lidar_LiDARNode_nativeRCLKotlinInit(JNIEnv *j_env, jclass j_class);
JNIEXPORT jlong JNICALL Java_net_wavem_lidar_LiDARNode_nativeCreateNode(JNIEnv *j_env, jclass j_class, jlong j_rclkotlin_init_result);
JNIEXPORT jlong JNICALL Java_net_wavem_lidar_LiDARNode_nativeSpin(JNIEnv *j_env, jclass j_class);

JNIEXPORT void JNICALL Java_net_wavem_lidar_LiDARNode_nativeNodeRun(JNIEnv *j_env, jclass j_class);

#endif
#endif