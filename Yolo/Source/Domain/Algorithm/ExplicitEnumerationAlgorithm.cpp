#include "Domain/Algorithm/ExplicitEnumerationAlgorithm.hpp"

namespace Yolo
{
    Solution ExplicitEnumerationAlgorithm::solve()
    {
        enumerateFromVertex(mBestSolution, 0);

        return mBestSolution;
    }

    void ExplicitEnumerationAlgorithm::enumerateFromVertex(Solution solution, int vertex)
    {
        if (vertex == mGraph.getNbVertices())
        {
            if (mCriterion->evaluate(mGraph, solution))
            {
                if (!mIsBestSolutionValid)
                {
                    mBestSolution = solution;
                    mIsBestSolutionValid = true;
                }
                else if (mGraph.getSolutionCost(solution) < mGraph.getSolutionCost(mBestSolution))
                {
                    mBestSolution = solution;
                }
            }
        }
        else
        {
            /* We take the minimum between vertex and mNbClasses-1 to eliminate symmetries. For example, <0, 2, 1> is equivalent to <0, 1, 2> */
            for (int i = 0; i <= std::min(vertex, mNbClasses - 1); ++i)
            {
                solution.setVertexClass(vertex, i);
                enumerateFromVertex(solution, vertex + 1);
            }
        }
    }
} // namespace Yolo
