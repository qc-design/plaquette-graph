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

TEST_CASE("Check GetLocalEdgeStride") {
    std::vector<bool> vertex_boundary_type = {false, false, false};
    DecodingGraph graph(3, {{0, 1}, {1, 2}, {2, 0}}, vertex_boundary_type);

    REQUIRE(graph.GetLocalEdgeStride(0) == 0);
    REQUIRE(graph.GetLocalEdgeStride(1) == 2);
    REQUIRE(graph.GetLocalEdgeStride(2) == 4);
}

TEST_CASE("Check GetGlobalEdgeFromLocalEdge") {
    std::vector<bool> vertex_boundary_type = {false, false, false};
    DecodingGraph graph(3, {{0, 1}, {1, 2}, {2, 0}}, vertex_boundary_type);

    REQUIRE(graph.GetGlobalEdgeFromLocalEdge(0) == 0);
    REQUIRE(graph.GetGlobalEdgeFromLocalEdge(1) == 2);
    REQUIRE(graph.GetGlobalEdgeFromLocalEdge(2) == 0);
    REQUIRE(graph.GetGlobalEdgeFromLocalEdge(3) == 1);
    REQUIRE(graph.GetGlobalEdgeFromLocalEdge(4) == 1);
    REQUIRE(graph.GetGlobalEdgeFromLocalEdge(5) == 2);
}

TEST_CASE("Check GetLocalEdgeFromGlobalEdge") {
    std::vector<bool> vertex_boundary_type = {false, false, false};
    DecodingGraph graph(3, {{0, 1}, {1, 2}, {2, 0}}, vertex_boundary_type);

    REQUIRE(graph.GetLocalEdgeFromGlobalEdge(0, 0) == 0);
    REQUIRE(graph.GetLocalEdgeFromGlobalEdge(0, 1) == 2);
    REQUIRE(graph.GetLocalEdgeFromGlobalEdge(1, 0) == 3);
    REQUIRE(graph.GetLocalEdgeFromGlobalEdge(1, 1) == 4);
    REQUIRE(graph.GetLocalEdgeFromGlobalEdge(2, 0) == 1);
    REQUIRE(graph.GetLocalEdgeFromGlobalEdge(2, 1) == 5);
}
