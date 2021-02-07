#include "Graph.hpp"

#include <iostream> // TODO

namespace Yolo
{
    Graph::Graph(const std::vector<std::vector<Edge>>& adjacencyList)
        : mAdjacencyList(std::vector<std::vector<Edge>>(adjacencyList))
    {
        mVertexDegrees = std::vector<int>(mAdjacencyList.size());
        mMaxDegree = static_cast<int>(mAdjacencyList[0].size());
        mMinDegree = static_cast<int>(mAdjacencyList[0].size());

        mNbEdges = 0;

        for (unsigned int i = 0; i < mAdjacencyList.size(); ++i)
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

    CheckerOutput Graph::checkSolution(const Solution& solution, bool criterion(std::vector<int>, int, int)) const
    {
        // TODO
        return {false, getSolutionCost(solution)};
    }

    double Graph::getSolutionCost(const Solution& solution) const
    {
        double cost = 0;

        for (unsigned int i = 0; i < mAdjacencyList.size(); ++i)
        {
            for (unsigned int j = 0; j < mAdjacencyList[i].size(); ++j)
            {
                if (solution.getVertexClass(mAdjacencyList[i][j].getSource()) != solution.getVertexClass(mAdjacencyList[i][j].getDestination()))
                {
                    cost += mAdjacencyList[i][j].getWeight();
                }
            }
        }

        /* Since the graph is inderected, we've gone through each arc twice. */
        return cost / 2;
    }

    double Graph::getSolutionCostDifference(const Solution& solution, int vertex, int newClass) const
    {
        int currentClass = solution.getVertexClass(vertex);
        double delta = 0;
        for (unsigned int j = 0; j < mAdjacencyList[vertex].size(); ++j)
        {
            int adjacentClass = solution.getVertexClass(mAdjacencyList[vertex][j].getDestination());
            if (adjacentClass == currentClass)
            {
                delta += mAdjacencyList[vertex][j].getWeight();
            }
            else if (adjacentClass == newClass)
            {
                delta -= mAdjacencyList[vertex][j].getWeight();
            }
        }
        return delta;
    }
} // namespace Yolo
