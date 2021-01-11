#include <vector>

class Graph
{
public:
    Graph(const std::vector<std::vector<int>>& adjacencyList) { mAdjacencyList = std::vector<std::vector<int>>(adjacencyList); }

private:
    std::vector<std::vector<int>> mAdjacencyList;
};