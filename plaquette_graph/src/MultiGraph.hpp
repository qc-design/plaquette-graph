#pragma once

#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Plaquette {

class MultiGraph {
  public:
    MultiGraph(const std::vector<std::pair<size_t, size_t>> &edges,
               const std::vector<size_t> &weights)
        : num_vertices_(0), num_edges_(edges.size()) {
        // Determine number of vertices
        for (const auto &edge : edges) {
            num_vertices_ =
                std::max(num_vertices_, std::max(edge.first, edge.second) + 1);
        }

        // Initialize edge weight map
        edge_to_weight_map_.resize(num_edges_);
        for (size_t i = 0; i < num_edges_; i++) {
            edge_to_weight_map_[i] = weights[i];
        }

        // Initialize edge connectivity and adjacency lists
        vertex_to_edge_map_.resize(num_vertices_);
        vertex_adjacency_lists_.resize(num_vertices_);
        for (size_t i = 0; i < num_edges_; i++) {
            const auto &edge = edges[i];
            vertex_to_edge_map_[edge.first].push_back(i);
            vertex_to_edge_map_[edge.second].push_back(i);

            vertex_adjacency_lists_[edge.first].push_back(edge.second);
            vertex_adjacency_lists_[edge.second].push_back(edge.first);
        }

        edges_ = edges;
    }

    std::vector<size_t> GetEdgesTouchingVertex(size_t vertex) const {
        return vertex_to_edge_map_[vertex];
    }

    std::vector<size_t> GetVerticesTouchingVertex(size_t vertex) const {
        if (vertex >= num_vertices_) {
            return {};
        }
        return vertex_adjacency_lists_[vertex];
    }

    size_t GetWeight(size_t edge_index) const {
        return edge_to_weight_map_[edge_index];
    }

    size_t GetEdgeConnectingVertices(size_t vertex1, size_t vertex2) const {

        const auto &vem = vertex_to_edge_map_[vertex1];
        for (const auto &edge_index : vem) {
            const auto &edge = edges_[edge_index];
            if ((edge.first == vertex1 && edge.second == vertex2) ||
                (edge.first == vertex2 && edge.second == vertex1)) {
                return edge_index;
            }
        }
        return num_edges_;
    }

    std::vector<size_t> GetEdgesTouchingEdge(size_t edge) const {
        std::vector<size_t> edges;

        if (edge < num_edges_) {
            size_t u = edges_[edge].first;
            size_t v = edges_[edge].second;

            for (size_t i = 0; i < num_edges_; ++i) {
                if (i != edge) {
                    if (edges_[i].first == u || edges_[i].second == v ||
                        edges_[i].first == v || edges_[i].second == u) {
                        edges.push_back(i);
                    }
                }
            }
        }

        return edges;
    }

    size_t GetNumVertices() const { return num_vertices_; }

    size_t GetNumEdges() const { return num_edges_; }

  private:
    size_t num_vertices_;
    size_t num_edges_;
    std::vector<std::pair<size_t, size_t>> edges_;
    std::vector<size_t> edge_to_weight_map_;
    std::vector<std::vector<size_t>> vertex_to_edge_map_;
    std::vector<std::vector<size_t>> vertex_adjacency_lists_;
};

}; // namespace Plaquette
