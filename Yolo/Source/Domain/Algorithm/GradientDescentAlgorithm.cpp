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

        Solution bestNeighbor = findBestNeighbor(mActualSolution, true);
        double bestNeighborCost = mGraph.getSolutionCost(bestNeighbor);

        while (mActualSolutionCost - bestNeighborCost > mEpsilon)
        {
            mActualSolution = bestNeighbor;
            mActualSolutionCost = bestNeighborCost;

            bestNeighbor = findBestNeighbor(mActualSolution, true);
            bestNeighborCost = mGraph.getSolutionCost(bestNeighbor);
        }

        return mActualSolution;
    }

    Solution GradientDescentAlgorithm::findBestNeighbor(Solution solution, bool real)
    {
        std::vector<Solution> neighborhood = mNeigh(solution);
        double cost;
        Solution best = solution;
        bool isSet = false;

        for (unsigned int i = 0; i < neighborhood.size(); i++)
        {
            if (!real || mCriterion->evaluate(mGraph, neighborhood[i]))
            {
                if (!isSet)
                {
                    cost = mGraph.getSolutionCost(neighborhood[i]);
                    best = neighborhood[i];
                    isSet = true;
                }
                else
                {
                    double newCost = mGraph.getSolutionCost(neighborhood[i]);
                    if (newCost < cost)
                    {
                        cost = newCost;
                        best = neighborhood[i];
                    }
                }
            }
        }
        return best;
    }

    // TODO: We don't actually know if it's valid.
    Solution GradientDescentAlgorithm::generateValidSolution()
    {
        Solution sol = Solution(mGraph.getNbVertices(), mNbClasses);
        for (int i = 0; i < mGraph.getNbVertices(); i++)
        {
            sol.setVertexClass(i, i % mNbClasses);
        }
        return sol;
    }
} // namespace Yolo
