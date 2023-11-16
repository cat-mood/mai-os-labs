cmake_minimum_required(VERSION 3.10)
project(lab03)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# add_compile_options(-pthread)

# set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

# file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false ${SOURCE_DIR}/*.cpp)

# add_library(${CMAKE_PROJECT_NAME}_lib ${SOURCES})
add_executable(main parent.cpp)
add_executable(child1 child1.cpp)
add_executable(child2 child2.cpp)

# target_link_libraries(main PRIVATE ${CMAKE_PROJECT_NAME}_lib)
# target_link_libraries(child1 PRIVATE ${CMAKE_PROJECT_NAME}_lib)
# target_link_libraries(child2 PRIVATE ${CMAKE_PROJECT_NAME}_lib)
