if(NOT COVERAGE_MODULE_INCLUDED)
    set(COVERAGE_MODULE_INCLUDED ON)

    if (CMAKE_COMPILER_IS_GNUCXX)
        set (_GCC_COVERAGE ON)
        set (COVERAGE_COMPILE_FLAGS -fprofile-arcs -ftest-coverage)
        set (COVERAGE_LINK_FLAGS --coverage)
        set (COVERAGE_LIBRARIES gcov)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set (_GCC_COVERAGE ON)
        set (COVERAGE_COMPILE_FLAGS -fprofile-arcs -ftest-coverage)
        set (COVERAGE_LINK_FLAGS --coverage)
    endif (CMAKE_COMPILER_IS_GNUCXX)

    add_custom_target(coverage)
    add_custom_target(coverage-html)

    function(coverage_target TARGET)
        if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
            if (_GCC_COVERAGE)
                if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
                    set(GCOVR_GCOV_EXECUTABLE "--gcov-executable=llvm-cov gcov")
                    set(LCOV_GCOV_EXECUTABLE --gcov-tool ${CMAKE_CURRENT_BINARY_DIR}/llvm-gcov.sh)
                    file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/llvm-gcov.sh "#!/bin/bash\nexec llvm-cov gcov \"$@\"")
                    execute_process(COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/llvm-gcov.sh)
                endif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
                add_custom_target(coverage-${TARGET}
                    COMMAND find ${CMAKE_CURRENT_BINARY_DIR} -name "mocs_*.gcda" -o -name "mocs_*.gcno" | xargs rm -f
                    COMMAND gcovr -k --root ${CMAKE_CURRENT_SOURCE_DIR} --object-directory=${CMAKE_CURRENT_BINARY_DIR} ${GCOVR_GCOV_EXECUTABLE}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                )
                add_custom_target(coverage-html-${TARGET}
                    COMMAND find ${CMAKE_CURRENT_BINARY_DIR} -name "mocs_*.gcda" -o -name "mocs_*.gcno" | xargs rm -f
                    COMMAND gcovr -k --root ${CMAKE_CURRENT_SOURCE_DIR} --object-directory=${CMAKE_CURRENT_BINARY_DIR}
                            --html --html-detail --output coverage.html ${GCOVR_GCOV_EXECUTABLE}
                    COMMAND xdg-open coverage.html
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                )

                add_dependencies(coverage coverage-${TARGET})
                add_dependencies(coverage-html coverage-html-${TARGET})

                target_compile_options(${TARGET} PRIVATE ${COVERAGE_COMPILE_FLAGS})
                target_link_options(${TARGET} PRIVATE ${COVERAGE_LINK_FLAGS})
                target_link_libraries(${TARGET} "${COVERAGE_LIBRARIES}")
            endif (_GCC_COVERAGE)
        elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
            message(AUTHOR_WARNING "COVERAGE flag ignored for not debug build type")
        endif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    endfunction()

endif(NOT COVERAGE_MODULE_INCLUDED)
