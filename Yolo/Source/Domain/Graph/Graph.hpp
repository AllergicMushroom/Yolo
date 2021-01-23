#pragma once

#include <vector>

#include "Domain/Graph/Edge.hpp"

namespace Yolo
{
    class Graph
    {
    public:
        Graph(const std::vector<std::vector<Edge>>& adjacencyList);

        inline int getNbVertices() const { return static_cast<int>(mAdjacencyList.size()); }

        inline int getVertexDegree(int vertex) const { return mVertexDegrees[vertex]; }

        inline int getMaxDegree() const { return mMaxDegree; }
        inline int getMinDegree() const { return mMinDegree; }

        inline int getNbEdges() const { return mNbEdges; }

    private:
        std::vector<int> mVertexDegrees;
        int mMaxDegree;
        int mMinDegree;

        int mNbEdges;

        std::vector<std::vector<Edge>> mAdjacencyList;
    };
}