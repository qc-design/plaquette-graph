project(plaquette_graph_components LANGUAGES CXX)

if (PLAQUETTE_GRAPH_BUILD_TESTS)
    add_subdirectory("tests" "tests")
endif()
