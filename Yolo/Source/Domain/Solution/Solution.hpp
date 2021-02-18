#pragma once

#include <string>
#include <vector>

static constexpr int sVertexNoClass = -1;

namespace Yolo
{
    class Solution
    {
    public:
        Solution(int nbVertices, int nbClasses);

        inline int getNbVertices() const { return static_cast<int>(mSolution.size()); }
        inline int getNbClasses() const { return static_cast<int>(mClassCardinals.size()); }

        void setVertexClass(int vertex, int vertexClass);
        int getVertexClass(int vertex) const;

        int getClassCardinal(int c) const;

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
    };
} // namespace Yolo
