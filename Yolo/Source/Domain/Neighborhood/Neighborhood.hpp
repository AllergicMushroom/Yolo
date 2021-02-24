#pragma once

#include <cmath>

#include "Domain/Criterion/Criterion.hpp"
#include "Domain/Graph/Graph.hpp"
#include "Domain/Solution/Solution.hpp"
#include <list>
#include <iterator>

#include "Core/Logger/Logger.hpp"

namespace Yolo
{
    class Neighborhood
    {
    public:
        Neighborhood() {}

        virtual std::vector<Solution> generateAll(const Solution& solution) const = 0;
        virtual Solution generateBest(const Graph& g, const Criterion* criterion, const Solution& solution) const = 0;
        virtual Solution generateBestWithExceptions(const Graph& g, const std::list<Solution> &Exceptions, const Criterion* criterion, const Solution& solution) const = 0;
    };
} // namespace Yolo
