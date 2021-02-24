#include "gtest/gtest.h"

#include "Domain/Criterion/AlwaysValidCriterion.hpp"
#include "Domain/Criterion/SimilarSizeCriterion.hpp"

#include "Technical/Repositories/GraphFileRepository.hpp"

static constexpr char* instance = "Instances/quatreSommets.txt";

TEST(CriterionSuite, AlwaysValid)
{
    Yolo::Graph graph(10, 10, 1, 9);

    Yolo::AlwaysValidCriterion criterion;

    Yolo::Solution solution(-1, -1);
    EXPECT_TRUE(criterion.evaluate(graph, solution));

    solution = Yolo::Solution(0, 0);
    EXPECT_TRUE(criterion.evaluate(graph, solution));

    solution = Yolo::Solution(1, 1);
    EXPECT_TRUE(criterion.evaluate(graph, solution));

    solution = Yolo::Solution(graph.getNbVertices(), 100);
    EXPECT_TRUE(criterion.evaluate(graph, solution));

    for (int vertex = 0; vertex < graph.getNbVertices(); ++vertex)
    {
        solution.setVertexClass(vertex, vertex);
        EXPECT_TRUE(criterion.evaluate(graph, solution));
    }
}

TEST(CriterionSuite, SimilarSize)
{
    Yolo::Graph graph(10, 10, 1, 9);

    Yolo::SimilarSizeCriterion criterion(1);

    Yolo::Solution solution(graph.getNbVertices(), -1);
    EXPECT_FALSE(criterion.evaluate(graph, solution));

    solution = Yolo::Solution(graph.getNbVertices(), 0);
    EXPECT_FALSE(criterion.evaluate(graph, solution));

    /* A solution with a vertex without class should not be valid. */
    solution = Yolo::Solution(graph.getNbVertices(), 1);
    EXPECT_FALSE(criterion.evaluate(graph, solution));

    for (int vertex = 0; vertex < graph.getNbVertices(); ++vertex)
    {
        EXPECT_FALSE(criterion.evaluate(graph, solution));
        solution.setVertexClass(vertex, 0);
    }

    EXPECT_TRUE(criterion.evaluate(graph, solution));

    // Todo: How to test for other criterion's parameters?
}
