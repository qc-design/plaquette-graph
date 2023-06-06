PYTHON3 := $(shell which python3 2>/dev/null)

PYTHON := python3
COVERAGE := --cov=plaquette_graph --cov-report term-missing --cov-report=html:coverage_html_report
TESTRUNNER := -m pytest tests --tb=short

.PHONY: format format-cpp format-python clean test-builtin test-cpp clean-docs
format: format-cpp format-python

format-cpp:
ifdef check
	./bin/format --check --cfversion $(if $(version:-=),$(version),0) ./plaquette_graph/src
else
	./bin/format --cfversion $(if $(version:-=),$(version),0) ./plaquette_graph/src ./plaquette_graph/src/tests
endif

format-python:
ifdef check
	black -l 100 ./tests ./plaquette_graph/ --check
else
	black -l 100 ./tests ./plaquette_graph/
endif

test-python:
	$(PYTHON) -I $(TESTRUNNER)

test-cpp:
	rm -rf ./BuildTests
	cmake . -BBuildTests -DPLAQUETTE_GRAPH_BUILD_TESTS=On
	cmake --build ./BuildTests
	./BuildTests/plaquette_graph/src/tests/runner_unionfind

docs:
	$(MAKE) -C doc html

clean-docs:
	$(MAKE) -C doc clean

clean:
	$(PYTHON) setup.py clean --all
	rm -rf __pycache__
	rm -rf src/__pycache__
	rm -rf tests/__pycache__
	rm -rf src/tests/__pycache__
	rm -rf dist
	rm -rf build
	rm -rf BuildTests Build
	rm -rf .coverage coverage_html_report/
	rm -rf tmp
	rm -rf *.dat
	rm -f *~
	rm -f *#*
	rm -rf *egg*
	for d in $(find . -name "*~"); do rm $d; done
	rm *.so
