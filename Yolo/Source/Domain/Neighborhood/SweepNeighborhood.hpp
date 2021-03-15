#pragma once

#include "Domain/Neighborhood/Neighborhood.hpp"

namespace Yolo
{
    class SweepNeighborhood : public Neighborhood
    {
    public:
        SweepNeighborhood() {}

        virtual std::vector<Solution> generateAll(const Solution& solution) const override;
        virtual std::vector<Solution> generateAll(const Solution& solution, const Graph& graph, std::shared_ptr<const Criterion> criterion) const override;

        virtual Solution generateRandom(const Solution& solution) const override;
        virtual Solution generateRandom(const Solution& solution, const Graph& graph, std::shared_ptr<const Criterion> criterion) const override;

        virtual Solution generateBest(const Solution& solution, const Graph& graph, std::shared_ptr<const Criterion> criterion) const override;
        virtual Solution generateBest(const Solution& solution, const Graph& graph, std::shared_ptr<const Criterion> criterion, const std::list<Solution>& exceptions) const override;
    };
} // namespace Yolo
