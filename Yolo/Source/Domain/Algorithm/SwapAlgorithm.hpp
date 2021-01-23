#pragma once

#include "Algorithm.hpp"

namespace Yolo
{
    class SwapAlgorithm : public Algorithm
    {
    public:
        SwapAlgorithm(const Graph& graph, int nbClasses) : Algorithm(graph, nbClasses) {}

        virtual Solution solve() override;
    };
}