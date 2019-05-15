# QPagedProxyModel

This Qt proxy model show source model by pages.

## Getting Started

This project use `CMake` as build system and was tested only on different
`Linux` systems. It's compatible with `Qt4` and `Qt5`.

## Prerequisites

* cmake >= 3.1
* Qt4 or Qt5
* googletest (only if you need unit-tests)

## Build

In source tree:

```shell
mkdir build
cd build
cmake ..
cmake --build . 
```

CMake options:

* `QPAGEDPROXYMODEL_BUILD_TESTS` - build unit tests (GTest required)
* `QPAGEDPROXYMODEL_BUILD_DEMO` - build demo gui app
* `QPAGEDPROXYMODEL_COVERAGE` - create target for test code coverage
* `QPAGEDPROXYMODEL_USE_QT4` - use Qt4 even if Qt5 found

## Install

After build in build tree run:

```shell
cmake --build . --target install
```
Files will be install to /usr/local by default, use `CMAKE_INSTALL_PREFIX` to change this.

## Running the tests

If you are run `cmake` with `-DQPAGEDPROXYMODEL_BUILD_TESTS=ON` run `ctest` in build tree.

