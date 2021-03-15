#pragma once

#include "Domain/Graph/Graph.hpp"

#include "Domain/Solution/Solution.hpp"

#include <optional>

namespace Yolo
{
    class Criterion
    {
    public:
        Criterion() {}

        virtual bool evaluate(const Graph& graph, const Solution& solution, bool isPartial = false) const = 0;

        virtual std::optional<Solution> generateInitialSolution(const Graph& graph, int nbClasses) const = 0;
    };
} // namespace Yolo
