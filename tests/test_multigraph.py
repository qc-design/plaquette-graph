import pytest
import plaquette_graph


def test_create_empty_graph():
    g = plaquette_graph.MultiGraph([], [])
    assert g.get_num_vertices() == 0
    assert g.get_num_edges() == 0


def test_create_graph_with_edges():
    edges = [(0, 1), (0, 2), (1, 2)]
    weights = [1, 2, 3]
    g = plaquette_graph.MultiGraph(edges, weights)
    assert g.get_num_vertices() == 3
    assert g.get_num_edges() == 3


def test_get_weight():
    edges = [(0, 1), (0, 2), (1, 2)]
    weights = [1, 2, 3]
    g = plaquette_graph.MultiGraph(edges, weights)
    assert g.get_weight(0) == 1
    assert g.get_weight(1) == 2
    assert g.get_weight(2) == 3


def test_get_edge_connecting_vertices():
    edges = [(0, 1), (0, 2), (1, 2)]
    weights = [1, 2, 3]
    g = plaquette_graph.MultiGraph(edges, weights)
    assert g.get_edge_connecting_vertices(0, 1) == 0
    assert g.get_edge_connecting_vertices(0, 2) == 1
    assert g.get_edge_connecting_vertices(1, 2) == 2
    assert g.get_edge_connecting_vertices(2, 0) == 1


def test_get_edges_touching_edge():
    edges = [(0, 1), (0, 2), (1, 2)]
    weights = [1, 2, 3]
    g = plaquette_graph.MultiGraph(edges, weights)
    assert g.get_edges_touching_edge(0) == [1, 2]
    assert g.get_edges_touching_edge(1) == [0, 2]
    assert g.get_edges_touching_edge(2) == [0, 1]


def test_get_edges_touching_vertex():
    edges = [(0, 1), (0, 2), (1, 2)]
    weights = [1, 2, 3]
    g = plaquette_graph.MultiGraph(edges, weights)
    assert g.get_edges_touching_vertex(0) == [0, 1]
    assert g.get_edges_touching_vertex(1) == [0, 2]
    assert g.get_edges_touching_vertex(2) == [1, 2]


def test_get_vertices_touching_vertex():
    edges = [(0, 1), (0, 2), (1, 2)]
    weights = [1, 2, 3]
    g = plaquette_graph.MultiGraph(edges, weights)
    assert g.get_vertices_touching_vertex(0) == [1, 2]
    assert g.get_vertices_touching_vertex(1) == [0, 2]
    assert g.get_vertices_touching_vertex(2) == [0, 1]


def test_create_graph_with_duplicate_edges():
    edges = [(0, 1), (0, 1), (1, 2)]
    weights = [1, 2, 3]
    g = plaquette_graph.MultiGraph(edges, weights)
    assert g.get_num_vertices() == 3
    assert g.get_num_edges()
