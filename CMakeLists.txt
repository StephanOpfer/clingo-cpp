cmake_minimum_required(VERSION 2.8.3)
project(clingo_cpp)

find_package(catkin REQUIRED)

catkin_package(
   INCLUDE_DIRS libgringo libgringo/src libprogram_opts libclasp libclingo
   LIBRARIES clingo_shared program_opts clasp gringo
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclingo_shared.a
           ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclasp.a
           ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libgringo.a
           ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libprogram_opts.a
    COMMAND scons --build-dir=release
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/libclingo/src/clingocontrol.cc
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "${PROJECT_NAME}: Custom-Command: Compiling the ASP Solver clingo with \"scons --build-dir=release\"."
)

add_custom_command(
    OUTPUT ${CATKIN_DEVEL_PREFIX}/lib/libclingo.a
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CATKIN_DEVEL_PREFIX}/lib
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclingo_shared.a ${CATKIN_DEVEL_PREFIX}/lib/.
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclasp.a ${CATKIN_DEVEL_PREFIX}/lib/.
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libgringo.a ${CATKIN_DEVEL_PREFIX}/lib/.
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libprogram_opts.a ${CATKIN_DEVEL_PREFIX}/lib/.
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclingo.a
            ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclasp.a
            ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libgringo.a
            ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libprogram_opts.a
    COMMENT "${PROJECT_NAME}: Copying static libraries libclingo.a, libclasp.a, libgringo.a, and libprogram_opts.a into devel/lib folder."
)

add_custom_target(${PROJECT_NAME} ALL 
    DEPENDS ${CATKIN_DEVEL_PREFIX}/lib/libclingo.a
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "${PROJECT_NAME}: Compiling and copying libraries."
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclingo.a
)
