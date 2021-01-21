#pragma once

#include <vector>

#include "Domain/Graph/Edge.hpp"

namespace Yolo
{
    class Graph
    {
    public:
        Graph(const std::vector<std::vector<Edge>>& adjacencyList) { 
            mAdjacencyList = std::vector<std::vector<Edge>>(adjacencyList); 
            vertexDegree = std::vector<int>(mAdjacencyList.size());

            maxDegree = mAdjacencyList[0].size();
            minDegree = mAdjacencyList[0].size();

            for(int i = 0; i<mAdjacencyList.size(); ++i){
                vertexDegree[i] = mAdjacencyList[i].size();
                if(vertexDegree[i] > maxDegree)
                    maxDegree = vertexDegree[i];
                if(vertexDegree[i] < minDegree)
                    minDegree = vertexDegree[i];
            }

            
        }

        int getVertexDegree(int vertex){
            return vertexDegree[vertex];
        }

    private:
        int maxDegree;
        int minDegree;
        std::vector<std::vector<Edge>> mAdjacencyList;
        std::vector<int> vertexDegree;
    };
}