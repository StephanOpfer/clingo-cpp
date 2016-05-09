cmake_minimum_required(VERSION 2.8.3)
project(clingo_cpp)
 
find_package(catkin REQUIRED cmake_modules)
# this is necessary for python and lua script support of libclingo
find_package(Lua51 REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(TBB REQUIRED)
include(cmake/tbb_extras.cmake)

catkin_package(
   INCLUDE_DIRS libgringo libgringo/src libprogram_opts libclasp libclingo ${LUA_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS} ${TBB_INCLUDE_DIRS}
   LIBRARIES  ${LUA_LIBRARIES} ${PYTHON_LIBRARIES} ${TBB_LIBRARIES}
   CFG_EXTRAS tbb_extras.cmake
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclingo_shared.a
           ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclasp_shared.a
           ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libgringo_shared.a
           ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libprogram_opts_shared.a
    COMMAND scons --build-dir=release
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/libclingo/src/clingocontrol.cc
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "${PROJECT_NAME}: Custom-Command: Compiling the ASP Solver clingo with \"scons --build-dir=release\"."
)

add_custom_command(
    OUTPUT ${CATKIN_DEVEL_PREFIX}/lib/libclingo_shared.a
	   ${CATKIN_DEVEL_PREFIX}/lib/libclasp_shared.a
	   ${CATKIN_DEVEL_PREFIX}/lib/libgringo_shared.a
	   ${CATKIN_DEVEL_PREFIX}/lib/libprogram_opts_shared.a
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CATKIN_DEVEL_PREFIX}/lib
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclingo_shared.a ${CATKIN_DEVEL_PREFIX}/lib/.
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclasp_shared.a ${CATKIN_DEVEL_PREFIX}/lib/.
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libgringo_shared.a ${CATKIN_DEVEL_PREFIX}/lib/.
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libprogram_opts_shared.a ${CATKIN_DEVEL_PREFIX}/lib/.
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclingo_shared.a
            ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclasp_shared.a
            ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libgringo_shared.a
            ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libprogram_opts_shared.a
    COMMENT "${PROJECT_NAME}: Copying static libraries libclingo_shared.a, libclasp_shared.a, libgringo_shared.a, and libprogram_opts_shared.a into ${CATKIN_DEVEL_PREFIX}/lib folder."
)

add_custom_target(clingo_shared ALL 
    DEPENDS ${CATKIN_DEVEL_PREFIX}/lib/libclingo_shared.a
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "${PROJECT_NAME}: Compiling and copying libclingo_shared.a"
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclingo_shared.a
)

add_custom_target(program_opts_shared ALL 
    DEPENDS ${CATKIN_DEVEL_PREFIX}/lib/libprogram_opts_shared.a
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "${PROJECT_NAME}: Compiling and copying libprogram_opts_shared.a"
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libprogram_opts_shared.a
)

add_custom_target(clasp_shared ALL 
    DEPENDS ${CATKIN_DEVEL_PREFIX}/lib/libclasp_shared.a
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "${PROJECT_NAME}: Compiling and copying libclasp_shared.a"
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libclasp_shared.a
)

add_custom_target(gringo_shared ALL 
    DEPENDS ${CATKIN_DEVEL_PREFIX}/lib/libgringo_shared.a
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "${PROJECT_NAME}: Compiling and copying libgringo_shared.a"
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/build/release/libgringo_shared.a
)

set_target_properties(clingo_shared
                      PROPERTIES LINK_FLAGS )
