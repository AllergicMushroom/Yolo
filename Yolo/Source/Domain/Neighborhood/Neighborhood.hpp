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
        virtual std::vector<Solution> generateAll(const Solution& solution, const Graph& graph, std::shared_ptr<const Criterion> criterion) const = 0;

        virtual Solution generateRandom(const Solution& solution) const = 0;
        virtual Solution generateRandom(const Solution& sollution, const Graph& graph, std::shared_ptr<const Criterion> criterion) const = 0;

        virtual Solution generateBest(const Solution& solution, const Graph& graph, std::shared_ptr<const Criterion> criterion) const = 0;
        virtual Solution generateBest(const Solution& solution, const Graph& graph, std::shared_ptr<const Criterion> criterion, const std::list<Solution>& exceptions) const = 0; // Todo: std::vector instead of list
    };
} // namespace Yolo
