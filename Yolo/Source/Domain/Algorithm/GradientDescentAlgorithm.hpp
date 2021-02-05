#pragma once

#include "Algorithm.hpp"

namespace Yolo
{
    class GradientDescentAlgorithm : public Algorithm
    {
    public:
        GradientDescentAlgorithm(const Graph& graph, int nbClasses, double epsilon, std::vector<Solution> (*neigh)(Solution sol), const Criterion* criterion)
            : Algorithm(graph, nbClasses, criterion), mActualSolution(Solution(mGraph.getNbVertices(), mNbClasses))
        {
            mNeigh = neigh;
            mEpsilon = epsilon;
        }

        virtual Solution solve() override;

        std::string getName() override { return "Gradient Descent"; }

    private:
        Solution solve(Solution initialSolution);

        Solution findBestNeighbor(Solution sol, bool real);
        Solution generateValidSolution();

    protected:
        std::vector<Solution> (*mNeigh)(Solution sol);

        Solution mActualSolution;
        double mActualSolutionCost = 0.0f;

        double mEpsilon;
    };
} // namespace Yolo
