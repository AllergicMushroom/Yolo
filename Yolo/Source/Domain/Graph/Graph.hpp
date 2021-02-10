#pragma once

#include "Domain/Graph/Edge.hpp"

#include "Domain/Solution/Solution.hpp"

#include <vector>

namespace Yolo
{
    struct CheckerOutput
    {
        const bool isValid;
        const double cost;
    };

    class Graph
    {
    public:
        Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree);

        Graph(const std::vector<std::vector<Edge>>& adjacencyList);

        inline int getNbVertices() const { return static_cast<int>(mAdjacencyList.size()); }

        inline int getVertexDegree(int vertex) const { return mVerticesDegrees[vertex]; }

        inline int getMaxDegree() const { return mMaxDegree; }
        inline int getMinDegree() const { return mMinDegree; }

        inline int getNbEdges() const { return mNbEdges; }

        double getSolutionCost(const Solution& solution) const;
        double getSolutionCostDifference(const Solution& solution, int vertex, int newClass) const;

        CheckerOutput checkSolution(const Solution& solution, bool (*criterion)(std::vector<int>, int, int)) const;

    private:
        std::vector<int> mVerticesDegrees;
        int mMaxDegree;
        int mMinDegree;

        int mNbEdges;

        std::vector<std::vector<Edge>> mAdjacencyList;
    };
} // namespace Yolo
