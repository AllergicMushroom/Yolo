#pragma once

#include <string>
#include <vector>

namespace Yolo
{
    class Solution
    {
    public:
        Solution(int nbVertices, int nbClasses);

        inline int getNbClasses() const { return mNbClasses; }
        inline int getNbVertices() const { return mNbVertices; }

        void setVertexClass(int vertex, int vertexClass);
        inline int getVertexClass(int vertex) const { return mSolution[vertex]; }

        inline int getClassCardinal(int c) const { return mClassCardinals[c]; }

        Solution clone()
        {
            Solution clone = Solution(static_cast<int>(mSolution.size()), mNbClasses);
            for (unsigned int i = 0; i < mSolution.size(); i++)
            {
                clone.setVertexClass(i, mSolution[i]);
            }
            clone.mNbClasses = mNbClasses;
            return clone;
        }

        std::string toString()
        {
            std::string str = "";
            for (unsigned int i = 0; i < mSolution.size(); ++i)
            {
                str += std::to_string(mSolution[i]) + " ";
            }
            return str;
        }

    private:
        std::vector<int> mSolution;
        std::vector<int> mClassCardinals;

        int mNbClasses;
        int mNbVertices;
    };
} // namespace Yolo
