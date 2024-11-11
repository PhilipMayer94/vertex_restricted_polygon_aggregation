cmake_minimum_required(VERSION 3.10)
project(PolygonAggregation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
find_package(GUROBI REQUIRED)
# project
add_subdirectory(src)


#executable
add_executable(main src/main.cpp
        src/AggregationIlpSolver.cpp
        src/AggregationChordScheme.cpp
        src/basic_geometry.cpp
        src/data_handling.cpp
        src/Empty_Triangles.cpp
        src/Geometry.cpp
        src/TriangulationObjects.cpp
)

configure_file(
        "${PROJECT_SOURCE_DIR}/config.hpp.in"
        "${PROJECT_SOURCE_DIR}/include/config.hpp"
)

target_include_directories(main
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${GUROBI_INCLUDE_DIRS}
        /usr/local/include
)


set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)
install(TARGETS main
        RUNTIME
        DESTINATION bin)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)



target_link_libraries(main
        optimized ${GUROBI_CXX_LIBRARY}
        debug ${GUROBI_CXX_DEBUG_LIBRARY}
        ${GUROBI_LIBRARY}
)




# tests
option(TESTS "Build the tests" ON)
if (TESTS)
    enable_testing()
    add_subdirectory(extlibs/googletest)
    include(GoogleTest)
    add_subdirectory(test)
endif ()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()