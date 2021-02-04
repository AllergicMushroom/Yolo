#include "Core/Core.hpp"

#include "Technical/Repositories/GraphFileRepository.hpp"

#include "Domain/Algorithm/Criterion.hpp"
#include "Domain/Algorithm/ExplicitEnumerationAlgorithm.hpp"
#include "Domain/Algorithm/ImplicitEnumerationAlgorithm.hpp"

#include "Domain/Algorithm/GradientDescentAlgorithm.hpp"
#include "Domain/Algorithm/Neighborhood.hpp"

#include <iostream>


int main()
{
    printf("start\n");
    Yolo::Core::initialize();

    Yolo::GraphFileRepository graphRepository;

    std::optional<Yolo::Graph> g = graphRepository.load("Instances/quatreSommets.txt");
    if(g.has_value()){

        Yolo::ExplicitEnumerationAlgorithm algo = Yolo::ExplicitEnumerationAlgorithm(g.value(), 2, Yolo::cEqual);
        Yolo::Solution sol = algo.solve();
        std::cout << "\nExplicit Enumeration:\nBest solution: " << sol.toString() << " cost: " << g.value().getSolutionCost(sol) << "\n";
        
        Yolo::ImplicitEnumerationAlgorithm algo2 = Yolo::ImplicitEnumerationAlgorithm(g.value(), 2, Yolo::cEqual);
        Yolo::Solution sol2 = algo2.solve();
        std::cout << "\nImplicit Enumeration:\nBest solution: " << sol2.toString() << " cost: " << g.value().getSolutionCost(sol2) << "\n";

        Yolo::GradientDescentAlgorithm algo3 = Yolo::GradientDescentAlgorithm(g.value(), 2, 0,Yolo::pickNDrop , [](std::vector<int> array, int nbVertices, int to) { return Yolo::cEqualDelta(array, nbVertices, to, 1); });
        Yolo::Solution sol3 = algo3.solve();
        std::cout << "\nGradient Descent :\nBest solution: " << sol3.toString() << " cost: " << g.value().getSolutionCost(sol3) << "\n";
    }
    Yolo::Core::shutdown();

    return 0;
}
