#pragma once

#include "Domain/Neighborhood/Neighborhood.hpp"

namespace Yolo
{
    class SweepNeighborhood : public Neighborhood
    {
    public:
        SweepNeighborhood() {}

        virtual std::vector<Solution> generateAll(const Solution& solution) const override;

        virtual Solution generateBest(const Graph& graph, const Criterion* criterion, const Solution& solution) const override;
        virtual Solution generateBestWithExceptions(const Graph& g, const std::list<Solution> &Exceptions, const Criterion* criterion, const Solution& solution) const override;
    };
} // namespace Yolo
