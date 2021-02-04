#include "Core/Core.hpp"

#include "Technical/Repositories/GraphFileRepository.hpp"

#include "Domain/Algorithm/Criterion.hpp"
#include "Domain/Algorithm/ExplicitEnumerationAlgorithm.hpp"
#include "Domain/Algorithm/ImplicitEnumerationAlgorithm.hpp"

#include "Domain/Algorithm/GradientDescentAlgorithm.hpp"
#include "Domain/Algorithm/Neighborhood.hpp"

#include <iostream>
#include <chrono>

int main()
{
    Yolo::Core::initialize();

    Yolo::GraphFileRepository graphRepository;

    std::optional<Yolo::Graph> g = graphRepository.load("Instances/quinzeSommets.txt");
    std::chrono::high_resolution_clock::time_point a,b;

    int nbClasses = 4;
    int epsilon = 0;
    
    Yolo::ExplicitEnumerationAlgorithm EE = Yolo::ExplicitEnumerationAlgorithm(g.value(), nbClasses, [](std::vector<int> array, int nbVertices, int to) { return Yolo::cEqualDelta(array, nbVertices, to, 1); });
    Yolo::ImplicitEnumerationAlgorithm IE = Yolo::ImplicitEnumerationAlgorithm(g.value(), nbClasses, [](std::vector<int> array, int nbVertices, int to) { return Yolo::cEqualDelta(array, nbVertices, to, 1); });
    Yolo::GradientDescentAlgorithm GD = Yolo::GradientDescentAlgorithm(g.value(), nbClasses, epsilon, Yolo::pickNDrop , 
        [](std::vector<int> array, int nbVertices, int to) { return Yolo::cEqualDelta(array, nbVertices, to, 1); });

    if(g.has_value()){
        std::vector<Yolo::Algorithm*> algos = std::vector<Yolo::Algorithm*>(
            {
                // &EE,
                &IE, 
                &GD
            });
        
        for (auto & algorithm: algos)
        {
            a = std::chrono::high_resolution_clock::now();
            Yolo::Solution sol = algorithm->solve();
            b = std::chrono::high_resolution_clock::now();
            std::cout << "\n"<< algorithm->getName()<<":\n  Best solution: " << sol.toString() << "\n    cost: " << g.value().getSolutionCost(sol) << "\n    Found in:" << (b - a).count()<<" microseconds \n";
        }
    }
    Yolo::Core::shutdown();

    return 0;
}