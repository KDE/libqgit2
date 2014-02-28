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

# CMake build script for the libgit2 project
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

set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(WIN32) # Dll's into bin
    set(LIBRARY_OUTPUT_PATH     ${CMAKE_BINARY_DIR}/bin)
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
  set(QT_MIN_VERSION "4.6.0")
  find_package(Qt4 REQUIRED)
endif()


FILE(STRINGS "qgit2.h" QGIT2_HEADER REGEX "^#define LIBQGIT2_VERSION \"[^\"]*\"$")

STRING(REGEX REPLACE "^.*LIBQGIT2_VERSION \"([0-9]+).*$" "\\1" LIBQGIT2_VERSION_MAJOR "${QGIT2_HEADER}")
STRING(REGEX REPLACE "^.*LIBQGIT2_VERSION \"[0-9]+\\.([0-9]+).*$" "\\1" LIBQGIT2_VERSION_MINOR  "${QGIT2_HEADER}")
STRING(REGEX REPLACE "^.*LIBQGIT2_VERSION \"[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" LIBQGIT2_VERSION_REV "${QGIT2_HEADER}")
SET(LIBQGIT2_VERSION_STRING "${LIBQGIT2_VERSION_MAJOR}.${LIBQGIT2_VERSION_MINOR}.${LIBQGIT2_VERSION_REV}")

# Installation paths
SET(INSTALL_BIN bin CACHE PATH "Where to install binaries to.")
SET(INSTALL_LIB lib CACHE PATH "Where to install libraries to.")
SET(INSTALL_INC include CACHE PATH "Where to install headers to.")

# Build options
OPTION (BUILD_SHARED_LIBS "Build Shared Library (OFF for Static)" ON)
OPTION (BUILD_TESTS "Build Tests" ON)

#Project options, to provide a selective build process
#Note that some of these are mutually exclusive or just
#do not make a whole lot of sense combined.
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

if(CMAKE_INSTALL_PREFIX STREQUAL "/usr")
    option(INSTALL_CMAKE_FILES "Install the *.cmake files into the CMake root" ON)
else()
    option(INSTALL_CMAKE_FILES "Install the *.cmake files into the CMake root" OFF)
endif()

set(LIBQGIT2_BUILD_ALL TRUE
    CACHE INTERNAL "Build all of LIBQGIT2"
)

if(NOT MSVC)
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -O2 -g")
    SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2")
    SET(CMAKE_CXX_FLAGS_DEBUG  "-Wall -g3 -ggdb -O0" CACHE STRING "Debug options." FORCE)
endif()

# Build Release by default
IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
ENDIF ()

option(USE_LOCAL_LIBGIT2 "Use libgit2 compiled in ${CMAKE_SOURCE_DIR}/libgit2" OFF)
option(LIBGIT2_TESTS     "Build libgit2 tests" OFF)

if(USE_LOCAL_LIBGIT2)
    message("Using local libgit2 in ${CMAKE_SOURCE_DIR}/libgit2")
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
    add_subdirectory(libgit2)
    set(GIT2_INCLUDEDIR ${CMAKE_SOURCE_DIR}/libgit2/include)
    set(GIT2_LIBRARIES git2)
    set(GIT2_FOUND 1)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GIT2 REQUIRED libgit2)
endif()

if(NOT GIT2_FOUND)
    message(STATUS)
    message(STATUS "Error: No libgit2 found on your system.")
    message(STATUS "       Define -DUSE_LOCAL_LIBGIT2=1 to embed libgit2 into qgit2")
    message(STATUS "       (submodules must be up-to-date).")
    message(STATUS)
    message(FATAL_ERROR "Exiting now")
endif()

include_directories(src ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${GIT2_INCLUDEDIR} ${Qt5Core_INCLUDES})

if(Qt5Core_FOUND)
    include_directories(${Qt5Core_INCLUDES})
else()
    include_directories(${QT_INCLUDES})
endif()

# Collect files
FILE(GLOB SRC src/*.cpp)
FILE(GLOB QGIT2_HEADERS src/*.h)

# MOC
if(Qt5Core_FOUND)
else()
    QT4_WRAP_CPP(MOC_SRC src/qgitindexmodel.h src/qgitrepository.h)
endif()

message(STATUS)
message(STATUS "========== LIBQGIT2 Build Information ==========")
message(STATUS "Build Version: ${LIBQGIT2_VERSION_STRING} (${LIBQGIT2_VERSION_NAME})")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Install the *.cmake files into CMake root (INSTALL_CMAKE_FILES): ${INSTALL_CMAKE_FILES}")
message(STATUS)
message(STATUS "To change any of these options, override them using -D{OPTION_NAME} on the commandline.")
message(STATUS "To build and install LIBQGIT2, run \"make\" and \"make install\"")
message(STATUS)

# Compile and link libqgit2
add_definitions(-DMAKE_LIBQGIT2_LIB)
add_library(qgit2 ${SRC} ${MOC_SRC} ${QGIT2_HEADERS})
TARGET_LINK_LIBRARIES(qgit2 ${GIT2_LIBRARIES} ${QT_QTCORE_LIBRARY})
SET_TARGET_PROPERTIES(qgit2 PROPERTIES VERSION ${LIBQGIT2_VERSION_STRING})
SET_TARGET_PROPERTIES(qgit2 PROPERTIES SOVERSION ${LIBQGIT2_VERSION_MAJOR})

if(Qt5Core_FOUND)
    qt5_use_modules(qgit2 Core)
endif()

# Install
INSTALL(TARGETS qgit2
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_LIB}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

INSTALL(FILES
    ${QGIT2_HEADERS}
    libqgit2_export.h
    DESTINATION ${INSTALL_INC}/qgit2 COMPONENT Devel
)
INSTALL(FILES qgit2.h DESTINATION ${INSTALL_INC} COMPONENT Devel)


if(INSTALL_CMAKE_FILES)
    install(
        FILES
        DESTINATION
        ${CMAKE_ROOT}/Modules
        OPTIONAL
    )
else()
    install(
        FILES
        DESTINATION
        ${SHARE_INSTALL_DIR}/mula/cmake
        OPTIONAL
    )
endif()

#Packaging options
set(CPACK_GENERATOR "RPM;TGZ")
set(CPACK_PACKAGE_NAME "LIBQGIT2")
set(CPACK_PACKAGE_FILE_NAME "LIBQGIT2-${LIBQGIT2_VERSION_STRING}")
set(CPACK_PACKAGE_VERSION "${LIBQGIT2_VERSION_MAJOR}.${LIBQGIT2_VERSION_MINOR}.${LIBQGIT2_VERSION_REV}")
set(CPACK_PACKAGE_CONTACT "Laszlo Papp <djszapi@archlinux.us>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libgit2 bindings for Qt")
include(InstallRequiredSystemLibraries)
include(CPack)


enable_testing()
add_subdirectory(tests)

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)
