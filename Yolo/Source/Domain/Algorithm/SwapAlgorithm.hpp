#pragma once

#include "Algorithm.hpp"

namespace Yolo
{
    class SwapAlgorithm : public Algorithm
    {
    public:
        SwapAlgorithm(const Graph& graph);

        virtual Solution solve() override;
    };
}