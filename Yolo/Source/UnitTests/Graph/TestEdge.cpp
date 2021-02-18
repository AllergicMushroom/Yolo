#include "gtest/gtest.h"

#include "Domain/Graph/Edge.hpp"

TEST(EdgeSuite, constructors)
{
    Yolo::Edge edge(0, 1, 2.5);

    EXPECT_EQ(edge.getSource(), 0);
    EXPECT_EQ(edge.getDestination(), 1);
    EXPECT_EQ(edge.getWeight(), 2.5);
}

TEST(EdgeSuite, equalityComparison)
{
    Yolo::Edge edge1(0, 1, 2.5);
    Yolo::Edge edge2(0, 1, 2.5);

    EXPECT_EQ(edge1, edge2);

    edge2 = Yolo::Edge(1, 1, 2.5);
    EXPECT_NE(edge1, edge2);

    edge2 = Yolo::Edge(0, 2, 2.5);
    EXPECT_NE(edge1, edge2);

    edge2 = Yolo::Edge(0, 1, 3.5);
    EXPECT_NE(edge1, edge2);
}

TEST(EdgeSuite, getters)
{
    Yolo::Edge edge(0, 1, 2.5);

    EXPECT_EQ(edge.getSource(), 0);
    EXPECT_EQ(edge.getDestination(), 1);
    EXPECT_EQ(edge.getWeight(), 2.5);
}
