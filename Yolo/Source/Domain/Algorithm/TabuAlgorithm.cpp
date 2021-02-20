#include "TabuAlgorithm.hpp"

#include "Core/Logger/Logger.hpp"

namespace Yolo
{
    Solution TabuAlgorithm::solve()
    {
        Solution initalSolution = generateValidSolution();
        return solve(initalSolution);
    }

    Solution TabuAlgorithm::solve(Solution initialSolution)
    {
        Solution veryBest = initialSolution;

        mActualSolution = initialSolution;

        if (!mCriterion->evaluate(mGraph, mActualSolution))
        {
            YOLO_WARN("Initial solution in tabu isn't valid.");
        }

        mActualSolutionCost = mGraph.getSolutionCost(initialSolution);
        double veryBestCost = mActualSolutionCost;

        // Solution bestNeighbor = mNeighborhood->generateBestWithExceptions(mGraph, mTabu, mCriterion, initialSolution);
        Solution bestNeighbor = mNeighborhood->generateBest(mGraph, mCriterion, initialSolution);
        double bestNeighborCost = mGraph.getSolutionCost(bestNeighbor);
        
        // YOLO_INFO("{0} {1}",bestNeighbor.toString(), bestNeighborCost);

        int iter = 0;
        while (iter < mIterMax)
        {
            ++iter;

            if(mEach || bestNeighborCost >= mActualSolutionCost){
                mTabu.push_back(bestNeighbor);
            }

            if(mTabu.size() > mTabuSize){
                mTabu.pop_front();
            }

            mActualSolution = bestNeighbor;
            mActualSolutionCost = bestNeighborCost;

            if(mActualSolutionCost < veryBestCost){
                veryBest = mActualSolution;
                veryBestCost = mActualSolutionCost;
            }

            bestNeighbor = mNeighborhood->generateBestWithExceptions(mGraph, mTabu, mCriterion, mActualSolution);
            bestNeighborCost = mGraph.getSolutionCost(bestNeighbor);

        }
        if(bestNeighborCost < veryBestCost){
                veryBest = bestNeighbor;
                veryBestCost = bestNeighborCost;
        }
        return veryBest;
    }

    Solution TabuAlgorithm::generateValidSolution()
    {
        Solution solution = Solution(mGraph.getNbVertices(), mNbClasses);
        for (int i = 0; i < mGraph.getNbVertices(); i++)
        {
            solution.setVertexClass(i, i % mNbClasses);
        }
        return solution;
    }
} // namespace Yolo
