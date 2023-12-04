cmake_minimum_required(VERSION 3.10)

project(lab04)

set(C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${INCLUDE_DIR})

add_library(lib1 SHARED ${SOURCE_DIR}/lib_lab04_impl1.c)
target_include_directories(lib1 PUBLIC ${INCLUDE_DIR})
add_library(lib2 SHARED ${SOURCE_DIR}/lib_lab04_impl2.c)
target_include_directories(lib2 PUBLIC ${INCLUDE_DIR})

add_executable(main1_impl1 ${CMAKE_CURRENT_SOURCE_DIR}/main1.c)
add_executable(main1_impl2 ${CMAKE_CURRENT_SOURCE_DIR}/main1.c)

target_link_libraries(main1_impl1 PRIVATE lib1 PRIVATE m)
target_link_libraries(main1_impl2 PRIVATE lib2 PRIVATE m)
