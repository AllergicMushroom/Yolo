#include "Domain/Algorithm/ExplicitEnumerationAlgorithm.hpp"

namespace Yolo
{
    Solution ExplicitEnumerationAlgorithm::solve()
    {
        enumerateFrom(mBestSolution, 0, 0);
        return mBestSolution;
    }

    void ExplicitEnumerationAlgorithm::enumerateFrom(Solution solution, int vertex, double cost)
    {
        if (vertex == mGraph.getNbVertices())
        {
            if (mCriterion->evaluate(mGraph, solution))
            {
                if (!mIsBestSolutionValid)
                {
                    mIsBestSolutionValid = true;
                    mBestSolution = solution;
                    mBestCost = cost;
                }
                else if (cost < mBestCost)
                {
                    mBestSolution = solution;
                    mBestCost = cost;
                }
            }
        }
        else
        {
            for (int i = 0; i <= std::min(vertex, mNbClasses - 1); ++i)
            {
                cost = cost + mGraph.getSolutionCostDifference(solution, vertex, i);
                solution.setVertexClass(vertex, i);
                enumerateFrom(solution, vertex + 1, cost);

                // Todo
                /*if (mGraph.isPartialSolutionValid(sol, mCriterion, from))
                    enumerateFrom(sol, from + 1);*/
            }
        }
    }
} // namespace Yolo
