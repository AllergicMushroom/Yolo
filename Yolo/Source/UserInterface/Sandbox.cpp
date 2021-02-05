#include "Core/Core.hpp"

#include "Domain/Algorithm/ExplicitEnumerationAlgorithm.hpp"
#include "Domain/Algorithm/GradientDescentAlgorithm.hpp"
#include "Domain/Algorithm/ImplicitEnumerationAlgorithm.hpp"

#include "Domain/Criterion/SimilarSizeCriterion.hpp"

#include "Domain/Neighborhood/Neighborhood.hpp"

#include "Technical/Repositories/GraphFileRepository.hpp"

#include <chrono>

int main()
{
    Yolo::Core::initialize();

    Yolo::GraphFileRepository graphRepository;

    std::optional<Yolo::Graph> graphOptional = graphRepository.load("Instances/dixSommets.txt");
    if (graphOptional.has_value())
    {
        Yolo::Graph graph = *graphOptional;

        int nbClasses = 3;
        int epsilon = 0;

        Yolo::SimilarSizeCriterion criterion = Yolo::SimilarSizeCriterion(1);

        Yolo::ExplicitEnumerationAlgorithm EE = Yolo::ExplicitEnumerationAlgorithm(graph, nbClasses, &criterion);
        Yolo::ImplicitEnumerationAlgorithm IE = Yolo::ImplicitEnumerationAlgorithm(graph, nbClasses, &criterion);
        Yolo::GradientDescentAlgorithm GD = Yolo::GradientDescentAlgorithm(graph, nbClasses, epsilon, Yolo::pickNDrop, &criterion);

        Yolo::Algorithm* algorithms[] = {
            &EE,
            &IE,
            &GD};

        for (auto& algorithm : algorithms)
        {
            const auto& startPoint = std::chrono::steady_clock::now();

            Yolo::Solution solution = algorithm->solve();

            const auto& endPoint = std::chrono::steady_clock::now();

            auto elapsedSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(endPoint - startPoint).count() / 1000.0f;

            YOLO_INFO(algorithm->getName());
            YOLO_INFO("Best solution found: {0}", solution.toString());
            YOLO_INFO("Cost: {0}", graph.getSolutionCost(solution));
            YOLO_INFO("Found in {0} seconds.", elapsedSeconds);
        }
    }

    Yolo::Core::shutdown();

    return 0;
}
