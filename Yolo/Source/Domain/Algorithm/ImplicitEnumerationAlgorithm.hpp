#pragma once

#include "Algorithm.hpp"

namespace Yolo
{
    class ImplicitEnumerationAlgorithm : public Algorithm
    {
    public:
        ImplicitEnumerationAlgorithm(const Graph& graph, int nbClasses, bool (*criterion)(std::vector<int>, int, int)) : Algorithm(graph, nbClasses, criterion), mBest(Solution(mGraph.getNbVertices(), mNbClasses)){}

        virtual Solution solve() override;
        void enumerateFrom(Solution sol, int from);
        void compareBest(Solution sol);

    private:
        Solution mBest;
    };
}