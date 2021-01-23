#pragma once

#include <vector>

namespace Yolo
{
    class Solution
    {
    public:
        Solution(int nbVertex, int nbClasses) : mSolution(std::vector<int>(nbVertex)) {}

        inline void setVertexClass(int vertex, int vertexClass) { mSolution[vertex] = vertexClass; }
        inline int getVertexClass(int vertex) const { return mSolution[vertex]; }
        inline int getNbClasses() const { return mNbClasses; }
    private:
        std::vector<int> mSolution;
        int mNbClasses;
    };
}