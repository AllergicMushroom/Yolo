#include "GradientDescentAlgorithm.hpp"

#include "Core/Logger/Logger.hpp"

namespace Yolo
{
    Solution GradientDescentAlgorithm::solve()
    {
        Solution initalSolution = generateValidSolution();
        return solve(initalSolution);
    }

    Solution GradientDescentAlgorithm::solve(Solution initialSolution)
    {
        mActualSolution = initialSolution;

        if (!mCriterion->evaluate(mGraph, mActualSolution))
        {
            YOLO_WARN("Initial solution in gradient descent isn't valid.");
        }

        mActualSolutionCost = mGraph.getSolutionCost(initialSolution);

        Solution bestNeighbor = mNeighborhood->generateBest(mGraph, mCriterion, initialSolution);
        double bestNeighborCost = mGraph.getSolutionCost(bestNeighbor);

        while (mActualSolutionCost - bestNeighborCost > mEpsilon)
        {
            mActualSolution = bestNeighbor;
            mActualSolutionCost = bestNeighborCost;

            bestNeighbor = mNeighborhood->generateBest(mGraph, mCriterion, mActualSolution);
            bestNeighborCost = mGraph.getSolutionCost(bestNeighbor);
        }

        return mActualSolution;
    }

    Solution GradientDescentAlgorithm::generateValidSolution()
    {
        Solution solution = Solution(mGraph.getNbVertices(), mNbClasses);
        for (int i = 0; i < mGraph.getNbVertices(); i++)
        {
            solution.setVertexClass(i, i % mNbClasses);
        }
        return solution;
    }
} // namespace Yolo
