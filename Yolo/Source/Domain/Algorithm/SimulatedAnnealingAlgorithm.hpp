#pragma once

#include "Domain/Algorithm/Algorithm.hpp"

#include "Domain/Neighborhood/Neighborhood.hpp"

namespace Yolo
{
    class SimulatedAnnealingAlgorithm : public Algorithm
    {
    public:
        SimulatedAnnealingAlgorithm(const Graph& graph, int nbClasses);

        virtual inline std::string getName() const override { return "Simulated annealing"; }
        virtual std::string getDetails() const override;

        inline void setNeighborhood(std::shared_ptr<const Neighborhood> neighborhood) { mNeighborhood = neighborhood; }
        inline void setMaxIterations(int maxIterations) { mMaxIterations = std::max(1, maxIterations); }
        inline void setStartingTemperature(double temperature) { mTemperature = temperature; }

        virtual std::optional<Solution> solve() override;

    private:
        std::shared_ptr<const Neighborhood> mNeighborhood;

        int mMaxIterations;

        double mTemperature; // Todo: We can improve this.
    };
} // namespace Yolo
