#pragma once

#include "Domain/Graph/Graph.hpp"

#include "Domain/Solution/Solution.hpp"

namespace Yolo
{
    class Algorithm
    {
    public:
        Algorithm(const Graph& graph, int nbClasses, bool (*criterion)(std::vector<int>, int, int)) : mGraph(graph), mNbClasses(nbClasses), mCriterion(criterion) {}

        virtual Solution solve() = 0;

    protected:
        Graph mGraph;
        
        const int mNbClasses;
        bool (*mCriterion)(std::vector<int>, int, int);
    };
}