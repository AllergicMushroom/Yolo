#pragma once

#include <vector>

namespace Yolo
{
    class Solution
    {
    public:
        Solution(int nbClasses) : mSolution(std::vector<int>(nbClasses)) {}

        inline void setVertexClass(int vertex, int vertexClass) { mSolution[vertex] = vertexClass; }
        inline int getVertexClass(int vertex) const { return mSolution[vertex]; }

    private:
        std::vector<int> mSolution;
    };
}