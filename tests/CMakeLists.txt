
set(testdir ${CMAKE_BINARY_DIR}/TestDir)
file(MAKE_DIRECTORY ${testdir})
add_definitions(-DTEST_DIR=${testdir})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)

macro(addTest name)
    set(_source ${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp)
    set(_moced ${CMAKE_CURRENT_BINARY_DIR}/${name}.moc)
    qt4_generate_moc(${_source} ${_moced})
    set_source_files_properties(${_source} PROPERTIES OBJECT_DEPENDS "${_moced}")
    set(testname test${name})
    add_executable(${testname} ${_source})
    target_link_libraries(test${name} qgit2 git2 ${QT_QTCORE_LIBRARY} ${QT_QTTEST_LIBRARY})
    add_test(NAME ${testname} COMMAND ${testname})
endmacro()


addTest(Init)
addTest(Revision)
addTest(StatusOptions)
