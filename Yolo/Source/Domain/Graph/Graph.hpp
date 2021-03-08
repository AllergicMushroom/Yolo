#pragma once

#include "Domain/Graph/Edge.hpp"

#include "Domain/Solution/Solution.hpp"

#include <vector>

namespace Yolo
{
    class Graph
    {
    public:
        Graph()
            : Graph(0, 0, 0, 0) {}

        Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree);

        Graph(const std::vector<std::vector<Edge>>& adjacencyList);

        bool operator==(const Graph& graph2) const;
        bool operator!=(const Graph& graph2) const;

        inline int getNbVertices() const { return static_cast<int>(mAdjacencyList.size()); }

        inline int getVertexDegree(int vertex) const { return mVerticesDegrees[vertex]; }

        inline int getMaxDegree() const { return mMaxDegree; }
        inline int getMinDegree() const { return mMinDegree; }

        inline int getNbEdges() const { return mNbEdges; }

        double getEdgeWeight(int source, int destination) const;

        double getSolutionCost(const Solution& solution) const;
        double getSolutionCostDifference(const Solution& solution, int vertex, int newClass) const;

    private:
        std::vector<int> mVerticesDegrees;
        int mMaxDegree;
        int mMinDegree;

        int mNbEdges;

        std::vector<std::vector<Edge>> mAdjacencyList;
    };
} // namespace Yolo
