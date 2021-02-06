#include "ImplicitEnumerationAlgorithm.hpp"

namespace Yolo
{
    Solution ImplicitEnumerationAlgorithm::solve()
    {
        enumerateFrom(mBestSolution, 0, 0);
        return mBestSolution;
    }

    void ImplicitEnumerationAlgorithm::enumerateFrom(Solution solution, int vertex, double cost)
    {
        if (vertex == mGraph.getNbVertices())
        {
            if (mCriterion->evaluate(mGraph, solution))
            {
                // Todo: We shouldn't need to evaluate mBestSolution at every call. Maybe get a random initial solution
                if (!mCriterion->evaluate(mGraph, mBestSolution))
                {
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

                // TODO
                /*if (mGraph.isPartialSolutionValid(sol, mCriterion, from))
                    enumerateFrom(sol, from + 1);*/
            }
        }
    }
} // namespace Yolo
