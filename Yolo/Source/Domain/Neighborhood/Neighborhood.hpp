#pragma once

#include <cmath>

#include "Domain/Solution/Solution.hpp"
#include "Domain/Graph/Graph.hpp"
#include "Domain/Criterion/Criterion.hpp"

namespace Yolo
{
    class Neighborhood
    {
    public:
        Neighborhood() {}

        virtual std::vector<Solution> generate(Solution solution) const = 0;
        virtual Solution getBest(Graph g, const Criterion* criterion, Solution solution) const = 0;
    };
} // namespace Yolo
