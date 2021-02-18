#include "gtest/gtest.h"

#include "Technical/Repositories/GraphFileRepository.hpp"

static constexpr char* instance = "Instances/quatreSommets.txt";

TEST(RepositorySuite, save)
{
    constexpr char* fileName = "TempInstance.txt";

    Yolo::GraphFileRepository repository;

    auto graphOptional = repository.load(instance);
    EXPECT_TRUE(graphOptional.has_value());
    if (graphOptional.has_value())
    {
        Yolo::Graph graph1 = *graphOptional;
        repository.save(fileName, graph1);

        graphOptional = repository.load(fileName);
        EXPECT_TRUE(graphOptional.has_value());
        if (graphOptional.has_value())
        {
            Yolo::Graph graph2 = *graphOptional;
            EXPECT_EQ(graph1, graph2);
        }
    }
}

TEST(RepositorySuite, load)
{
    constexpr char* fileName = "TempInstance.txt";

    Yolo::GraphFileRepository repository;

    auto graphOptional = repository.load(instance);
    EXPECT_TRUE(graphOptional.has_value());
    if (graphOptional.has_value())
    {
        Yolo::Graph graph1 = *graphOptional;
        repository.save(fileName, graph1);

        graphOptional = repository.load(fileName);
        EXPECT_TRUE(graphOptional.has_value());
        if (graphOptional.has_value())
        {
            Yolo::Graph graph2 = *graphOptional;
            EXPECT_EQ(graph1, graph2);
        }
    }
}
