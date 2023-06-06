#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include "Test_DecodingGraph.hpp"
#include "Test_MultiGraph.hpp"
#include "Test_SparseGraph.hpp"

int main(int argc, char *argv[]) {
    int result;
    result = Catch::Session().run(argc, argv);
    return result;
}
