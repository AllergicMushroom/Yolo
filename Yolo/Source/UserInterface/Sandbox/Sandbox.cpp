#include "Core/Core.hpp"

#include "Domain/Algorithm/ExplicitEnumerationAlgorithm.hpp"
#include "Domain/Algorithm/GradientDescentAlgorithm.hpp"
#include "Domain/Algorithm/ImplicitEnumerationAlgorithm.hpp"
#include "Domain/Algorithm/SimulatedAnnealingAlgorithm.hpp"
#include "Domain/Algorithm/TabuListAlgorithm.hpp"

#include "Domain/Criterion/SimilarSizeCriterion.hpp"

#include "Domain/Neighborhood/PickNDropNeighborhood.hpp"
#include "Domain/Neighborhood/SwapNeighborhood.hpp"
#include "Domain/Neighborhood/SweepNeighborhood.hpp"

#include "Technical/Repositories/GraphFileRepository.hpp"

#include <chrono>

int main()
{
    Yolo::Core::initialize();

    Yolo::GraphFileRepository graphRepository;

    const auto& startPoint2 = std::chrono::steady_clock::now();

    std::optional<Yolo::Graph> graphOptional = graphRepository.load("Instances/dixSommets.txt");

    const auto& endPoint2 = std::chrono::steady_clock::now();

    auto elapsedSeconds2 = std::chrono::duration_cast<std::chrono::milliseconds>(endPoint2 - startPoint2).count() / 1000.0f;

    YOLO_INFO("Loading took {0} seconds.\n", elapsedSeconds2);

    if (graphOptional.has_value())
    {
        Yolo::Graph graph1 = *graphOptional;
        // Yolo::Graph graph2 = Yolo::Graph(500, 2000, 1, 4);

        Yolo::Graph graphs[] = {
            graph1,
            // graph2
        };

        int nbClasses = 4;

        for (const auto& graph : graphs)
        {
            Yolo::ExplicitEnumerationAlgorithm EE = Yolo::ExplicitEnumerationAlgorithm(graph, nbClasses);
            Yolo::ImplicitEnumerationAlgorithm IE = Yolo::ImplicitEnumerationAlgorithm(graph, nbClasses);
            Yolo::GradientDescentAlgorithm GD = Yolo::GradientDescentAlgorithm(graph, nbClasses);
            Yolo::TabuListAlgorithm TA = Yolo::TabuListAlgorithm(graph, nbClasses);
            Yolo::SimulatedAnnealingAlgorithm SA = Yolo::SimulatedAnnealingAlgorithm(graph, nbClasses);

            Yolo::Algorithm* algorithms[] = {
                &EE,
                &IE,
                &GD,
                &TA,
                &SA
                //
            };

            for (auto& algorithm : algorithms)
            {
                YOLO_INFO(algorithm->getName());
                const auto& startPoint = std::chrono::steady_clock::now();

                std::optional<Yolo::Solution> solution = algorithm->solve();

                const auto& endPoint = std::chrono::steady_clock::now();

                auto elapsedSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(endPoint - startPoint).count() / 1000.0f;

                const std::string& details = algorithm->getDetails();
                if (!details.empty())
                {
                    YOLO_INFO(details);
                }

                if (solution.has_value())
                {
                    // YOLO_INFO("Best solution found: {0}", solution.toString());
                    YOLO_INFO("Cost: {0}", graph.getSolutionCost(*solution));
                    YOLO_INFO("Found in {0} seconds.\n", elapsedSeconds);
                }
                else
                {
                    YOLO_INFO("The instance is infeasible.\n");
                }
            }
        }
    }

    Yolo::Core::shutdown();

    return 0;
}
