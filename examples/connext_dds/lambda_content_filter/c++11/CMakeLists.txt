# (c) 2019 Copyright, Real-Time Innovations, Inc. All rights reserved.
# No duplications, whole or partial, manual or electronic, may be made
# without express written permission.  Any such copies, or revisions thereof,
# must display this notice unaltered.
# This code contains trade secrets of Real-Time Innovations, Inc.
cmake_minimum_required(VERSION 3.11)
project(rtiexamples-lambda-content-filter)
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../resources/cmake"
)

# Include ConnextDdsAddExample.cmake from resources/cmake
include(ConnextDdsAddExample)

connextdds_add_example(
    IDL "LambdaFilterExample"
    LANG "C++11"
    NO_REQUIRE_QOS
)

# To include the LambdaFilter.hpp file
target_include_directories(lambda_content_filter_publisher_cpp2
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(lambda_content_filter_subscriber_cpp2
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}")
