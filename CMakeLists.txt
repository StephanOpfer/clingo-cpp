cmake_minimum_required(VERSION 2.8.3)
project(clingo-cpp)


find_package(catkin REQUIRED)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES clingo-cpp
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

message(" #### Project Binary: ${PROJECT_BINARY_DIR}")
message(" #### Project Source: ${CMAKE_CURRENT_SOURCE_DIR}")
message(" #### Project Bin Destination: ${CATKIN_DEVEL_PREFIX}")

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclingo.so
    #COMMAND rosrun udp_proxy_generator udp_proxy_generator ${PROJECT_NAME}
    COMMAND scons configure --build-dir=release
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/libclingo/src/clingocontrol.cc
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E make_directory ${CATKIN_DEVEL_PREFIX}
       COMMAND ${CMAKE_COMMAND} -E copy build/release/libclingo.so ${CATKIN_DEVEL_PREFIX}/.
    COMMENT "${PROJECT_NAME}: Custom-Command: Compiling the ASP Solver clingo with \"scons\"."
)

add_custom_target(clingo ALL 
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclingo.so
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "${PROJECT_NAME}: Custom-Target: Compiling the ASP Solver clingo with \"scons\"."
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclingo.so
)
