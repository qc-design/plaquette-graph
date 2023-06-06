#pragma once

#include <algorithm>
#include <complex>
#include <iostream>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include <catch2/catch.hpp>

#include "SparseGraph.hpp"

using namespace std;
using namespace Plaquette;

// template<typename T, typename U>
// bool operator==(const pair<T, U>& lhs, const pair<T, U>& rhs) {
//     return lhs.first == rhs.first && lhs.second == rhs.second;
// }

TEST_CASE("SparseGraph construction", "[SparseGraph]") {
    std::vector<std::pair<size_t, size_t>> edges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}};
    SparseGraph g(4, edges);
    REQUIRE(g.GetNumVertices() == 4);
    REQUIRE(g.GetNumEdges() == 4);
}

TEST_CASE("SparseGraph GetEdgesTouchingVertex", "[SparseGraph]") {
    std::vector<std::pair<size_t, size_t>> edges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}};
    SparseGraph g(4, edges);

    SECTION("Vertex 0") {
        SparseGraphRow row = g.GetEdgesTouchingVertex(0);
        REQUIRE(row.size() == 2);
        REQUIRE(row[0] == 0);
    }

    SECTION("Vertex 1") {
        SparseGraphRow row = g.GetEdgesTouchingVertex(1);
        REQUIRE(row.size() == 2);
        REQUIRE(row[0] == 0);
        REQUIRE(row[1] == 1);
    }

    SECTION("Vertex 2") {
        SparseGraphRow row = g.GetEdgesTouchingVertex(2);
        REQUIRE(row.size() == 2);
        REQUIRE(row[0] == 1);
        REQUIRE(row[1] == 2);
    }

    SECTION("Vertex 3") {
        SparseGraphRow row = g.GetEdgesTouchingVertex(3);
        REQUIRE(row.size() == 2);
        REQUIRE(row[0] == 2);
        REQUIRE(row[1] == 3);
    }
}

TEST_CASE("SparseGraph GetVerticesConnectedByEdge", "[SparseGraph]") {
    std::vector<std::pair<size_t, size_t>> edges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}};
    SparseGraph g(4, edges);

    REQUIRE(g.GetVerticesConnectedByEdge(0) ==
            std::make_pair<size_t, size_t>(0, 1));
    REQUIRE(g.GetVerticesConnectedByEdge(1) ==
            std::make_pair<size_t, size_t>(1, 2));
    REQUIRE(g.GetVerticesConnectedByEdge(2) ==
            std::make_pair<size_t, size_t>(2, 3));
    REQUIRE(g.GetVerticesConnectedByEdge(3) ==
            std::make_pair<size_t, size_t>(3, 0));
}

TEST_CASE("SparseGraph GetEdgeFromVertexPair", "[SparseGraph]") {
    std::vector<std::pair<size_t, size_t>> edges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}};
    SparseGraph g(4, edges);

    REQUIRE(g.GetEdgeFromVertexPair(std::make_pair<size_t, size_t>(0, 1)) == 0);
    REQUIRE(g.GetEdgeFromVertexPair(std::make_pair<size_t, size_t>(1, 2)) == 1);
    REQUIRE(g.GetEdgeFromVertexPair(std::make_pair<size_t, size_t>(2, 3)) == 2);
    REQUIRE(g.GetEdgeFromVertexPair(std::make_pair<size_t, size_t>(3, 0)) == 3);
}
