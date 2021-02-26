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
            mIsBestSolutionValid = criterion->evaluate(mGraph, mBestSolution);
        }

        virtual inline std::string getName() const override { return "Explicit Enumeration"; }

        virtual std::optional<Solution> solve() override;

    private:
        void enumerateFrom(Solution solution, int vertex, double cost);

    private:
        Solution mBestSolution;
        double mBestCost;
        bool mIsBestSolutionValid;
    };
} // namespace Yolo
