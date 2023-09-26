#ifndef LIDAR_FOR_KOTLIN__HPP
#define LIDAR_FOR_KOTLIN__HPP

#include <jni.h>

#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <rmw/rmw.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rcl/error_handling.h>
#include <rcutils/logging_macros.h>
#include <sensor_msgs/msg/laser_scan.h>
#include "lidar_for_kotlin/utils.hpp"

#ifndef _Included_JNI
#define _Included_JNI
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_net_wavem_lidar_LiDARNode_nativeNodeRun(JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif

static constexpr const char *RCL_NODE_NAME = "lidar_for_kotlin";
static constexpr const int &RCL_EXECUTOR_SPIN_RATE_MS = 25;
static constexpr const char *RCL_SCAN_TOPIC = "/scan";

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

#endif

#endif