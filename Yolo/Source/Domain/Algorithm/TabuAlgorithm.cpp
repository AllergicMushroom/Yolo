#include "TabuAlgorithm.hpp"

#include "Core/Logger/Logger.hpp"

#include <sstream>

namespace Yolo
{
    TabuAlgorithm::TabuAlgorithm(const Graph& graph, int nbClasses, const Criterion* criterion, const Neighborhood* neighborhood, int tabuListSize, int maxIterations, bool storeAll, bool aspiration)
        : Algorithm(graph, nbClasses, criterion), mActualSolution(Solution(mGraph.getNbVertices(), mNbClasses))
    {
        mNeighborhood = neighborhood;

        mTabuList = std::list<Solution>();
        mTabuListSize = tabuListSize;

        mStoreAll = storeAll;
        mAspiration = aspiration;
        mMaxIterations = maxIterations;
    }

    std::optional<Solution> TabuAlgorithm::solve()
    {
        // Todo
        Solution initalSolution = generateValidSolution();
        return solve(initalSolution);
    }

    std::string TabuAlgorithm::getDetails() const
    {
        std::stringstream ss;
        ss << "\n\tNumber of iterations: " << mMaxIterations;
        ss << "\n\tTabu list size: " << mTabuListSize;
        ss << "\n\tAre we storing potentially better solutions: " << (mStoreAll ? "Yes" : "No");
        ss << "\n\tAspiration: " << (mAspiration ? "True" : "False");

        return ss.str();
    }

    Solution TabuAlgorithm::solve(Solution initialSolution)
    {
        Solution veryBest = initialSolution;

        mActualSolution = initialSolution;

        if (!mCriterion->evaluate(mGraph, mActualSolution))
        {
            // Todo
            YOLO_WARN("Initial solution in tabu isn't valid.");
        }

        mActualSolutionCost = mGraph.getSolutionCost(initialSolution);
        double veryBestCost = mActualSolutionCost;

        // Solution bestNeighbor = mNeighborhood->generateBestWithExceptions(mGraph, mTabu, mCriterion, initialSolution);
        Solution bestNeighbor = mNeighborhood->generateBest(initialSolution, mGraph, mCriterion);
        double bestNeighborCost = mGraph.getSolutionCost(bestNeighbor);

        // YOLO_INFO("{0} {1}",bestNeighbor.toString(), bestNeighborCost);

        int iter = 0;
        while (iter < mMaxIterations)
        {
            ++iter;

            if (mStoreAll || bestNeighborCost >= mActualSolutionCost)
            {
                mTabuList.push_back(bestNeighbor);
            }

            if (mTabuList.size() > mTabuListSize)
            {
                mTabuList.pop_front();
            }

            mActualSolution = bestNeighbor;
            mActualSolutionCost = bestNeighborCost;

            if (mActualSolutionCost < veryBestCost)
            {
                veryBest = mActualSolution;
                veryBestCost = mActualSolutionCost;
            }

            bestNeighbor = mNeighborhood->generateBest(mActualSolution, mGraph, mCriterion, mTabuList);
            bestNeighborCost = mGraph.getSolutionCost(bestNeighbor);
        }
        if (bestNeighborCost < veryBestCost)
        {
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
