#pragma once

#include "Domain/Criterion/Criterion.hpp"

#include "Domain/Graph/Graph.hpp"

#include "Domain/Solution/Solution.hpp"

namespace Yolo
{
    class SimilarSizeCriterion : public Criterion
    {
    public:
        SimilarSizeCriterion(int maxSlack);
        SimilarSizeCriterion(double percentage);

        virtual bool evaluate(const Graph& graph, const Solution& solution) const override;

        virtual std::optional<Solution> generateInitialSolution(const Graph& graph, int nbClasses) const override;

    private:
        const bool mUseIntSlack;

        int mIntSlack = -1;
        double mPercentageSlack = -1;
    };
} // namespace Yolo
