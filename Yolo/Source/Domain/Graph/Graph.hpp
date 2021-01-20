#pragma once

#include <vector>

#include "Domain/Graph/Edge.hpp"

namespace Yolo
{
    class Graph
    {
    public:
        Graph(const std::vector<std::vector<Edge>>& adjacencyList) { mAdjacencyList = std::vector<std::vector<Edge>>(adjacencyList); }

    private:
        std::vector<std::vector<Edge>> mAdjacencyList;
    };
}