cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(pcd_write)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
endif()

find_package(PCL 1.2 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable (csv_to_pcd src/csv_to_pcd.cpp)
target_link_libraries (csv_to_pcd ${PCL_LIBRARIES})

add_executable (batch_converter src/batch_converter.cpp)
target_link_libraries (batch_converter ${PCL_LIBRARIES})
