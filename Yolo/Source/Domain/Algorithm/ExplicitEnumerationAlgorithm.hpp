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

        virtual Solution solve() override;

        virtual inline std::string getName() override { return "Explicit Enumeration"; }
        virtual std::string getDetail() override { return ""; }

    private:
        void enumerateFrom(Solution solution, int vertex, double cost);

    protected:
        Solution mBestSolution;
        double mBestCost;
        bool mIsBestSolutionValid;
    };
} // namespace Yolo
