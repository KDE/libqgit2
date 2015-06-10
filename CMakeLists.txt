#############################################################################
# This file is part of the libqgit2 library
# Copyright (C) 2011 Laszlo Papp
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

# CMake build script for the libqgit2 project
#
# Building (out of source build):
# > mkdir build && cd build
# > cmake .. [-DSETTINGS=VALUE]
# > cmake --build .
#
# Testing:
# > ctest -V
#
# Install:
# > cmake --build . --target install

cmake_minimum_required(VERSION 2.8)
project(libqgit2)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(WIN32) # Dll's into bin
    set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
endif()

option(QT4_BUILD "Force building with Qt4 even if Qt5 is found")
if (NOT QT4_BUILD)
    find_package(Qt5Core QUIET)
endif()

if (Qt5Core_FOUND)
    message(STATUS "Building with Qt5 support")
    find_package(Qt5Core REQUIRED Core Network)
    add_definitions(${Qt5Core_DEFINITIONS})
    set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
    set(CMAKE_AUTOMOC ON)
else()
    message(STATUS "Building with Qt4 support")
    set(QT_MIN_VERSION "4.6.0")
    find_package(Qt4 REQUIRED)
endif()


file(STRINGS "qgit2.h" QGIT2_HEADER REGEX "^#define LIBQGIT2_VERSION \"[^\"]*\"$")
string(REGEX REPLACE "^.*LIBQGIT2_VERSION \"([0-9]+).*$" "\\1" LIBQGIT2_VERSION_MAJOR "${QGIT2_HEADER}")
string(REGEX REPLACE "^.*LIBQGIT2_VERSION \"[0-9]+\\.([0-9]+).*$" "\\1" LIBQGIT2_VERSION_MINOR  "${QGIT2_HEADER}")
string(REGEX REPLACE "^.*LIBQGIT2_VERSION \"[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" LIBQGIT2_VERSION_REV "${QGIT2_HEADER}")
set(LIBQGIT2_VERSION_STRING "${LIBQGIT2_VERSION_MAJOR}.${LIBQGIT2_VERSION_MINOR}.${LIBQGIT2_VERSION_REV}")

file(STRINGS "qgit2.h" QGIT2_HEADER_SOVERSION REGEX "^#define LIBQGIT2_SOVERSION [0-9]+$")
string(REGEX REPLACE "^.*LIBQGIT2_SOVERSION ([0-9]+)$" "\\1" LIBQGIT2_SOVERSION "${QGIT2_HEADER_SOVERSION}")

# Installation paths
set(INSTALL_BIN bin CACHE PATH "Where to install binaries to.")
set(INSTALL_LIB lib CACHE PATH "Where to install libraries to.")
set(INSTALL_INC include CACHE PATH "Where to install headers to.")

# Build options
option(BUILD_TESTS "Build Tests" ON)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -O2 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2")
    set(CMAKE_CXX_FLAGS_DEBUG  "-Wall -g3 -ggdb -O0" CACHE STRING "Debug options." FORCE)
endif()

# Build Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

set(LIBGIT2_SRC_DIR "" CACHE PATH "Build libgit2 source from this directory.")
option(LIBGIT2_TESTS "Build libgit2 tests" OFF)

if(NOT LIBGIT2_SRC_DIR STREQUAL "")
    message(STATUS "Using libgit2 source from ${LIBGIT2_SRC_DIR}")
    if(NOT LIBGIT2_TESTS)
        set(BUILD_CLAR OFF CACHE STRING "libgit2 tests" FORCE)
    endif()
    if(MSVC)
        set(STDCALL    OFF CACHE STRING "no stdcall" FORCE)
        set(STATIC_CRT OFF CACHE STRING "shared runtime" FORCE)
        # QSharedPointer on opaque libgit structures
        set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   /wd4150")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /wd4150")
    endif()
    add_subdirectory(${LIBGIT2_SRC_DIR} libgit2)
    set(LIBGIT2_INCLUDE_DIR ${LIBGIT2_SRC_DIR}/include)
    set(LIBGIT2_LIBRARIES git2)
    set(LIBGIT2_FOUND 1)
else()
    find_package(Libgit2 REQUIRED)
endif()

if(NOT LIBGIT2_FOUND)
    message(FATAL_ERROR "No libgit2 found!
Install the libgit2 development files to your system.
Alternatively define LIBGIT2_SRC_DIR to point to a libgit2 source directory.")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${LIBGIT2_INCLUDE_DIR})

if(Qt5Core_FOUND)
    include_directories(${Qt5Core_INCLUDES})
else()
    include_directories(${QT_INCLUDES})
endif()

# Collect files
file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB QGIT2_HEADERS src/qgit*.h src/libqgit2_config.h)
file(GLOB_RECURSE QGIT2_PRIVATE_HEADERS src/*.h)
list(REMOVE_ITEM QGIT2_PRIVATE_HEADERS ${QGIT2_HEADERS})

# MOC
if(NOT Qt5Core_FOUND)
    QT4_WRAP_CPP(MOC_SRC src/qgitindexmodel.h src/qgitrepository.h src/qgitremote.h)
endif()

message(STATUS)
message(STATUS "========== LIBQGIT2 Build Information ==========")
message(STATUS "Build Version: ${LIBQGIT2_VERSION_STRING}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS)
message(STATUS "To change any of these options, override them using -D{OPTION_NAME} on the commandline.")
message(STATUS "To build and install LIBQGIT2, run \"make\" and \"make install\"")
message(STATUS)

# Compile and link libqgit2
add_definitions(-DMAKE_LIBQGIT2_LIB)
add_library(qgit2 ${SRC} ${MOC_SRC} ${QGIT2_HEADERS} ${QGIT2_PRIVATE_HEADERS})
target_link_libraries(qgit2 ${LIBGIT2_LIBRARIES} ${QT_QTCORE_LIBRARY})
set_target_properties(qgit2 PROPERTIES VERSION ${LIBQGIT2_VERSION_STRING})
set_target_properties(qgit2 PROPERTIES SOVERSION ${LIBQGIT2_SOVERSION})

if(Qt5Core_FOUND)
    qt5_use_modules(qgit2 Core)
endif()

# Install
install(TARGETS qgit2
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_LIB}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

install(FILES ${QGIT2_HEADERS} DESTINATION ${INSTALL_INC}/qgit2 COMPONENT Devel)
install(FILES qgit2.h DESTINATION ${INSTALL_INC} COMPONENT Devel)

#Packaging options
set(CPACK_GENERATOR "RPM;TGZ")
set(CPACK_PACKAGE_NAME "LIBQGIT2")
set(CPACK_PACKAGE_FILE_NAME "LIBQGIT2-${LIBQGIT2_VERSION_STRING}")
set(CPACK_PACKAGE_VERSION "${LIBQGIT2_VERSION_MAJOR}.${LIBQGIT2_VERSION_MINOR}.${LIBQGIT2_VERSION_REV}")
set(CPACK_PACKAGE_CONTACT "Laszlo Papp <djszapi@archlinux.us>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libgit2 bindings for Qt")
include(InstallRequiredSystemLibraries)
include(CPack)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)
