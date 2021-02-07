#pragma once

#include "Domain/Neighborhood/Neighborhood.hpp"

namespace Yolo
{
    class SwapNeighborhood : public Neighborhood
    {
    public:
        SwapNeighborhood() {}

        virtual std::vector<Solution> generateAll(const Solution& solution) const override;
        virtual Solution generateBest(const Graph& g, const Criterion* criterion, const Solution& solution) const override;
    };
} // namespace Yolo
