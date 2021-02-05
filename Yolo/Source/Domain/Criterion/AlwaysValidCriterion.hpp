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

        virtual inline bool evaluate(Graph graph, Solution solution) const override { return true; };
    };
} // namespace Yolo