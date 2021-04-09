#pragma once

#include "Domain/Neighborhood/Neighborhood.hpp"

namespace Yolo
{
    class PickNDropNeighborhood : public Neighborhood
    {
    public:
        PickNDropNeighborhood() {}

        virtual std::vector<Solution> generateAll(const Solution& solution) const override;
        virtual std::vector<std::pair<Solution, double>> generateAll(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const override;

        virtual Solution generateRandom(const Solution& solution) const override;
        virtual std::pair<Solution, double> generateRandom(const Solution& sollution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const override;

        virtual std::pair<Solution, double> generateBest(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const override;
        virtual std::pair<Solution, double> generateBest(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion, const std::list<Solution>& exceptions) const override;
    };
} // namespace Yolo
