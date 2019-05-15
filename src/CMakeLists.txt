cmake_minimum_required (VERSION 3.1)

project (qpagedproxymodel)

SET(_TARGET qpagedproxymodel)
SET(_VERSION 0.0.1)
SET(_HEADERS qpagedproxymodel.h)
SET(_PUBLIC_HEADERS ${_HEADERS})
SET(_SOURCES qpagedproxymodel.cpp)


if (UNIX)
    include(coverage.cmake)
endif ()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)

add_library(${_TARGET} SHARED ${_HEADERS} ${_SOURCES})
target_compile_options(${_TARGET} PRIVATE ${COVERAGE_COMPILE_FLAGS})
target_link_libraries(${_TARGET} ${COVERAGE_LINK_FLAGS} ${COVERAGE_LIBRARIES} ${Qt_LIBRARIES})
set_target_properties(${_TARGET} PROPERTIES SOVERSION ${_VERSION})
coverage_target(${_TARGET})

install(TARGETS ${_TARGET}
    LIBRARY DESTINATION lib
)

install(FILES ${_PUBLIC_HEADERS}
    DESTINATION include/qpagedproxymodel
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
