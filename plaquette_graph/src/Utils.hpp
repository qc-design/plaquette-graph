#pragma once

#include <iostream>
#include <tuple>
#include <vector>

namespace Plaquette {
namespace Utils {

std::tuple<std::vector<size_t>, std::vector<size_t>>
ConvertEdgeListToCSR(size_t num_vertices,
                     const std::vector<std::pair<size_t, size_t>> &edges) {

    std::vector<size_t> row_ptr;
    std::vector<size_t> col_ind;

    row_ptr.resize(num_vertices + 1);

    // Initialize the CSR row pointer vector to zero
    std::fill(row_ptr.begin(), row_ptr.end(), 0);

    // Count the number of edges incident to each vertex
    for (const auto &edge : edges) {
        row_ptr[edge.first + 1]++;
        row_ptr[edge.second + 1]++;
    }

    // Compute the prefix sum of the CSR row pointer vector
    for (size_t i = 1; i < row_ptr.size(); i++) {
        row_ptr[i] += row_ptr[i - 1];
    }

    // Resize the CSR col_indumn index vector and edge ID vector to hold the
    // total number of edges
    int numEdges = row_ptr.back();
    col_ind.resize(numEdges);

    // Fill the CSR col_indumn index and edge ID vectors with the endpoints and
    // IDs of the edges
    std::vector<size_t> next(row_ptr.begin(), row_ptr.end());

    for (size_t i = 0; i < edges.size(); i++) {
        const auto &edge = edges[i];
        col_ind[next[edge.first]] = edge.second;
        next[edge.first]++;

        col_ind[next[edge.second]] = edge.first;
        next[edge.second]++;
    }

    return std::make_tuple(row_ptr, col_ind);
}

}; // namespace Utils
}; // namespace Plaquette
