cmake_minimum_required(VERSION 3.5)

project(lidar_for_kotlin)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(
  include
  include/${PROJECT_NAME}
  /usr/local/include
  /usr/lib/jvm/java-17-openjdk-amd64/include
  /usr/lib/jvm/java-17-openjdk-amd64/include/linux
  ${JNI_INCLUDE_DIRS}
)

find_package(ament_cmake REQUIRED)
find_package(rcl REQUIRED)
find_package(rclc REQUIRED)
find_package(rcutils REQUIRED)
find_package(rclc_lifecycle REQUIRED)
find_package(rmw REQUIRED)
find_package(rmw_implementation_cmake REQUIRED)
find_package(rosidl_generator_c REQUIRED)
find_package(rosidl_runtime_c REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(JNI REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")

set(SOURCE_FILES
    include/lidar_for_kotlin/net_wavem_lidar_lidar_for_kotlin.hpp
    src/net_wavem_lidar_lidar_for_kotlin.cpp
    include/lidar_for_kotlin/utils.hpp
    src/utils.cpp
)

get_rmw_typesupport(typesupport_impls "rmw_implementation" LANGUAGE "c")
if(typesupport_impls STREQUAL "")
  message(STATUS "Skipping rcljava because no C typesupport library was found.")
  return()
endif()

set(RCL_DEPENDENCIES
  rcl
  rclc
  rcutils
  rmw
  rclc_lifecycle
  rmw_implementation_cmake
  std_msgs
  sensor_msgs
  rosidl_generator_c
  rosidl_runtime_c
)

add_library(lidar_for_kotlin
  SHARED
    ${SOURCE_FILES}
)

ament_target_dependencies(lidar_for_kotlin
  ${RCL_DEPENDENCIES}
)

target_include_directories(lidar_for_kotlin
    PUBLIC
      ${JNI_LIBRARIES}
)

ament_export_libraries(lidar_for_kotlin)

install(TARGETS lidar_for_kotlin
  LIBRARY DESTINATION "/usr/lib"
)

# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# set_target_properties(lidar_for_kotlin PROPERTIES OUTPUT_NAME rclkotlin_test)
# set(CMAKE_VERBOSE_MAKEFILE ON)
# set_target_properties(lidar_for_kotlin PROPERTIES
#     POSITION_INDEPENDENT_CODE TRUE
# )

# ament_package()
