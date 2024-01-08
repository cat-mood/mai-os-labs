cmake_minimum_required(VERSION 3.10)

project(kp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false ${SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME}_lib ${SOURCES})
add_executable(${PROJECT_NAME}_server server_main.cpp)
add_executable(${PROJECT_NAME}_client client_main.cpp)
add_executable(${PROJECT_NAME}_game game_main.cpp)
add_executable(${PROJECT_NAME}_make_db make_db.cpp)

target_include_directories(${PROJECT_NAME}_lib PRIVATE ${INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_server PRIVATE ${PROJECT_NAME}_lib pthread)
target_link_libraries(${PROJECT_NAME}_client PRIVATE ${PROJECT_NAME}_lib pthread)
target_link_libraries(${PROJECT_NAME}_game PRIVATE ${PROJECT_NAME}_lib pthread)
