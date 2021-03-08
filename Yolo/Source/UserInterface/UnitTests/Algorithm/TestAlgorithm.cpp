#include "gtest/gtest.h"

#include "Domain/Algorithm/ExplicitEnumerationAlgorithm.hpp"
#include "Domain/Algorithm/GradientDescentAlgorithm.hpp"
#include "Domain/Algorithm/ImplicitEnumerationAlgorithm.hpp"
#include "Domain/Algorithm/TabuAlgorithm.hpp"

#include "Domain/Criterion/AlwaysValidCriterion.hpp"
#include "Domain/Criterion/SimilarSizeCriterion.hpp"

#include "Domain/Neighborhood/PickNDropNeighborhood.hpp"
#include "Domain/Neighborhood/SwapNeighborhood.hpp"
#include "Domain/Neighborhood/SweepNeighborhood.hpp"

#include "Technical/Repositories/GraphFileRepository.hpp"

static constexpr char* instance = "Instances/quatreSommets.txt";

TEST(AlgorithmsSuite, ExplicitEnumerationValid)
{
    Yolo::AlwaysValidCriterion alwaysValid = Yolo::AlwaysValidCriterion();
    Yolo::SimilarSizeCriterion similarSize = Yolo::SimilarSizeCriterion(1);

    Yolo::Criterion* criterions[] = {&alwaysValid,
                                     &similarSize};

    Yolo::GraphFileRepository graphRepository;

    std::optional<Yolo::Graph> graphOptional = graphRepository.load(instance);
    if (graphOptional.has_value())
    {
        Yolo::Graph graph = *graphOptional;

        constexpr int maxNbClasses = 100;
        for (int nbClasses = 1; nbClasses < maxNbClasses; ++nbClasses)
        {
            for (const auto& criterion : criterions)
            {
                Yolo::ExplicitEnumerationAlgorithm algorithm(graph, nbClasses);
                algorithm.setCriterion(criterion);

                Yolo::Solution solution = algorithm.solve().value();

                EXPECT_TRUE(criterion->evaluate(graph, solution));
            }
        }
    }
}

TEST(AlgorithmsSuite, ImplicitEnumerationValid)
{
    Yolo::AlwaysValidCriterion alwaysValid = Yolo::AlwaysValidCriterion();
    Yolo::SimilarSizeCriterion similarSize = Yolo::SimilarSizeCriterion(1);

    Yolo::Criterion* criterions[] = {&alwaysValid,
                                     &similarSize};

    Yolo::GraphFileRepository graphRepository;

    std::optional<Yolo::Graph> graphOptional = graphRepository.load(instance);
    if (graphOptional.has_value())
    {
        Yolo::Graph graph = *graphOptional;

        constexpr int maxNbClasses = 100;
        for (int nbClasses = 1; nbClasses < maxNbClasses; ++nbClasses)
        {
            for (const auto& criterion : criterions)
            {
                Yolo::ImplicitEnumerationAlgorithm algorithm(graph, nbClasses);
                algorithm.setCriterion(criterion);

                Yolo::Solution solution = algorithm.solve().value();

                EXPECT_TRUE(criterion->evaluate(graph, solution));
            }
        }
    }
}

TEST(AlgorithmsSuite, ExplicitImplicitEquality)
{
    Yolo::AlwaysValidCriterion alwaysValid = Yolo::AlwaysValidCriterion();
    Yolo::SimilarSizeCriterion similarSize = Yolo::SimilarSizeCriterion(1);

    Yolo::Criterion* criterions[] = {&alwaysValid,
                                     &similarSize};

    Yolo::GraphFileRepository graphRepository;

    std::optional<Yolo::Graph> graphOptional = graphRepository.load(instance);
    if (graphOptional.has_value())
    {
        Yolo::Graph graph = *graphOptional;

        constexpr int maxNbClasses = 100;
        for (int nbClasses = 1; nbClasses < maxNbClasses; ++nbClasses)
        {
            for (const auto& criterion : criterions)
            {
                Yolo::ExplicitEnumerationAlgorithm explicitAlgorithm(graph, nbClasses);
                explicitAlgorithm.setCriterion(criterion);

                Yolo::ImplicitEnumerationAlgorithm implicitAlgorithm(graph, nbClasses);
                implicitAlgorithm.setCriterion(criterion);

                Yolo::Solution explicitSolution = explicitAlgorithm.solve().value();
                Yolo::Solution implicitSolution = implicitAlgorithm.solve().value();

                EXPECT_EQ(graph.getSolutionCost(explicitSolution), graph.getSolutionCost(implicitSolution));
            }
        }
    }
}

TEST(AlgorithmsSuite, GradientDescentValid)
{
    Yolo::AlwaysValidCriterion alwaysValid = Yolo::AlwaysValidCriterion();
    Yolo::SimilarSizeCriterion similarSize = Yolo::SimilarSizeCriterion(1);

    Yolo::Criterion* criterions[] = {&alwaysValid,
                                     &similarSize};

    Yolo::SwapNeighborhood swap = Yolo::SwapNeighborhood();
    Yolo::SweepNeighborhood sweep = Yolo::SweepNeighborhood();
    Yolo::PickNDropNeighborhood pnd = Yolo::PickNDropNeighborhood();

    Yolo::Neighborhood* neighborhoods[] = {&swap, &sweep, &pnd};

    Yolo::GraphFileRepository graphRepository;

    std::optional<Yolo::Graph> graphOptional = graphRepository.load(instance);
    if (graphOptional.has_value())
    {
        Yolo::Graph graph = *graphOptional;

        constexpr int maxNbClasses = 100;
        for (int nbClasses = 1; nbClasses < maxNbClasses; ++nbClasses)
        {
            for (const auto& criterion : criterions)
            {
                for (const auto& neighborhood : neighborhoods)
                {
                    Yolo::GradientDescentAlgorithm algorithm(graph, nbClasses);
                    algorithm.setCriterion(criterion);
                    algorithm.setNeighborhood(neighborhood);
                    algorithm.setEpsilon(1);

                    Yolo::Solution solution = algorithm.solve().value();

                    EXPECT_TRUE(criterion->evaluate(graph, solution));
                }
            }
        }
    }
}

TEST(AlgorithmsSuite, TabuAlgorithmValid)
{
    Yolo::AlwaysValidCriterion alwaysValid = Yolo::AlwaysValidCriterion();
    Yolo::SimilarSizeCriterion similarSize = Yolo::SimilarSizeCriterion(1);

    Yolo::Criterion* criterions[] = {&alwaysValid,
                                     &similarSize};

    Yolo::SwapNeighborhood swap = Yolo::SwapNeighborhood();
    Yolo::SweepNeighborhood sweep = Yolo::SweepNeighborhood();
    Yolo::PickNDropNeighborhood pnd = Yolo::PickNDropNeighborhood();

    Yolo::Neighborhood* neighborhoods[] = {&swap, &sweep, &pnd};

    Yolo::GraphFileRepository graphRepository;

    std::optional<Yolo::Graph> graphOptional = graphRepository.load(instance);
    if (graphOptional.has_value())
    {
        Yolo::Graph graph = *graphOptional;

        constexpr int maxNbClasses = 100;
        for (int nbClasses = 1; nbClasses < maxNbClasses; ++nbClasses)
        {
            for (const auto& criterion : criterions)
            {
                for (const auto& neighborhood : neighborhoods)
                {
                    Yolo::TabuAlgorithm algorithm(graph, nbClasses);
                    algorithm.setCriterion(criterion);
                    algorithm.setNeighborhood(neighborhood);
                    algorithm.setTabuListSize(10);
                    algorithm.setMaxIterations(1000);
                    algorithm.setStoreAll(true);

                    Yolo::Solution solution = algorithm.solve().value();

                    EXPECT_TRUE(criterion->evaluate(graph, solution));
                }
            }
        }
    }
}
