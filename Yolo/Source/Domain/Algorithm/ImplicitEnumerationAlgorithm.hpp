#pragma once

#include "Algorithm.hpp"

namespace Yolo
{
    class ImplicitEnumerationAlgorithm : public Algorithm
    {
    public:
        ImplicitEnumerationAlgorithm(const Graph& graph, int nbClasses, const Criterion* criterion)
            : Algorithm(graph, nbClasses, criterion), mBestSolution(Solution(mGraph.getNbVertices(), mNbClasses)) 
            {
                mIsBestSolutionValid = criterion->evaluate(mGraph, mBestSolution);
            }

        virtual Solution solve() override;

        virtual inline std::string getName() override { return "Implicit Enumeration"; }
        virtual std::string getDetail() override { return ""; }

    private:
        void enumerateFrom(Solution solution, int vertex, double cost);

    private:
        Solution mBestSolution;
        double mBestCost;
        bool mIsBestSolutionValid;
    };
} // namespace Yolo
