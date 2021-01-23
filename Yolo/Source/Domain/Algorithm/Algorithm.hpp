#pragma once

#include "Domain/Graph/Graph.hpp"

#include "Domain/Solution/Solution.hpp"

namespace Yolo
{
    class Algorithm
    {
    public:
        Algorithm(const Graph& graph, int nbClasses) : mGraph(graph), mNbClasses(nbClasses) {}

        virtual Solution solve() = 0;

    protected:
        Graph mGraph;

        const int mNbClasses;
    };
}