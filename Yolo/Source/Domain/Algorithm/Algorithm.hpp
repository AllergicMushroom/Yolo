#pragma once

#include "Domain/Graph/Graph.hpp"

#include "Domain/Criterion/Criterion.hpp"

#include "Domain/Solution/Solution.hpp"

namespace Yolo
{
    class Algorithm
    {
    public:
        Algorithm(const Graph& graph, int nbClasses, const Criterion* criterion)
            : mGraph(graph), mCriterion(criterion), mNbClasses(nbClasses)
        {
        }

        virtual Solution solve() = 0;

        virtual std::string getName() = 0;
        virtual std::string getDetail() = 0;

    protected:
        Graph mGraph;
        const Criterion* mCriterion;

        const int mNbClasses;
    };
} // namespace Yolo
