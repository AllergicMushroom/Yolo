#include "TabuListAlgorithm.hpp"

#include "Core/Logger/Logger.hpp"

#include "Domain/Neighborhood/PickNDropNeighborhood.hpp"

#include <sstream>

static const Yolo::PickNDropNeighborhood sDefaultNeighborhood;
static constexpr int sDefaultMaxIterations = 1000;
static constexpr int sDefaultTabuListSize = 100;
static constexpr bool sDefaultStoreAll = true;

namespace Yolo
{
    TabuListAlgorithm::TabuListAlgorithm(const Graph& graph, int nbClasses)
        : Algorithm(graph, nbClasses)
    {
        mNeighborhood = std::make_shared<PickNDropNeighborhood>(sDefaultNeighborhood);

        mTabuListSize = sDefaultTabuListSize;
        mTabuList = std::list<Solution>();

        mStoreAll = sDefaultStoreAll;
        mMaxIterations = sDefaultMaxIterations;
    }

    std::string TabuListAlgorithm::getDetails() const
    {
        std::stringstream ss;
        ss << "\n\tNumber of iterations: " << mMaxIterations;
        ss << "\n\tTabu list size: " << mTabuListSize;
        ss << "\n\tAre we storing potentially better solutions: " << (mStoreAll ? "Yes" : "No");

        return ss.str();
    }

    std::optional<Solution> TabuListAlgorithm::solve()
    {
        std::optional<Solution> opt = mCriterion->generateInitialSolution(mGraph, mNbClasses);
        if (opt)
        {
            return solve(*opt);
        }

        return std::nullopt;
    }

    Solution TabuListAlgorithm::solve(Solution initialSolution)
    {
        mActualSolution = initialSolution;
        mActualSolutionCost = mGraph.getSolutionCost(initialSolution);

        Solution veryBest = initialSolution;
        double veryBestCost = mActualSolutionCost;

        // Solution bestNeighbor = mNeighborhood->generateBestWithExceptions(mGraph, mTabu, mCriterion, initialSolution);
        auto [bestNeighbor, bestNeighborCost] = mNeighborhood->generateBest(mActualSolution, mActualSolutionCost, mGraph, mCriterion);

        int iter = 0;
        while (iter < mMaxIterations)
        {
            ++iter;

            if (mStoreAll || bestNeighborCost >= mActualSolutionCost)
            {
                mTabuList.push_back(bestNeighbor);
                if (mTabuList.size() > mTabuListSize)
                {
                    mTabuList.pop_front();
                }
            }

            mActualSolution = bestNeighbor;
            mActualSolutionCost = bestNeighborCost;

            if (mActualSolutionCost < veryBestCost)
            {
                veryBest = mActualSolution;
                veryBestCost = mActualSolutionCost;
            }

            auto [newBestNeighbor, newBestNeighborCost] = mNeighborhood->generateBest(mActualSolution, mActualSolutionCost, mGraph, mCriterion, mTabuList);
            bestNeighbor = newBestNeighbor;
            bestNeighborCost = newBestNeighborCost;
        }

        if (bestNeighborCost < veryBestCost)
        {
            veryBest = bestNeighbor;
            veryBestCost = bestNeighborCost;
        }

        return veryBest;
    }
} // namespace Yolo
