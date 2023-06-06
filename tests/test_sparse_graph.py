import pytest
import plaquette_graph as pcg


def test_SparseGraph():
    edges = [(0, 1), (0, 2), (1, 2)]
    num_vertices = 3
    graph = pcg.SparseGraph(num_vertices, edges)

    assert graph.get_num_vertices() == 3
    assert graph.get_num_edges() == 3

    assert graph.get_edges_touching_vertex(0)[0] == 0
    assert graph.get_edges_touching_vertex(0)[1] == 1
    assert graph.get_edges_touching_vertex(1)[0] == 0
    assert graph.get_edges_touching_vertex(1)[1] == 2
    assert graph.get_edges_touching_vertex(1).size() == 2

    assert graph.get_edges_touching_vertex(2)[0] == 1
    assert graph.get_edges_touching_vertex(2)[1] == 2
    assert graph.get_edges_touching_vertex(2).size() == 2

    assert graph.get_vertices_touching_vertex(0)[0] == 1
    assert graph.get_vertices_touching_vertex(0)[1] == 2

    assert graph.get_vertices_touching_vertex(1)[0] == 0
    assert graph.get_vertices_touching_vertex(1)[1] == 2
    assert graph.get_vertices_touching_vertex(2)[0] == 0
    assert graph.get_vertices_touching_vertex(2)[1] == 1

    assert graph.get_edges_touching_edge(0)[0] == 1
    assert graph.get_edges_touching_edge(0)[1] == 2

    assert graph.get_edges_touching_edge(1)[0] == 0
    assert graph.get_edges_touching_edge(1)[1] == 2

    assert graph.get_edges_touching_edge(2)[0] == 0
    assert graph.get_edges_touching_edge(2)[1] == 1

    assert graph.get_vertices_connected_by_edge(0) == (0, 1)
    assert graph.get_vertices_connected_by_edge(1) == (0, 2)
    assert graph.get_vertices_connected_by_edge(2) == (1, 2)
