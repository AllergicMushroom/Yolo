# include "Graph.hpp"

namespace Yolo
{
    Graph::Graph(const std::vector<std::vector<Edge>>& adjacencyList)
        : mAdjacencyList(std::vector<std::vector<Edge>>(adjacencyList))
    {
        mVertexDegrees = std::vector<int>(mAdjacencyList.size());
        mMaxDegree = static_cast<int>(mAdjacencyList[0].size());
        mMinDegree = static_cast<int>(mAdjacencyList[0].size());

        mNbEdges = 0;

        for (int i = 0; i < mAdjacencyList.size(); ++i)
        {
            mVertexDegrees[i] = static_cast<int>(mAdjacencyList[i].size());
            if (mVertexDegrees[i] > mMaxDegree)
                mMaxDegree = mVertexDegrees[i];
            if (mVertexDegrees[i] < mMinDegree)
                mMinDegree = mVertexDegrees[i];

            mNbEdges += static_cast<int>(mAdjacencyList[i].size());
        }

        mNbEdges /= 2; /* Undirected graph */
    }

    CheckerOutput Graph::checkSolution(const Solution& solution) const
    {
        return {};
    }
}