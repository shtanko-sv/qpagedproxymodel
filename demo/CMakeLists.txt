cmake_minimum_required (VERSION 3.0)

project(qpagedproxymodel-src)

SET(_TARGET libqpagedproxymodel_demo)
SET(_HEADERS
    widget.h
)
SET(_SOURCES
    main.cpp
    widget.cpp
)
SET(_LIBRARIES
    qpagedproxymodel
)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)

add_executable(${_TARGET} ${_HEADERS} ${_SOURCES})
target_link_libraries(${_TARGET} ${_LIBRARIES} ${Qt_LIBRARIES})

IF(CMAKE_INSTALL_PREFIX_INITIALIZE_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX /usr/release CACHE PATH "default install path" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZE_TO_DEFAULT)

install(TARGETS ${_TARGET}
    DESTINATION bin/aux/tests
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ WORLD_READ
)
