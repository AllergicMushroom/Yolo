#include "gtest/gtest.h"

#include "Domain/Solution/Solution.hpp"

TEST(SolutionSuite, constructors)
{
    Yolo::Solution solution = Yolo::Solution(-1, -1);

    EXPECT_EQ(solution.getNbVertices(), 0);
    EXPECT_EQ(solution.getNbClasses(), 0);

    for (int i = 0; i < 100; ++i)
    {
        /* Make sure these are different. */
        int nbVertices = i;
        int nbClasses = 2 * i;
        solution = Yolo::Solution(nbVertices, nbClasses);

        EXPECT_EQ(solution.getNbVertices(), nbVertices);
        EXPECT_EQ(solution.getNbClasses(), nbClasses);
    }
}

TEST(SolutionSuite, getNbVertices)
{
    Yolo::Solution solution = Yolo::Solution(-1, -1);

    EXPECT_EQ(solution.getNbVertices(), 0);

    for (int i = 0; i < 100; ++i)
    {
        /* Make sure these are different. */
        int nbVertices = i;
        int nbClasses = 2 * i;
        solution = Yolo::Solution(nbVertices, nbClasses);

        EXPECT_EQ(solution.getNbVertices(), nbVertices);
    }
}

TEST(SolutionSuite, getNbClasses)
{
    Yolo::Solution solution = Yolo::Solution(-1, -1);

    EXPECT_EQ(solution.getNbClasses(), 0);

    for (int i = 0; i < 100; ++i)
    {
        /* Make sure these are different. */
        int nbVertices = i;
        int nbClasses = 2 * i;
        solution = Yolo::Solution(nbVertices, nbClasses);

        EXPECT_EQ(solution.getNbClasses(), nbClasses);
    }
}

TEST(SolutionSuite, setVertexClass)
{
    Yolo::Solution solution(1, 1);

    solution.setVertexClass(-1, 0);
    EXPECT_EQ(solution.getVertexClass(-1), sVertexNoClass);

    solution.setVertexClass(1, 0);
    EXPECT_EQ(solution.getVertexClass(1), sVertexNoClass);

    solution.setVertexClass(0, sVertexNoClass - 1);
    EXPECT_EQ(solution.getVertexClass(0), sVertexNoClass);

    solution.setVertexClass(0, 1);
    EXPECT_EQ(solution.getVertexClass(0), sVertexNoClass);

    solution.setVertexClass(0, 0);
    EXPECT_EQ(solution.getVertexClass(0), 0);
}

TEST(SolutionSuite, getVertexClass)
{
    Yolo::Solution solution(1, 1);

    solution.setVertexClass(0, 0);

    EXPECT_EQ(solution.getVertexClass(-1), sVertexNoClass);
    EXPECT_EQ(solution.getVertexClass(0), 0);
    EXPECT_EQ(solution.getVertexClass(1), sVertexNoClass);
}

TEST(SolutionSuite, getClassCardinal)
{
    constexpr int nbVertices = 100;
    constexpr int nbClasses = 100;

    Yolo::Solution solution(nbVertices, nbClasses);

    EXPECT_EQ(solution.getClassCardinal(-1), 0);
    EXPECT_EQ(solution.getClassCardinal(nbClasses + 1), 0);

    for (int c = 0; c < nbClasses; ++c)
    {
        EXPECT_EQ(solution.getClassCardinal(c), 0);
    }

    for (int c = 0; c < nbClasses; ++c)
    {
        for (int vertex = 0; vertex < nbVertices; ++vertex)
        {
            solution.setVertexClass(vertex, c);
        }

        EXPECT_EQ(solution.getClassCardinal(c), nbVertices);
    }
}
