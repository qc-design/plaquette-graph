#pragma once

#include <algorithm>
#include <complex>
#include <iostream>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include <catch2/catch.hpp>

#include "MultiGraph.hpp"
TEST_CASE("GetEdgesTouchingVertex returns correct edges", "[MultiGraph]") {
    std::vector<std::pair<size_t, size_t>> edges{{0, 1}, {0, 2}, {1, 2}};
    std::vector<size_t> weights{1, 2, 3};
    MultiGraph g(edges, weights);

    REQUIRE(g.GetEdgesTouchingVertex(0) == std::vector<size_t>{0, 1});
    REQUIRE(g.GetEdgesTouchingVertex(1) == std::vector<size_t>{0, 2});
    REQUIRE(g.GetEdgesTouchingVertex(2) == std::vector<size_t>{1, 2});
}

TEST_CASE("GetVerticesTouchingVertex returns correct vertices",
          "[MultiGraph]") {
    std::vector<std::pair<size_t, size_t>> edges{{0, 1}, {0, 2}, {1, 2}};
    std::vector<size_t> weights{1, 2, 3};
    MultiGraph g(edges, weights);

    REQUIRE(g.GetVerticesTouchingVertex(0) == std::vector<size_t>{1, 2});
    REQUIRE(g.GetVerticesTouchingVertex(1) == std::vector<size_t>{0, 2});
    REQUIRE(g.GetVerticesTouchingVertex(2) == std::vector<size_t>{0, 1});
}

TEST_CASE("GetWeight returns correct weight", "[MultiGraph]") {
    std::vector<std::pair<size_t, size_t>> edges{{0, 1}, {0, 2}, {1, 2}};
    std::vector<size_t> weights{1, 2, 3};
    MultiGraph g(edges, weights);

    REQUIRE(g.GetWeight(0) == 1);
    REQUIRE(g.GetWeight(1) == 2);
    REQUIRE(g.GetWeight(2) == 3);
}

TEST_CASE("GetEdgeConnectingVertices returns correct edge index",
          "[MultiGraph]") {
    std::vector<std::pair<size_t, size_t>> edges{{0, 1}, {0, 2}, {1, 2}};
    std::vector<size_t> weights{1, 2, 3};
    MultiGraph g(edges, weights);

    REQUIRE(g.GetEdgeConnectingVertices(0, 1) == 0);
    REQUIRE(g.GetEdgeConnectingVertices(0, 2) == 1);
    REQUIRE(g.GetEdgeConnectingVertices(1, 2) == 2);
}

TEST_CASE("GetEdgesTouchingEdge returns correct edges", "[MultiGraph]") {
    std::vector<std::pair<size_t, size_t>> edges{{0, 1}, {0, 2}, {1, 2}};
    std::vector<size_t> weights{1, 2, 3};
    MultiGraph g(edges, weights);

    REQUIRE(g.GetEdgesTouchingEdge(0) == std::vector<size_t>{1, 2});
    REQUIRE(g.GetEdgesTouchingEdge(1) == std::vector<size_t>{0, 2});
    REQUIRE(g.GetEdgesTouchingEdge(2) == std::vector<size_t>{0, 1});
}

TEST_CASE(
    "GetVerticesTouchingVertex returns empty vector for unconnected vertex",
    "[MultiGraph]") {
    std::vector<std::pair<size_t, size_t>> edges{{0, 1}, {0, 2}, {1, 2}};
    std::vector<size_t> weights{1, 2, 3};
    MultiGraph g(edges, weights);

    REQUIRE(g.GetVerticesTouchingVertex(3) == std::vector<size_t>{});
}
