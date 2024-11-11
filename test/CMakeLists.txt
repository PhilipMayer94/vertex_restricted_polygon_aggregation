macro(package_add_test TESTNAME)
    # create an executable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main function to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_include_directories(${TESTNAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${TESTNAME} gtest gmock gtest_main)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
            # set a working directory so your project root so that you can find test data via paths relative to the project root
            WORKING_DIRECTORY ..
            PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
            )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()


package_add_test(basic basic.cpp
 ../src/Empty_Triangles.cpp
 ../src/basic_geometry.cpp
 ../src/data_handling.cpp
        )

        package_add_test(correctness_test correctness_test.cpp
 ../src/Empty_Triangles.cpp
 ../src/basic_geometry.cpp
 ../src/data_handling.cpp
        )
        package_add_test(num_triangles_test num_triangles_test.cpp
 ../src/Empty_Triangles.cpp
 ../src/basic_geometry.cpp
 ../src/data_handling.cpp
        )

        package_add_test(triangulations_test Triangulations_test.cpp
        ../src/Empty_Triangles.cpp
        ../src/Geometry.cpp
        ../src/basic_geometry.cpp
        ../src/data_handling.cpp
        ../src/TriangulationObjects.cpp
        )


package_add_test(intersection_test intersection_test.cpp
        ../src/Geometry.cpp
)


package_add_test(ilp_tests ILPTest.cpp
        ../src/Empty_Triangles.cpp
        ../src/Geometry.cpp
        ../src/basic_geometry.cpp
        ../src/data_handling.cpp
        ../src/TriangulationObjects.cpp
        ../src/AggregationIlpSolver.cpp
)



target_link_libraries(ilp_tests
        optimized ${GUROBI_CXX_LIBRARY}
        debug ${GUROBI_CXX_DEBUG_LIBRARY}
        ${GUROBI_LIBRARY}
)

target_include_directories(ilp_tests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${GUROBI_INCLUDE_DIRS}
        /usr/local/include
)

package_add_test(chord_test ChordTest.cpp
        ../src/Empty_Triangles.cpp
        ../src/Geometry.cpp
        ../src/basic_geometry.cpp
        ../src/data_handling.cpp
        ../src/TriangulationObjects.cpp
        ../src/AggregationIlpSolver.cpp
        ../src/AggregationChordScheme.cpp
)


target_link_libraries(chord_test
        optimized ${GUROBI_CXX_LIBRARY}
        debug ${GUROBI_CXX_DEBUG_LIBRARY}
        ${GUROBI_LIBRARY}
)

target_include_directories(chord_test
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${GUROBI_INCLUDE_DIRS}
        /usr/local/include
)

