#include "Solution.hpp"

static constexpr int sNoClass = -1;

namespace Yolo
{
    Solution::Solution(int nbVertices, int nbClasses)
        : mSolution(std::vector<int>(nbVertices, sNoClass)), mClassCardinals(std::vector<int>(nbClasses, 0)), mNbVertices(nbVertices), mNbClasses(nbClasses)
    {
    }

    void Solution::setVertexClass(int vertex, int vertexClass)
    {
        if (mSolution[vertex] != sNoClass)
        {
            --mClassCardinals[mSolution[vertex]];
        }

        mSolution[vertex] = vertexClass;
        ++mClassCardinals[mSolution[vertex]];
    }
} // namespace Yolo
