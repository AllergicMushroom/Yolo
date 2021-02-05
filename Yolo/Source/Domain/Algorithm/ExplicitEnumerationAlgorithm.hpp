#pragma once

#include "Algorithm.hpp"

namespace Yolo
{
    class ExplicitEnumerationAlgorithm : public Algorithm
    {
    public:
        ExplicitEnumerationAlgorithm(const Graph& graph, int nbClasses, const Criterion* criterion)
            : Algorithm(graph, nbClasses, criterion), mBestSolution(Solution(mGraph.getNbVertices(), mNbClasses))
        {
        }

        virtual Solution solve() override;

        virtual inline std::string getName() override { return "Explicit Enumeration"; }

    private:
        void enumerateFromVertex(Solution solution, int vertex);

    protected:
        Solution mBestSolution;
    };
} // namespace Yolo
