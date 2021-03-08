#include "gtest/gtest.h"

#include "Domain/Graph/Graph.hpp"

#include "Technical/Repositories/GraphFileRepository.hpp"

static constexpr char* instance1 = "Instances/quatreSommets.txt";
static constexpr char* instance2 = "Instances/cinqSommets.txt";

TEST(GraphSuite, constructors)
{
    Yolo::Graph graph(-1, -1, -1, -1);
    EXPECT_EQ(graph.getNbVertices(), 0);
    EXPECT_EQ(graph.getNbEdges(), 0);
    EXPECT_EQ(graph.getMinDegree(), 0);
    EXPECT_EQ(graph.getMaxDegree(), 0);

    graph = Yolo::Graph(0, -1, -1, -1);
    EXPECT_EQ(graph.getNbVertices(), 0);
    EXPECT_EQ(graph.getNbEdges(), 0);
    EXPECT_EQ(graph.getMinDegree(), 0);
    EXPECT_EQ(graph.getMaxDegree(), 0);

    graph = Yolo::Graph(1, -1, -1, -1);
    EXPECT_EQ(graph.getNbVertices(), 1);
    EXPECT_EQ(graph.getNbEdges(), 0);
    EXPECT_EQ(graph.getMinDegree(), 0);
    EXPECT_EQ(graph.getMaxDegree(), 0);

    graph = Yolo::Graph(2, 1000, -1, 1000);
    EXPECT_EQ(graph.getNbVertices(), 2);
    EXPECT_EQ(graph.getNbEdges(), 1);
    EXPECT_EQ(graph.getMinDegree(), 1);
    EXPECT_EQ(graph.getMaxDegree(), 1);

    /* Test that generation doesn't loop indefinitely. */
    constexpr int nbIterations = 100;

    constexpr int minDegree = 2;
    constexpr int maxDegree = 4;

    for (int i = 5; i < nbIterations; ++i)
    {
        int nbVertices = i;
        int nbEdges = 2 * i;

        graph = Yolo::Graph(nbVertices, nbEdges, minDegree, maxDegree);

        EXPECT_EQ(graph.getNbVertices(), nbVertices);
        EXPECT_EQ(graph.getNbEdges(), nbEdges);

        EXPECT_GE(graph.getMinDegree(), minDegree);
        EXPECT_LE(graph.getMaxDegree(), maxDegree);
    }

    /* Weird formatting lol */
    std::vector<std::vector<Yolo::Edge>> adjacencyList =
        {
            {Yolo::Edge(0, 1, 1), Yolo::Edge(0, 2, 1), Yolo::Edge(0, 3, 1)},
            {Yolo::Edge(1, 0, 1), Yolo::Edge(1, 3, 1)},
            {Yolo::Edge(2, 0, 1)},
            {Yolo::Edge(3, 0, 1), Yolo::Edge(3, 1, 1)}};

    graph = Yolo::Graph(adjacencyList);
    EXPECT_EQ(graph.getNbVertices(), 4);
    EXPECT_EQ(graph.getNbEdges(), 4);
    EXPECT_EQ(graph.getMinDegree(), 1);
    EXPECT_EQ(graph.getMaxDegree(), 3);
}

TEST(GraphSuite, equalityComparison)
{
    Yolo::GraphFileRepository graphRepository;

    std::optional<Yolo::Graph> graphOptional = graphRepository.load(instance1);
    if (graphOptional.has_value())
    {
        Yolo::Graph graph1 = *graphOptional;

        graphOptional = graphRepository.load(instance2);
        if (graphOptional.has_value())
        {
            Yolo::Graph graph2 = *graphOptional;

            EXPECT_EQ(graph1, graph1);
            EXPECT_EQ(graph2, graph2);

            EXPECT_NE(graph1, graph2);
        }
    }
}

TEST(GraphSuite, getters)
{
    constexpr int nbIterations = 100;

    constexpr int minDegree = 2;
    constexpr int maxDegree = 4;

    for (int i = 5; i < nbIterations; ++i)
    {
        int nbVertices = i;
        int nbEdges = 2 * i;

        Yolo::Graph graph = Yolo::Graph(nbVertices, nbEdges, minDegree, maxDegree);

        EXPECT_EQ(graph.getNbVertices(), nbVertices);
        EXPECT_EQ(graph.getNbEdges(), nbEdges);

        EXPECT_GE(graph.getMinDegree(), minDegree);
        EXPECT_LE(graph.getMaxDegree(), maxDegree);
    }
}
