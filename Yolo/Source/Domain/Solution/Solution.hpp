#pragma once

#include <vector>
#include <iostream> 
#include <string> 
namespace Yolo
{
    class Solution
    {
    public:
        Solution(int nbVertex, int nbClasses) : mSolution(std::vector<int>(nbVertex)) {
            mNbClasses = nbClasses;
        }

        inline void setVertexClass(int vertex, int vertexClass) { mSolution[vertex] = vertexClass; }
        inline int getVertexClass(int vertex) const { return mSolution[vertex]; }
        inline int getNbClasses() const { return mNbClasses; }
        Solution clone(){
            Solution clone = Solution(mSolution.size(), mNbClasses);
            for (unsigned int i = 0; i < mSolution.size(); i++)
            {
                clone.setVertexClass(i, mSolution[i]);
            }
            return clone;
        }
        std::string toString(){
            std::string str = "";
            for (unsigned int i = 0; i < mSolution.size(); ++i)
            {
                str += std::to_string(mSolution[i]) +" ";
            }
            return str;
        }
    private:
        std::vector<int> mSolution;
        int mNbClasses;
    };
}