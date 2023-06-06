#pragma once

#include <algorithm>
#include <complex>
#include <iostream>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include <catch2/catch.hpp>

#include "DecodingGraph.hpp"

using namespace Plaquette;

TEST_CASE("DecodingGraph constructor initializes base class and "
          "vertex_boundary_type_",
          "[DecodingGraph]") {
    size_t num_vertices = 4;
    std::vector<std::pair<size_t, size_t>> edges = {{0, 1}, {1, 2}, {2, 3}};
    std::vector<bool> vertex_boundary_type = {true, false, false, true};

    DecodingGraph graph(num_vertices, edges, vertex_boundary_type);

    SECTION("Base class is initialized correctly") {
        REQUIRE(graph.GetNumVertices() == num_vertices);
        REQUIRE(graph.GetNumEdges() == edges.size());
        REQUIRE(graph.GetVerticesConnectedByEdge(0) == edges[0]);
    }

    SECTION("vertex_boundary_type_ is initialized correctly") {
        REQUIRE(graph.IsVertexOnBoundary(0) == true);
        REQUIRE(graph.IsVertexOnBoundary(1) == false);
        REQUIRE(graph.IsVertexOnBoundary(2) == false);
        REQUIRE(graph.IsVertexOnBoundary(3) == true);
    }
}

TEST_CASE("IsVertexOnBoundary returns correct value", "[DecodingGraph]") {
    size_t num_vertices = 4;
    std::vector<std::pair<size_t, size_t>> edges = {{0, 1}, {1, 2}, {2, 3}};
    std::vector<bool> vertex_boundary_type = {true, false, false, true};

    DecodingGraph graph(num_vertices, edges, vertex_boundary_type);

    REQUIRE(graph.IsVertexOnBoundary(0) == true);
    REQUIRE(graph.IsVertexOnBoundary(1) == false);
    REQUIRE(graph.IsVertexOnBoundary(2) == false);
    REQUIRE(graph.IsVertexOnBoundary(3) == true);
}
