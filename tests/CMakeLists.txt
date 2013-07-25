project(QStatus)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../../tests/src ${CMAKE_CURRENT_BINARY_DIR}/../../src ${CMAKE_CURRENT_BINARY_DIR}/../../ ${CMAKE_CURRENT_BINARY_DIR}/../../libgit2/include)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/../../libgit2/build)

set(QStatus_SRCS QStatus.cpp)
qt4_automoc(${QStatus_SRCS})
add_executable(QStatus ${QStatus_SRCS})
target_link_libraries(QStatus qgit2 git2 ${QT_QTCORE_LIBRARY})

set(QRev_SRCS QRev.cpp)
qt4_automoc(${QRev_SRCS})
add_executable(QRev ${QRev_SRCS})
target_link_libraries(QRev qgit2 git2 ${QT_QTCORE_LIBRARY})

install(TARGETS QStatus RUNTIME DESTINATION bin)
