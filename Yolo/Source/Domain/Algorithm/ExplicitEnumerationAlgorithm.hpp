#pragma once

#include "Algorithm.hpp"

namespace Yolo
{
    class ExplicitEnumerationAlgorithm : public Algorithm
    {
    public:
        ExplicitEnumerationAlgorithm(const Graph& graph, int nbClasses) : Algorithm(graph, nbClasses) {}

        virtual Solution solve() override;
    };
}