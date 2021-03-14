#include "SweepNeighborhood.hpp"

#include <algorithm>
#include <random>

namespace Yolo
{
    std::vector<Solution> SweepNeighborhood::generateAll(const Solution& solution) const
    {
        // Todo: This method generates a lot of doubles. How to optimize it?

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

    std::vector<Solution> SweepNeighborhood::generateAll(const Solution& solution, const Graph& graph, const Criterion* criterion) const
    {
        // Todo: This method generates a lot of doubles. How to optimize it?

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

                if (criterion->evaluate(graph, temp))
                {
                    neighbors[index++] = temp;
                }
            }
        }

        neighbors.erase(neighbors.begin() + index, neighbors.end());
        return neighbors;
    }

    Solution SweepNeighborhood::generateRandom(const Solution& solution) const
    {
        // Todo: optimise
        const auto& neighbors = generateAll(solution);

        std::random_device randomDevice;
        std::mt19937_64 randomEngine(randomDevice());
        std::uniform_int_distribution<int> randomIntDistribution(0, static_cast<int>(neighbors.size()) - 1);

        return neighbors[randomIntDistribution(randomEngine)];
    }

    Solution SweepNeighborhood::generateRandom(const Solution& solution, const Graph& graph, const Criterion* criterion) const
    {
        // Todo: optimise
        const auto& neighbors = generateAll(solution, graph, criterion);

        std::random_device randomDevice;
        std::mt19937_64 randomEngine(randomDevice());
        std::uniform_int_distribution<int> randomIntDistribution(0, static_cast<int>(neighbors.size()) - 1);

        return neighbors[randomIntDistribution(randomEngine)];
    }

    Solution SweepNeighborhood::generateBest(const Solution& solution, const Graph& graph, const Criterion* criterion) const
    {
        const auto& neighbors = generateAll(solution, graph, criterion);

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

    Solution SweepNeighborhood::generateBest(const Solution& solution, const Graph& graph, const Criterion* criterion, const std::list<Solution>& exceptions) const
    {
        const auto& neighbors = generateAll(solution, graph, criterion);

        Solution bestSolution = neighbors[0];
        double bestSolutionCost = graph.getSolutionCost(bestSolution);

        for (const auto& neighbor : neighbors)
        {
            double neighborCost = graph.getSolutionCost(neighbor);
            if (neighborCost < bestSolutionCost)
            {
                bool isInExceptions = (exceptions.end() != std::find(exceptions.begin(), exceptions.end(), bestSolution));
                if (!isInExceptions)
                {
                    bestSolution = neighbor;
                    bestSolutionCost = neighborCost;
                }
            }
        }

        return bestSolution;
    }
} // namespace Yolo
