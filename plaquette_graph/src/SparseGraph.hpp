#pragma once

#include <cassert>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "Utils.hpp"

namespace Plaquette {

/**
 * @brief A lightweight view into a range of elements in a vector.
 *
 * The `SparseGraphRow` class provides a lightweight view into a range of
 * elements in a vector. The class is used to represent a row of non-zero
 * elements in a matrix stored as a compressed sparse row (CSR) format. A
 * `SparseGraphRow` object consists of a reference to the vector of non-zero
 * elements, along with start and end indices that delimit the range of elements
 * in the row.
 *
 * The `SparseGraphRow` class provides two public member functions: `size()` and
 * `operator[]()`. The `size()` function returns the number of non-zero
 * elements in the row, and the `operator[]()` function returns the value
 * at a specific index in the row.
 */
class SparseGraphRow {
  public:
    // Constructor
    SparseGraphRow(const std::vector<size_t> &row, size_t start, size_t end)
        : row_(row), start_(start), end_(end) {}

    // Get the number of non-zero elements in the row
    size_t size() const { return end_ - start_; }

    // Get the value at a specific index in the row
    size_t operator[](int index) const { return row_[start_ + index]; }

  private:
    // Reference to the column index and ID vectors
    const std::vector<size_t> &row_;

    // Start and end indices of the row
    size_t start_;
    size_t end_;
};

/**
 * @class SparseGraph
 * @brief A class representing an undirected sparse graph.
 *
 * This class stores information about the connections between vertices and
 * edges in an undirected graph. The graph is represented by two adjacency
 * matrices: one for vertex-vertex connections and one for edge-edge
 * connections. The class also includes a lookup list for edge to vertices
 * connections. Once an object of this class has been created, it is immutable.
 * The adjacency matrices are stored in CSR format, because usually the graphs
 * (e.g. decoding graphs) are sparse.
 *
 */
class SparseGraph {

  private:
    size_t num_vertices_;

    /** @brief adjacency matrix for vertex-vertex connections. */
    std::vector<size_t> v_to_v_row_ptr_;
    std::vector<size_t> v_to_v_edges_;
    std::vector<size_t> v_to_v_col_;

    /** @brief adjacency matrix for edge-edge connections. */
    std::vector<size_t> e_to_e_row_ptr_;
    std::vector<size_t> e_to_e_vertices_;
    std::vector<size_t> e_to_e_col_;

    /** @brief edge to vertices lookup list */
    std::vector<std::pair<size_t, size_t>> e_to_v_;

  public:
    SparseGraph() = default;
    SparseGraph(size_t num_vertices,
                const std::vector<std::pair<size_t, size_t>> &edges) {
        num_vertices_ = num_vertices;
        ConstructEdgeToVertex_(edges);
        ConstructVertexToVertexMatrix_(e_to_v_);
        ConstructEdgeToEdgeMatrix_();
    }

    /**
     * @brief Construct the edge to vertex lookup list.
     *
     * @param edges A vector of pairs of vertices that represent the edges in
     * the graph.
     */
    void ConstructEdgeToVertex_(
        const std::vector<std::pair<size_t, size_t>> &edges) {
        std::unordered_set<size_t> visited;
        for (auto edge : edges) {
            if (visited.contains(edge.first * num_vertices_ + edge.second) ||
                visited.contains(edge.second * num_vertices_ + edge.first)) {
                continue;
            }
            e_to_v_.push_back(edge);
            visited.insert(edge.first * num_vertices_ + edge.second);
            visited.insert(edge.second * num_vertices_ + edge.first);
        }
    }

    /**
     * @brief Get the number of vertices in the graph.
     *
     * This function returns the number of vertices in the graph.
     *
     * @return The number of vertices in the graph.
     */
    size_t GetNumVertices() const { return num_vertices_; }

    /**
     * @brief Get the number of edges in the graph.
     *
     * This function returns the number of edges in the graph. The number of
     * edges is calculated as the size of the vector of pairs of vertices
     * connected by edges.
     *
     * @return The number of edges in the graph.
     */
    size_t GetNumEdges() const { return e_to_v_.size(); }

    /**
     * @brief Construct the vertex-vertex adjacency matrix.
     *
     * @param num_vertices The number of vertices in the graph.
     * @param edges A vector of pairs of vertex indices representing the edges
     * in the graph.
     */
    void ConstructVertexToVertexMatrix_(
        const std::vector<std::pair<size_t, size_t>> &edges) {

        // Resize the CSR row pointer vector to hold one more element than the
        // number of vertices
        v_to_v_row_ptr_.resize(num_vertices_ + 1);

        // Initialize the CSR row pointer vector to zero
        std::fill(v_to_v_row_ptr_.begin(), v_to_v_row_ptr_.end(), 0);

        // Count the number of edges incident to each vertex
        for (const auto &edge : edges) {
            v_to_v_row_ptr_[edge.first + 1]++;
            v_to_v_row_ptr_[edge.second + 1]++;
        }

        // Compute the prefix sum of the CSR row pointer vector
        for (size_t i = 1; i < v_to_v_row_ptr_.size(); i++) {
            v_to_v_row_ptr_[i] += v_to_v_row_ptr_[i - 1];
        }

        // Resize the CSR column index vector and edge ID vector to hold the
        // total number of edges
        int numEdges = v_to_v_row_ptr_.back();
        v_to_v_col_.resize(numEdges);
        v_to_v_edges_.resize(numEdges);

        // Fill the CSR column index and edge ID vectors with the endpoints and
        // IDs of the edges
        std::vector<size_t> next(v_to_v_row_ptr_.begin(),
                                 v_to_v_row_ptr_.end());

        for (size_t i = 0; i < edges.size(); i++) {
            const auto &edge = edges[i];
            v_to_v_col_[next[edge.first]] = edge.second;
            v_to_v_edges_[next[edge.first]] = i;
            next[edge.first]++;

            v_to_v_col_[next[edge.second]] = edge.first;
            v_to_v_edges_[next[edge.second]] = i;
            next[edge.second]++;
        }
    }

    /**
     * @brief Construct the edge-edge adjacency matrix.
     *
     * @param num_vertices The number of vertices in the graph.
     * @param edges A vector of pairs of vertex indices representing the edges
     * in the graph.
     */
    void ConstructEdgeToEdgeMatrix_() {

        size_t num_dual_vertices = GetNumEdges();
        std::unordered_set<size_t> visited;
        std::vector<std::pair<size_t, size_t>> dual_edge_list;

        for (size_t i = 0; i < num_dual_vertices; i++) {
            const auto &vertices = GetVerticesConnectedByEdge(i);
            const auto &edges1 = GetEdgesTouchingVertex(vertices.first);
            const auto &edges2 = GetEdgesTouchingVertex(vertices.second);

            for (size_t j = 0; j < edges1.size(); j++) {
                if (edges1[j] != i &&
                    !visited.contains(i * num_dual_vertices + edges1[j]) &&
                    !visited.contains(edges1[j] * num_dual_vertices + i)) {
                    dual_edge_list.push_back(std::make_pair(i, edges1[j]));
                    visited.insert(i * num_dual_vertices + edges1[j]);
                    visited.insert(edges1[j] * num_dual_vertices + i);
                }
            }
            for (size_t j = 0; j < edges2.size(); j++) {
                if (edges2[j] != i &&
                    !visited.contains(i * num_dual_vertices + edges2[j]) &&
                    !visited.contains(edges2[j] * num_dual_vertices + i)) {
                    dual_edge_list.push_back(std::make_pair(i, edges2[j]));
                    visited.insert(i * num_dual_vertices + edges2[j]);
                    visited.insert(edges2[j] * num_dual_vertices + i);
                }
            }
        }

        auto csr =
            Utils::ConvertEdgeListToCSR(num_dual_vertices, dual_edge_list);
        e_to_e_row_ptr_ = std::get<0>(csr);
        e_to_e_col_ = std::get<1>(csr);
    }

    /**
     * @brief Get a row of edges in the graph that touch a given vertex.
     *
     * @param vertex_index The index of the vertex in the graph.
     * @return A row of edges in the graph that touch the vertex.
     *
     * This function returns a row of edges in the graph that touch the given
     * vertex. The row is represented as a `SparseGraphRow` object, which is a
     * lightweight view into a range of elements in a larger container.
     *
     * The edges in the row are returned as an rvalue reference to a
     * `SparseGraphRow` object, which allows the caller to avoid creating an
     * unnecessary copy of the row.
     *
     * Note that the caller should not use the returned reference after the
     * lifetime of the `DecodingGraph` object, since the `SparseGraphRow` object
     * holds a reference to the `v_to_v_edges_` container owned by the
     * `DecodingGraph`.
     */
    SparseGraphRow GetEdgesTouchingVertex(size_t vertex_index) const {
        size_t start = v_to_v_row_ptr_[vertex_index];
        size_t end = v_to_v_row_ptr_[vertex_index + 1];
        return SparseGraphRow(v_to_v_edges_, start, end);
    }

    /**
     * @brief Get the row of a sparse graph corresponding to the vertices
     * touching the given vertex.
     *
     * @param vertex_index Index of the vertex to get the row for.
     * @return SparseGraphRow A SparseGraphRow object containing the column
     * indices of the vertices touching the given vertex.
     *
     * This function returns a SparseGraphRow object representing the row of a
     * sparse graph that contains the column indices of the vertices that are
     * touching the given vertex. The indices are obtained from the
     * v_to_v_row_ptr_ array.
     */
    SparseGraphRow GetVerticesTouchingVertex(size_t vertex_index) const {
        size_t start = v_to_v_row_ptr_[vertex_index];
        size_t end = v_to_v_row_ptr_[vertex_index + 1];
        return SparseGraphRow(v_to_v_col_, start, end);
    }

    /**
     * @brief Get a row of edges in the graph that touch a given edge.
     *
     * @param edge_index The index of the edge in the graph.
     * @return A row of edges in the graph that touch the edge.
     *
     * This function returns a row of edges in the graph that touch the given
     * edge. The row is represented as a `SparseGraphRow` object, which is a
     * lightweight view into a range of elements in a larger container.
     *
     * The edges in the row are returned as an rvalue reference to a
     * `SparseGraphRow` object, which allows the caller to avoid creating an
     * unnecessary copy of the row.
     *
     * Note that the caller should not use the returned reference after the
     * lifetime of the `DecodingGraph` object, since the `SparseGraphRow` object
     * holds a reference to the `e_to_e_col_` container owned by the
     * `DecodingGraph`.
     */
    SparseGraphRow GetEdgesTouchingEdge(size_t edge_index) const {
        size_t start = e_to_e_row_ptr_[edge_index];
        size_t end = e_to_e_row_ptr_[edge_index + 1];
        return SparseGraphRow(e_to_e_col_, start, end);
    }

    /**
     * @brief Get the pair of vertices connected by a given edge.
     *
     * @param edge_index The index of the edge in the graph.
     * @return The pair of indices of the two vertices connected by the edge.
     *
     * This function returns a reference to the pair of indices of the two
     * vertices connected by the edge with index `edge_index`. The function
     * simply looks up the pair in the `e_to_v_` map stored by the
     * `DecodingGraph` object.
     *
     * Note that the returned reference is const, so the caller should not
     * attempt to modify the pair of vertices.
     */

    const std::pair<size_t, size_t> &
    GetVerticesConnectedByEdge(size_t edge_index) const {
        return e_to_v_[edge_index];
    }

    /**
     * @brief Get the index of the edge that connects a given pair of vertices.
     *
     * @param vertex_pair A pair of indices of the two vertices.
     * @return The index of the edge that connects the two vertices.
     *
     * This function returns the index of the edge in the graph that connects
     * the two vertices specified by `vertex_pair`. The function searches for
     * the edge by iterating over the row in the graph corresponding to the
     * first vertex in `vertex_pair` and looking for the column index
     * corresponding to the second vertex in `vertex_pair`.
     *
     * If the edge is found, its index is returned. If the edge is not found, an
     * assertion failure occurs.
     */
    size_t
    GetEdgeFromVertexPair(const std::pair<size_t, size_t> &vertex_pair) const {
        size_t start = v_to_v_row_ptr_[vertex_pair.first];
        size_t end = v_to_v_row_ptr_[vertex_pair.first + 1];

        // Search for the column index j in the row
        for (size_t k = start; k < end; k++) {
            if (v_to_v_col_[k] == vertex_pair.second) {
                // j is in the row, so the element is 1
                return v_to_v_edges_[k];
            }
        }

        assert(false && "Edge not found");
        return -1;
    }
};
}; // namespace Plaquette
