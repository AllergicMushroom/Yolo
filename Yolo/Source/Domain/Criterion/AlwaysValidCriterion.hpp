#pragma once

#include "Domain/Criterion/Criterion.hpp"

#include "Domain/Graph/Graph.hpp"

#include "Domain/Solution/Solution.hpp"

namespace Yolo
{
    class AlwaysValidCriterion : public Criterion
    {
    public:
        AlwaysValidCriterion()
            : Criterion() {}

        virtual inline bool evaluate(const Graph&, const Solution&, bool = false) const override { return true; };

        virtual std::optional<Solution> generateInitialSolution(const Graph& graph, int nbClasses) const override { return Solution(graph.getNbVertices(), nbClasses); };
    };
} // namespace Yolo
