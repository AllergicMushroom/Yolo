#pragma once

#include "Algorithm.hpp"

namespace Yolo
{
    class ImplicitEnumerationAlgorithm : public Algorithm
    {
    public:
        ImplicitEnumerationAlgorithm(const Graph& graph, int nbClasses, const Criterion* criterion)
            : Algorithm(graph, nbClasses, criterion), mBestSolution(Solution(mGraph.getNbVertices(), mNbClasses)) {}

        virtual Solution solve() override;

        virtual inline std::string getName() override { return "Implicit Enumeration"; }

    private:
        void enumerateFrom(Solution solution, int vertex);

    private:
        Solution mBestSolution;
    };
} // namespace Yolo
