#pragma once

#include <string>
#include <vector>

static constexpr int sVertexNoClass = -1;

namespace Yolo
{
    class Solution
    {
    public:
        Solution()
            : Solution(0, 0) {}

        Solution(int nbVertices, int nbClasses);

        inline int getNbVertices() const { return static_cast<int>(mSolution.size()); }
        inline int getNbClasses() const { return static_cast<int>(mClassCardinals.size()); }

        void setVertexClass(int vertex, int vertexClass);
        int getVertexClass(int vertex) const;

        int getClassCardinal(int c) const;

        bool operator==(const Solution& solution) const;
        bool operator!=(const Solution& solution) const;

        std::string toString() const;

    private:
        std::vector<int> mSolution;
        std::vector<int> mClassCardinals;
    };
} // namespace Yolo
