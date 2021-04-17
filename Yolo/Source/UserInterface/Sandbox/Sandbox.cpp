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

    std::optional<Yolo::Graph> graphOptional = graphRepository.load("Instances/milleSommets.txt");

    const auto& endPoint2 = std::chrono::steady_clock::now();

    auto elapsedSeconds2 = std::chrono::duration_cast<std::chrono::milliseconds>(endPoint2 - startPoint2).count() / 1000.0f;

    YOLO_INFO("Loading took {0} seconds.\n", elapsedSeconds2);

    if (graphOptional.has_value())
    {
        Yolo::Graph graph1 = *graphOptional;

        Yolo::Graph graphs[] = {
            graph1,
        };

        const int nbClasses = 16;

        const int commonTabuListSize = 256;

        const double commonStartingTemperature = 4000;

        std::shared_ptr<Yolo::SwapNeighborhood> swapNeighborhood = std::shared_ptr<Yolo::SwapNeighborhood>(new Yolo::SwapNeighborhood());
        std::shared_ptr<Yolo::SweepNeighborhood> sweepNeighborhood = std::shared_ptr<Yolo::SweepNeighborhood>(new Yolo::SweepNeighborhood());
        std::shared_ptr<Yolo::PickNDropNeighborhood> pndNeighborhood = std::shared_ptr<Yolo::PickNDropNeighborhood>(new Yolo::PickNDropNeighborhood());

        for (const auto& graph : graphs)
        {
            Yolo::ExplicitEnumerationAlgorithm EE = Yolo::ExplicitEnumerationAlgorithm(graph, nbClasses);
            Yolo::ImplicitEnumerationAlgorithm IE = Yolo::ImplicitEnumerationAlgorithm(graph, nbClasses);

            Yolo::GradientDescentAlgorithm GD1 = Yolo::GradientDescentAlgorithm(graph, nbClasses);
            GD1.setNeighborhood(sweepNeighborhood);
            GD1.setMaxIterations(10);

            Yolo::GradientDescentAlgorithm GD2 = Yolo::GradientDescentAlgorithm(graph, nbClasses);
            GD2.setNeighborhood(sweepNeighborhood);
            GD2.setMaxIterations(100);

            Yolo::GradientDescentAlgorithm GD3 = Yolo::GradientDescentAlgorithm(graph, nbClasses);
            GD3.setNeighborhood(sweepNeighborhood);
            GD3.setMaxIterations(1000);

            Yolo::TabuListAlgorithm TA1 = Yolo::TabuListAlgorithm(graph, nbClasses);
            TA1.setNeighborhood(sweepNeighborhood);
            TA1.setTabuListSize(commonTabuListSize);
            TA1.setMaxIterations(10);

            Yolo::TabuListAlgorithm TA2 = Yolo::TabuListAlgorithm(graph, nbClasses);
            TA2.setNeighborhood(sweepNeighborhood);
            TA2.setTabuListSize(commonTabuListSize);
            TA2.setMaxIterations(100);

            Yolo::TabuListAlgorithm TA3 = Yolo::TabuListAlgorithm(graph, nbClasses);
            TA3.setNeighborhood(sweepNeighborhood);
            TA3.setTabuListSize(commonTabuListSize);
            TA3.setMaxIterations(1000);

            Yolo::SimulatedAnnealingAlgorithm SA1 = Yolo::SimulatedAnnealingAlgorithm(graph, nbClasses);
            SA1.setNeighborhood(pndNeighborhood);
            SA1.setStartingTemperature(commonStartingTemperature);
            SA1.setMaxIterations(10);

            Yolo::SimulatedAnnealingAlgorithm SA2 = Yolo::SimulatedAnnealingAlgorithm(graph, nbClasses);
            SA2.setNeighborhood(pndNeighborhood);
            SA2.setStartingTemperature(commonStartingTemperature);
            SA2.setMaxIterations(100);

            Yolo::SimulatedAnnealingAlgorithm SA3 = Yolo::SimulatedAnnealingAlgorithm(graph, nbClasses);
            SA3.setNeighborhood(pndNeighborhood);
            SA3.setStartingTemperature(commonStartingTemperature);
            SA3.setMaxIterations(1000);

            Yolo::Algorithm* algorithms[] = {
                //&EE,
                //&IE,
                //&GD1,
                //&GD2,
                //&GD3,
                //&TA1,
                //&TA2,
                //&TA3,
                &SA1,
                &SA2,
                &SA3
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
                    YOLO_INFO("Best solution found: {0}", solution->toString());
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
