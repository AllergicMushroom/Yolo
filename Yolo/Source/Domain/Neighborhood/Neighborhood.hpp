#pragma once

#include "Domain/Criterion/Criterion.hpp"
#include "Domain/Graph/Graph.hpp"
#include "Domain/Solution/Solution.hpp"

#include <list>
#include <memory>

namespace Yolo
{
    class Neighborhood
    {
    public:
        Neighborhood() {}

        virtual std::vector<Solution> generateAll(const Solution& solution) const = 0;
        virtual std::vector<std::pair<Solution, double>> generateAll(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const = 0;

        virtual Solution generateRandom(const Solution& solution) const = 0;
        virtual std::pair<Solution, double> generateRandom(const Solution& sollution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const = 0;

        virtual std::pair<Solution, double> generateBest(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const = 0;
        virtual std::pair<Solution, double> generateBest(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion, const std::list<Solution>& exceptions) const = 0;
    };
} // namespace Yolo
