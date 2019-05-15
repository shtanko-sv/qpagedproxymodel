cmake_minimum_required (VERSION 3.1)
project (qpagedproxymodel)

option(QPAGEDPROXYMODEL_BUILD_TESTS "Enable building tests with gtest" OFF)
option(QPAGEDPROXYMODEL_BUILD_DEMO "Enable building gui demo app" OFF)
option(QPAGEDPROXYMODEL_COVERAGE "Add flags for source coverage report" OFF)
option(QPAGEDPROXYMODEL_USE_QT4 "Force use Qt4" OFF)

if (NOT QPAGEDPROXYMODEL_QT4)
    find_package(Qt5Widgets)
    find_package(Qt5Core)
endif()

if (Qt5Widgets_FOUND AND Qt5Core_FOUND)
    set(Qt_LIBRARIES Qt5::Widgets Qt5::Core)
    set(USE_Qt 5)
else ()
    find_package(Qt4 REQUIRED QtGui QtCore)
    set(Qt_LIBRARIES Qt4::QtGui Qt4::QtCore)
    set(USE_Qt 4)
endif ()

add_subdirectory (src)

if (QPAGEDPROXYMODEL_BUILD_TESTS)
    add_subdirectory (tests)
    enable_testing()
endif ()

if (QPAGEDPROXYMODEL_BUILD_DEMO)
    add_subdirectory(demo)
endif ()
