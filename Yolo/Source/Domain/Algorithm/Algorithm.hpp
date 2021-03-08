#pragma once

#include "Domain/Graph/Graph.hpp"

#include "Domain/Criterion/Criterion.hpp"

#include "Domain/Solution/Solution.hpp"

namespace Yolo
{
    class Algorithm
    {
    public:
        Algorithm(const Graph& graph, int nbClasses);

        virtual std::string getName() const = 0;
        virtual inline std::string getDetails() const { return std::string(); };

        inline void setCriterion(const Criterion* criterion) { mCriterion = criterion; }

        virtual std::optional<Solution> solve() = 0;

    protected:
        Graph mGraph;
        const Criterion* mCriterion;

        const int mNbClasses;
    };
} // namespace Yolo
