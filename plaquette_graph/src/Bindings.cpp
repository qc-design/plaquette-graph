#include <functional>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "DecodingGraph.hpp"
#include "MultiGraph.hpp"
#include "SparseGraph.hpp"

namespace {
using namespace Plaquette;
namespace py = pybind11;

PYBIND11_MODULE(plaquette_graph_bindings, m) {

    py::class_<MultiGraph>(m, "MultiGraph",
                           R"pbdoc(
        Undirected multi-graph class.

        An instance of this class represents an undirected multi-graph, which is a graph
        where multiple edges can exist between two vertices. Edges are identified by
        their indices, which correspond to their position in the list of edges provided
        when the graph is constructed.
    )pbdoc")
        .def(py::init<const std::vector<std::pair<size_t, size_t>> &,
                      const std::vector<size_t> &>(),
             py::arg("edges"), py::arg("weights"),
             R"pbdoc(
             Construct an undirected multi-graph.

             Args:
                 edges: A list of pairs of integers representing the edges of the graph.
                        Each pair represents the indices of the two vertices that the
                        edge connects.
                 weights: A list of integers representing the weights of each edge.
                          The length of this list must be equal to the number of edges
                          in the graph.

             Raises:
                 ValueError: If the length of the `weights` list does not match the
                             number of edges in the graph.
             )pbdoc")
        .def("get_edges_touching_vertex", &MultiGraph::GetEdgesTouchingVertex,
             py::arg("vertex"),
             R"pbdoc(
             Get the indices of the edges that touch a given vertex.

             Args:
                 vertex: An integer representing the index of the vertex.

             Returns:
                 A list of integers representing the indices of the edges that touch the
                 vertex.
             )pbdoc")
        .def("get_weight", &MultiGraph::GetWeight, py::arg("edge_id"),
             R"pbdoc(
             Get the weight of the edge connecting two vertices.

             Args:
                 edge_id: An integer representing the edge index.

             Returns:
                 The weight of the edge. If no such edge
                 exists, returns 0.
             )pbdoc")
        .def("get_edge_connecting_vertices",
             &MultiGraph::GetEdgeConnectingVertices, py::arg("vertex1"),
             py::arg("vertex2"),
             R"pbdoc(
             Get the index of the edge connecting two vertices.

             Args:
                 vertex1: An integer representing the index
                 of the first vertex.
                 vertex2: An integer representing the index of the second vertex.

             Returns:
                 The index of the edge connecting the two vertices. If no such edge
                 exists, returns the number of edges in the graph.
             )pbdoc")
        .def("get_edges_touching_edge", &MultiGraph::GetEdgesTouchingEdge,
             py::arg("edge"),
             R"pbdoc(
             Get the indices of the edges that touch a given edge.

             Args:
                 edge: An integer representing the index of the edge.

             Returns:
                 A list of integers representing the indices of the edges that touch the
                 edge.
             )pbdoc")
        .def("get_vertices_touching_vertex",
             &MultiGraph::GetVerticesTouchingVertex, py::arg("vertex"),
             R"pbdoc(
             Get the indices of the vertices that touch a given vertex.

             Args:
                 vertex: An integer representing the index of the vertex.

             Returns:
                 A list of integers representing the indices of the vertexs that touch the
                 vertex.
             )pbdoc")
        .def("get_num_vertices", &MultiGraph::GetNumVertices,
             R"pbdoc(
             Get the number of vertices in the graph.

             Returns:
                 An integer representing the number of vertices in the graph.
             )pbdoc")
        .def("get_num_edges", &MultiGraph::GetNumEdges,
             R"pbdoc(
             Get the number of edges in the graph.

             Returns:
                 An integer representing the number of edges in the graph.
             )pbdoc");

    pybind11::class_<SparseGraphRow>(m, "SparseGraphRow",
                                     "A lightweight container for a row of the "
                                     "SparseGraph Adjacency matrix.")
        .def("size", &SparseGraphRow::size,
             "Return the number of entries in the row.")
        .def(
            "__getitem__",
            [](const SparseGraphRow &obj, int index) {
                if (index < 0 || index >= obj.size()) {
                    throw pybind11::index_error();
                }
                return obj[index];
            },
            "Get the value at the given index in the row.")
        .def("__len__", &SparseGraphRow::size,
             "Return the number of entries in the row.");

    pybind11::class_<SparseGraph>(
        m, "SparseGraph", "A sparse graph represented by an adjacency list.")
        .def(pybind11::init<size_t,
                            const std::vector<std::pair<size_t, size_t>> &>(),
             "Construct a sparse graph with the given number of vertices and "
             "edges. The edges are represented as a list of pairs of vertex "
             "indices.",
             py::arg("num_vertices"), py::arg("edges"))
        .def("get_num_vertices", &SparseGraph::GetNumVertices,
             "Return the number of vertices in the graph.")
        .def("get_num_edges", &SparseGraph::GetNumEdges,
             "Return the number of edges in the graph.")
        .def("get_edges_touching_vertex", &SparseGraph::GetEdgesTouchingVertex,
             "Return a list of the indices of edges touching the vertex with "
             "the given index.",
             py::arg("vertex_index"))
        .def("get_vertices_touching_vertex",
             &SparseGraph::GetVerticesTouchingVertex,
             "Return a list of the indices of vertices connected to the vertex "
             "with the given index.",
             py::arg("vertex_index"))
        .def("get_edges_touching_edge", &SparseGraph::GetEdgesTouchingEdge,
             "Return a list of the indices of edges touching the edge with the "
             "given index.",
             py::arg("edge_index"))
        .def("get_vertices_connected_by_edge",
             &SparseGraph::GetVerticesConnectedByEdge,
             "Return a list of the indices of vertices connected by the edge "
             "with the given index.",
             py::arg("edge_index"));

    pybind11::class_<DecodingGraph, SparseGraph>(
        m, "DecodingGraph",
        "A decoding graph represented by an adjacency list.")
        .def(
            pybind11::init<size_t,
                           const std::vector<std::pair<size_t, size_t>> &,
                           const std::vector<bool> &>(),
            "Construct a decoding graph with the given number of vertices, "
            "edges, and boundary vertices. The edges are represented as a list "
            "of pairs of vertex indices. The boundary vertices are represented "
            "as a list of booleans, with True indicating a boundary vertex.",
            py::arg("num_vertices"), py::arg("edges"),
            py::arg("boundary_vertices"))
        .def("get_num_vertices", &SparseGraph::GetNumVertices,
             "Return the number of vertices in the graph.")
        .def("get_num_edges", &SparseGraph::GetNumEdges,
             "Return the number of edges in the graph.")
        .def("get_edges_touching_vertex", &SparseGraph::GetEdgesTouchingVertex,
             "Return a list of the indices of edges touching the vertex with "
             "the given index.",
             py::arg("vertex_index"))
        .def("get_vertices_touching_vertex",
             &SparseGraph::GetVerticesTouchingVertex,
             "Return a list of the indices of vertices connected to the vertex "
             "with the given index.",
             py::arg("vertex_index"))
        .def("get_edges_touching_edge", &SparseGraph::GetEdgesTouchingEdge,
             "Return a list of the indices of edges touching the edge with the "
             "given index.",
             py::arg("edge_index"))
        .def("get_vertices_connected_by_edge",
             &SparseGraph::GetVerticesConnectedByEdge,
             "Return a list of the indices of vertices connected by the edge "
             "with the given index.",
             py::arg("edge_index"))
        .def("is_vertex_on_boundary", &DecodingGraph::IsVertexOnBoundary,
             "Return True if the vertex with the given index is a boundary "
             "vertex, and False otherwise.",
             py::arg("vertex_index"));
}

} // namespace
