#pragma once

#include "SparseGraph.hpp"

namespace Plaquette {
/**
 * @class DecodingGraph
 *
 * @brief A class that represents a decoding graph.
 *
 * This class represents a decoding graph, which is a type of undirected graph
 * commonly used in decoding algorithms. The decoding graph is represented as a
 * SparseGraph object, with an additional vector of boolean values indicating
 * which vertices are on the boundary of the graph.
 *
 */
class DecodingGraph : public SparseGraph {

  private:
    std::vector<bool>
        vertex_boundary_type_; ///< A vector of boolean values indicating which
                               ///< vertices are on the boundary of the graph.

    std::vector<size_t>
        local_edge_strides_; ///< A vector of edge strides for each vertex.
    std::vector<size_t> local_to_global_edge_map_;
    std::vector<int> global_to_local_edge_map_;
    size_t num_local_edges_;

  public:
    DecodingGraph() = default; ///< Default constructor.
    /**
     * @brief Constructor for the DecodingGraph class.
     *
     * This constructor initializes a DecodingGraph object with the specified
     * number of vertices, set of edges, and vector of boolean values indicating
     * which vertices are on the boundary of the graph. The SparseGraph
     * constructor is used to initialize the base SparseGraph object with the
     * specified number of vertices and set of edges.
     *
     * @param num_vertices The number of vertices in the decoding graph.
     * @param edges A vector of pairs of vertex identifiers representing the
     * edges in the decoding graph.
     * @param vertex_boundary_type A vector of boolean values indicating which
     * vertices are on the boundary of the decoding graph.
     */
    DecodingGraph(size_t num_vertices,
                  const std::vector<std::pair<size_t, size_t>> &edges,
                  const std::vector<bool> &vertex_boundary_type)
        : SparseGraph(num_vertices, edges) {
        vertex_boundary_type_ = vertex_boundary_type;

        num_local_edges_ = 0;
        for (size_t i = 0; i < num_vertices; i++) {
            local_edge_strides_.push_back(num_local_edges_);
            const auto &edges = GetEdgesTouchingVertex(i);
            num_local_edges_ += edges.size();
        }

        std::vector<bool> local_edge_visited(GetNumEdges() * 2, false);
        global_to_local_edge_map_.resize(GetNumEdges() * 2, -1);
        local_to_global_edge_map_.resize(num_local_edges_);

        for (size_t i = 0; i < num_vertices; i++) {
            const auto &edges = GetEdgesTouchingVertex(i);
            size_t stride = local_edge_strides_[i];
            for (size_t e = 0; e < edges.size(); e++) {
                local_to_global_edge_map_[stride + e] = edges[e];
                if (local_edge_visited[2 * edges[e] + 0] == false) {
                    global_to_local_edge_map_[2 * edges[e] + 0] = stride + e;
                    local_edge_visited[2 * edges[e] + 0] = true;
                } else {
                    global_to_local_edge_map_[2 * edges[e] + 1] = stride + e;
                    local_edge_visited[2 * edges[e] + 1] = true;
                }
            }
        }
    }

    /**
     * @brief Check if a vertex is on the boundary of the decoding graph.
     *
     * This function checks if the specified vertex is on the boundary of the
     * decoding graph, as indicated by the corresponding boolean value in the
     * vertex_boundary_type_ vector.
     *
     * @param vertex_id The identifier of the vertex to check.
     * @return true if the vertex is on the boundary of the decoding graph,
     * false otherwise.
     */
    inline bool IsVertexOnBoundary(size_t vertex_id) const {
        return vertex_boundary_type_[vertex_id];
    }

    /**
     * @brief Returns the number of local edges.
     *
     * @return The number of local edges.
     */
    size_t GetNumLocalEdges() const { return num_local_edges_; }

    /**
     * @brief Returns the local edge stride for a given vertex ID.
     *
     * @param vertex_id The ID of the vertex to get the local edge stride for.
     * @return The local edge stride for the given vertex ID.
     */
    inline size_t GetLocalEdgeStride(size_t vertex_id) const {
        return local_edge_strides_[vertex_id];
    }

    /**
     * @brief Returns the global edge ID corresponding to a given local edge ID.
     *
     * @param local_edge_id The ID of the local edge.
     * @return The corresponding global edge ID.
     */
    inline size_t GetGlobalEdgeFromLocalEdge(size_t local_edge_id) const {
        return local_to_global_edge_map_[local_edge_id];
    }

    /**
       @brief Returns the local edge ID corresponding to a given global edge ID
       and left or right ID.
       @param global_edge_id The ID of the global edge.
       @param left_or_right_id The ID indicating whether it is the left or right
       endpoint of the edge.
       @return The corresponding local edge ID.
    */
    inline size_t GetLocalEdgeFromGlobalEdge(size_t global_edge_id,
                                             size_t left_or_right_id) const {
        return global_to_local_edge_map_[2 * global_edge_id + left_or_right_id];
    }
};
}; // namespace Plaquette
