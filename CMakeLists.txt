#
# NOTE: determine required cmake here (use 'pip install cmake')
#
cmake_minimum_required(VERSION 3.10...3.21)

#
# NOTE: Import get_version() function from utils, and set up project name
#
include(cmake/utils.cmake)

get_version()
project(
  hackerchat 
  VERSION ${HACKERCHAT_VERSION}
  LANGUAGES CXX
)

#
# NOTE: Include our own cmake scripts, and the CPM package manager
#
include(cmake/CPM.cmake)
include(cmake/print.cmake)
include(cmake/StandardProjectSettings.cmake)

#
# NOTE: Link this 'library' to set the c++ standard / compile-time options requested
#
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)
add_library(project_warnings INTERFACE)

#
# standard compiler warnings
#
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

#
# NOTE: Set our source and include directory variables for use through the build
#
set(HACKERCHAT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(HACKERCHAT_TEST_SOURCE_DIR "${PROJECT_SOURCE_DIR}/tests")
set(HACKERCHAT_EXAMPLES_SOURCE_DIR "${PROJECT_SOURCE_DIR}/examples")
set(HACKERCHAT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

#
# NOTE: Prevent in source builds (can't build in src/ or in project root)
#
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR OR PROJECT_BINARY_DIR STREQUAL HACKERCHAT_SOURCE_DIR)
  print(
    FATAL_ERROR
    "In source builds are not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

#
# NOTE: bring in standard cmake includes
#
include(CMakeDependentOption)
include(GNUInstallDirs)

#
# NOTE: Set C/C++ parameters for all targets in this CMakeLists.txt
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic --std=c++17 -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -fPIC")

#
# NOTE: This is where 3rd party libraries should be installed via the light weight CPM dependancy
# management system. The CPM wiki is here:
# https://github.com/cpm-cmake/CPM.cmake/wiki/More-Snippets
#
CPMAddPackage(
  Name Catch2
  GITHUB_REPOSITORY catchorg/Catch2
  VERSION 2.13.7
)
if(Catch2_ADDED)
  print(STATUS "Catch2 configured correctly")
else()
  print(WARNING "Catch2 was not configured properly")
endif()

CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 8.0.1
)
if(fmt_ADDED)
  print(STATUS "fmt configured correctly")
else()
  print(WARNING "fmt was not configured properly")
endif()

#
# NOTE: add additional project options
#
option(HACKERCHAT_DEBUG "Building Debug version" OFF)
option(HACKERCHAT_TESTING "Enable Test builds" OFF)
option(HACKERCHAT_EXAMPLES "Enable example builds" OFF)
option(ENABLE_PCH "Enable Precompiled Headers" ON)

#
# NOTE: Build defaults to a release optimized build, cmake -DSTARTER_ENABLE_DEBUG=ON will build debug
#
if(HACKERCHAT_DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

#
# NOTE: Output build information
#
print(STATUS "Building ${PROJECT_NAME}-${PROJECT_VERSION} [${CMAKE_BUILD_TYPE}] with ${CMAKE_CXX_FLAGS}")

#
# HACK: glob src files and header files (some people may not like this)
#
file(GLOB_RECURSE HACKERCHAT_GLOBBED_HEADERS CONFIGURE_DEPENDS "${HACKERCHAT_INCLUDE_DIR}/${PROJECT_NAME}/*.hpp")
file(GLOB_RECURSE HACKERCHAT_GLOBBED_SOURCES CONFIGURE_DEPENDS "${HACKERCHAT_SOURCE_DIR}/*.cpp")

#
# NOTE: This sets a global PCH parameter, each project will build its own PCH, which is a good idea
# if any #define's change consider breaking this out per project as necessary
#
if(ENABLE_PCH)
  target_precompile_headers(project_options INTERFACE <vector> <string> <map> <utility>)
endif()

#
# NOTE: Build tests with catch2
#
if(HACKERCHAT_TESTING)
  enable_testing()
  print(STATUS "Tests Enabled")
  add_subdirectory("tests")
endif()

#
# NOTE: Build project examples, mostly useful for shared/static libraries
#
if(HACKERCHAT_EXAMPLES)
  print(STATUS "Examples Enabled")
  # you would want to build each example as one executable, so no glob here
  add_subdirectory("examples")
endif()

print(STATUS "Building main executable")
set(BINARY "${PROJECT_NAME}-client")
add_executable(${BINARY} ${HACKERCHAT_GLOBBED_SOURCES})
# NOTE: Add our external include directory => include/starter
target_include_directories(${BINARY} PUBLIC ${HACKERCHAT_INCLUDE_DIR})
# NOTE: Link 3rd party libraries with the executable here
target_link_libraries(${BINARY} PRIVATE stdc++fs fmt::fmt)
