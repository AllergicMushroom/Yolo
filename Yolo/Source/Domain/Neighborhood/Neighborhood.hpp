#pragma once

#include "Domain/Criterion/Criterion.hpp"
#include "Domain/Graph/Graph.hpp"
#include "Domain/Solution/Solution.hpp"

//#include <cmath>
//#include <iterator>
#include <list>

namespace Yolo
{
    class Neighborhood
    {
    public:
        Neighborhood() {}

        virtual std::vector<Solution> generateAll(const Solution& solution) const = 0;
        virtual std::vector<Solution> generateAll(const Solution& solution, const Graph& graph, const Criterion* criterion) const = 0;

        virtual Solution generateRandom(const Solution& solution) const = 0;
        virtual Solution generateRandom(const Solution& sollution, const Graph& graph, const Criterion* criterion) const = 0;

        virtual Solution generateBest(const Solution& solution, const Graph& graph, const Criterion* criterion) const = 0;
        virtual Solution generateBest(const Solution& solution, const Graph& graph, const Criterion* criterion, const std::list<Solution>& exceptions) const = 0;
    };
} // namespace Yolo
