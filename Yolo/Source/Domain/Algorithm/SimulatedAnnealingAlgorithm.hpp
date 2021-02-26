#pragma once

#include "Domain/Algorithm/Algorithm.hpp"

#include "Domain/Neighborhood/Neighborhood.hpp"

namespace Yolo
{
    class SimulatedAnnealingAlgorithm : public Algorithm
    {
    public:
        SimulatedAnnealingAlgorithm(const Graph& graph, int nbClasses, const Criterion* criterion, const Neighborhood* neighborhood, int maxIterations);

        virtual inline std::string getName() const override { return "Simulated annealing"; }
        virtual std::string getDetails() const override;

        virtual std::optional<Solution> solve() override;

    private:
        const Neighborhood* mNeighborhood;

        int mMaxIterations;
    };
} // namespace Yolo
