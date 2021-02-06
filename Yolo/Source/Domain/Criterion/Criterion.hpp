#pragma once

#include "Domain/Graph/Graph.hpp"

#include "Domain/Solution/Solution.hpp"

namespace Yolo
{
    class Criterion
    {
    public:
        Criterion() {}

        virtual bool evaluate(const Graph& graph, const Solution& solution) const = 0;
    };
} // namespace Yolo
