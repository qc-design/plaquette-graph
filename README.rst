#################################
Plaquette Graph Plugin
#################################
.. image:: https://github.com/qc-design/plaquette-graph/actions/workflows/tests_linux.yml/badge.svg
    :target: https://github.com/qc-design/plaquette-graph/actions/workflows/tests_linux.yml

.. image:: https://github.com/qc-design/plaquette-graph/actions/workflows/docs_linux.yml/badge.svg
    :target: https://qc-design.github.io/plaquette-graph/
.. header-start-inclusion-marker-do-not-remove
	     
|
.. contents:: Table of Contents

.. about-start-inclusion-marker-do-not-remove

About 
=====
   
The `Plaquette-Graph <https://github.com/qc-design/plaquette-graph>`_ plugin extends the `Plaquette <https://github.com/qc-design/plaquette>`_ error correction software, providing a fast graph library written in C++.

.. about-end-inclusion-marker-do-not-remove

.. installation-start-inclusion-marker-do-not-remove

Installation
============

The C++ tests/examples and python bindings can be built independently by

.. code-block:: console

   cmake -Bbuild -G Ninja -DPLAQUETTE_GRAPH_BUILD_TESTS=On -DPLAQUETTE_GRAPH_BUILD_BINDINGS=On
   cmake --build ./build

   
You can run the C++ backend tests with
   
.. code-block:: console

   make test-cpp


You can install just the python interface with (this quietly builds the C++ backend):

.. code-block:: console

   pip install -r requirements.txt
   pip install .


You can run the python frontend tests with
   
.. code-block:: console

   make test-python

To generate the documentation you will need to install graphviz and doxygen. Then run

.. code-block:: console

   pip install -r doc/requirements.txt
   make docs
   firefox ./doc/_build/html/index.html


Usage
============

The primary graph class is SparseGraph, an undirected immutable graph structure stored in CSR format for extremely fast lookup of neighbouring vertices and edges. See the documentation for other types of graphs we support.

Python Frontend
---------------

.. code-block:: python

		import plaquette_graph as pg

		edges = [(0, 1), (0, 2), (1, 2)]
		num_vertices = 3
		graph = pg.SparseGraph(num_vertices, edges)

C++ Backend
---------------

.. code-block:: C++

		#include "SparseGraph.hpp"

		int main(int argc, char *argv[]) {

		  using namespace Plaquette;
		  std::vector<std::pair<size_t,size_t>> edges = {{0,1},{0,2},{1,2}};
		  size_t num_vertices = 3;
		  auto graph = SparseGraph(num_vertices, edges);
		  
		}
