cmake_minimum_required (VERSION 3.1)

project (qpagedproxymodel)

SET(_TARGET libqpagedproxymodel_tests)
SET(_SOURCES
    main.cpp
    test_qpagedproxymodel_source_10x10.cpp
    tests_qpagedproxymodel.cpp
)
SET(_LIBRARIES qpagedproxymodel)

IF(NOT GTEST_FOUND)
    find_package(GTest REQUIRED)
ENDIF()

if(USE_Qt EQUAL 5)
    find_package(Qt5Test REQUIRED)
    set(QtTest_LIBRARY Qt5::Test)
else()
    find_package(Qt4 REQUIRED QtTest)
    set(QtTest_LIBRARY Qt4::QtTest)
endif()

add_executable(${_TARGET} ${_SOURCES})
target_link_libraries(${_TARGET} ${_LIBRARIES} ${GTEST_LIBRARY} ${Qt_LIBRARIES} ${QtTest_LIBRARY})

enable_testing()
add_test(${_TARGET} ${_TARGET})
