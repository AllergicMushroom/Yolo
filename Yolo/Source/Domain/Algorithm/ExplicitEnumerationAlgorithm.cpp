#include "Domain/Algorithm/ExplicitEnumerationAlgorithm.hpp"

#include "Core/Logger/Logger.hpp"

namespace Yolo
{
    ExplicitEnumerationAlgorithm::ExplicitEnumerationAlgorithm(const Graph& graph, int nbClasses)
        : Algorithm(graph, nbClasses)
    {
        mBestSolution = Solution(mGraph.getNbVertices(), mNbClasses);
        mIsBestSolutionValid = false;
        mBestSolutionCost = mGraph.getSolutionCost(mBestSolution);
    }

    std::optional<Solution> ExplicitEnumerationAlgorithm::solve()
    {
        Solution solution = Solution(mGraph.getNbVertices(), mNbClasses);
        enumerateFrom(solution, 0, 0);

        if (!mCriterion->evaluate(mGraph, mBestSolution))
        {
            YOLO_DEBUG("ExplicitEnumerationAlgorithm::solve(): Infeasible instance.\n");
            return std::nullopt;
        }

        return mBestSolution;
    }

    void ExplicitEnumerationAlgorithm::enumerateFrom(Solution& solution, int vertex, double cost)
    {
        if (vertex == mGraph.getNbVertices())
        {
            if (mCriterion->evaluate(mGraph, solution))
            {
                if (!mIsBestSolutionValid)
                {
                    mIsBestSolutionValid = true;
                    mBestSolution = solution;
                    mBestSolutionCost = cost;
                }
                else
                {
                    if (cost < mBestSolutionCost)
                    {
                        mBestSolution = solution;
                        mBestSolutionCost = cost;
                    }
                }
            }
        }
        else
        {
            int previousClass = solution.getVertexClass(vertex);

            for (int i = 0; i <= std::min(vertex, mNbClasses - 1); ++i)
            {
                cost = cost + mGraph.getSolutionCostDifference(solution, vertex, i);
                solution.setVertexClass(vertex, i);
                enumerateFrom(solution, vertex + 1, cost);
            }

            if (previousClass >= 0)
            {
                solution.setVertexClass(vertex, previousClass);
            }
        }
    }
} // namespace Yolo
