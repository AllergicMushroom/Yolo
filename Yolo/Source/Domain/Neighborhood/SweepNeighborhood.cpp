#include "SweepNeighborhood.hpp"

namespace Yolo
{
    std::vector<Solution> SweepNeighborhood::generateAll(const Solution& solution) const
    {
        /*
        * TODO
        * This method generates a lot of doubles.
        * How to optimize it?
        */

        size_t nbVertices = static_cast<size_t>(solution.getNbVertices());
        size_t size = (nbVertices * nbVertices - nbVertices) / 2 + 1;
        std::vector<Solution> neighbors = std::vector<Solution>(size, Solution(0, 0));

        size_t index = 0;
        neighbors[index++] = solution;

        for (int i = 0; i < solution.getNbVertices(); ++i)
        {
            for (int j = i + 1; j < solution.getNbVertices(); ++j)
            {
                Solution temp = solution;

                int jClass = temp.getVertexClass(j);

                for (int k = j; k > i; --k)
                {
                    temp.setVertexClass(k, temp.getVertexClass(k - 1));
                }

                temp.setVertexClass(i, jClass);

                neighbors[index++] = temp;
            }
        }

        return neighbors;
    }

    Solution SweepNeighborhood::generateBest(const Graph& graph, const Criterion* criterion, const Solution& solution) const
    {
        const auto& neighbors = generateAll(solution);

        Solution bestSolution = neighbors[0];
        double bestSolutionCost = graph.getSolutionCost(bestSolution);

        for (const auto& neighbor : neighbors)
        {
            double neighborCost = graph.getSolutionCost(neighbor);
            if (neighborCost < bestSolutionCost)
            {
                bestSolution = neighbor;
                bestSolutionCost = neighborCost;
            }
        }

        return bestSolution;
    }
} // namespace Yolo
