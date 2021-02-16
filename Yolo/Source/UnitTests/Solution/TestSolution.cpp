#include "gtest/gtest.h"

#include "Domain/Solution/Solution.hpp"

TEST(SolutionClassSuite, ConstructionParameters)
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

TEST(SolutionClassSuite, SetVertexClass)
{
    Yolo::Solution solution(1, 1);

    /* Out-of-bounds parameters should do nothing. */
    solution.setVertexClass(-1, 0);
    solution.setVertexClass(1, 0);

    EXPECT_EQ(solution.getVertexClass(-1), sVertexNoClass);
    EXPECT_EQ(solution.getVertexClass(1), sVertexNoClass);

    EXPECT_EQ(solution.getVertexClass(0), sVertexNoClass);

    solution.setVertexClass(0, sVertexNoClass - 1);

    EXPECT_EQ(solution.getVertexClass(0), sVertexNoClass);

    solution.setVertexClass(0, 1);

    EXPECT_EQ(solution.getVertexClass(0), sVertexNoClass);

    /* Good input */
    solution.setVertexClass(0, 0);

    EXPECT_EQ(solution.getVertexClass(0), 0);
}

TEST(SolutionClassSuite, GetVertexClass)
{
    Yolo::Solution solution(1, 1);

    solution.setVertexClass(0, 0);

    EXPECT_EQ(solution.getVertexClass(-1), sVertexNoClass);
    EXPECT_EQ(solution.getVertexClass(0), 0);
    EXPECT_EQ(solution.getVertexClass(1), sVertexNoClass);
}
