import pytest
import plaquette_graph as pcg


def test_DecodingGraph():
    edges = [(0, 1), (0, 2), (1, 2)]
    num_vertices = 3
    boundary_vertices = [False, True, True]
    graph = pcg.DecodingGraph(num_vertices, edges, boundary_vertices)
    assert graph.get_num_vertices() == 3
    assert graph.get_num_edges() == 3
    assert graph.is_vertex_on_boundary(0) == False
    assert graph.is_vertex_on_boundary(1) == True
    assert graph.is_vertex_on_boundary(2) == True
