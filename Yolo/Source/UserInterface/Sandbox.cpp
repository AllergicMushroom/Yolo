#include "Core/Core.hpp"

#include "Technical/Repositories/GraphFileRepository.hpp"
#include "Domain/Algorithm/ExplicitEnumerationAlgorithm.hpp"
#include "Domain/Algorithm/Criterion.hpp"

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
        std::cout << "\nBest solution: " << sol.toString() << " cost: " << g.value().getSolutionCost(sol) << "\n";
    }
    Yolo::Core::shutdown();

    return 0;
}
