cmake_minimum_required(VERSION 3.5)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(hello_world)
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components $ENV{IDF_PATH}/components ./components/micro_ros_espidf_component)
