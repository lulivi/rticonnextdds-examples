#
# (c) 2021 Copyright, Real-Time Innovations, Inc.  All rights reserved.
#
#  RTI grants Licensee a license to use, modify, compile, and create derivative
#  works of the Software.  Licensee has the right to distribute object form
#  only for use with RTI products.  The Software is provided "as is", with no
#  warranty of any type, including any warranty for fitness for any purpose.
#  RTI is under no obligation to maintain or support the Software.  RTI shall
#  not be liable for any incidental or consequential damages arising out of the
#  use or inability to use the software.
#
cmake_minimum_required(VERSION 3.11)
project(FileAdapterC)

if(NOT DEFINED CONNEXTDDS_DIR)
    set(msg 
        "Please set the CMake variable CONNEXTDDS_DIR "
        "to point to your installation of RTI Connext DDS")
    message(FATAL_ERROR ${msg})
endif()

if (NOT UNIX)
    set(msg 
        "This example uses the pthreads library "
        "which is only available on UNIX-like systems")
    message(FATAL_ERROR ${msg})
endif()

# Find RTI Connext dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CONNEXTDDS_DIR}/resource/cmake")
find_package(
    RTIConnextDDS "6.1.0"
    REQUIRED
    COMPONENTS
        core
        routing_service
)

add_library(fileadapter
    "${CMAKE_CURRENT_SOURCE_DIR}/directory_reading.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/line_conversion.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/file_adapter.c"
)

target_link_libraries(fileadapter
    RTIConnextDDS::routing_service_infrastructure
)

# To differentiate between debug and release builds
set_target_properties(fileadapter
    PROPERTIES
        DEBUG_POSTFIX "d"
)

ADD_CUSTOM_COMMAND(TARGET fileadapter
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/file_bridge.xml"
        "${CMAKE_BINARY_DIR}/file_bridge.xml"
)
