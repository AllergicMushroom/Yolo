#include "SweepNeighborhood.hpp"

#include <algorithm>
#include <random>

#include "Core/Logger/Logger.hpp"
#include <sstream>

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
                Solution newSolution = solution;

                int jClass = newSolution.getVertexClass(j);

                for (int k = j; k > i; --k)
                {
                    newSolution.setVertexClass(k, newSolution.getVertexClass(k - 1));
                }

                newSolution.setVertexClass(i, jClass);

                neighbors[index++] = newSolution;
            }
        }

        return neighbors;
    }

    std::vector<std::pair<Solution, double>> SweepNeighborhood::generateAll(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const
    {
        // Todo: This method generates a lot of doubles. How to optimize it?

        size_t nbVertices = static_cast<size_t>(solution.getNbVertices());
        size_t size = (nbVertices * nbVertices - nbVertices) / 2 + 1;
        std::vector<std::pair<Solution, double>> neighbors = std::vector<std::pair<Solution, double>>(size);

        size_t index = 0;
        neighbors[index++] = std::pair<Solution, double>(solution, solutionCost);

        for (int i = 0; i < solution.getNbVertices(); ++i)
        {
            for (int j = i + 1; j < solution.getNbVertices(); ++j)
            {
                Solution newSolution = solution;
                double newSolutionCost = solutionCost;

                for (int k = j; k > i; --k)
                {
                    const int& vertex1 = k;
                    const int& vertex2 = k - 1;

                    int cls1 = newSolution.getVertexClass(vertex1);
                    int cls2 = newSolution.getVertexClass(vertex2);

                    /* Don't swap elements of same class. */
                    if (cls1 == cls2)
                    {
                        continue;
                    }

                    newSolutionCost += graph.getSolutionCostDifference(newSolution, vertex1, cls2);
                    newSolution.setVertexClass(vertex1, cls2);

                    newSolutionCost += graph.getSolutionCostDifference(newSolution, vertex2, cls1);
                    newSolution.setVertexClass(vertex2, cls1);
                }

                if (criterion->evaluate(graph, newSolution))
                {
                    neighbors[index++] = std::pair<Solution, double>(newSolution, newSolutionCost);
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

    std::pair<Solution, double> SweepNeighborhood::generateRandom(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const
    {
        // Todo: optimise
        const auto& neighbors = generateAll(solution, solutionCost, graph, criterion);

        std::random_device randomDevice;
        std::mt19937_64 randomEngine(randomDevice());
        std::uniform_int_distribution<int> randomIntDistribution(0, static_cast<int>(neighbors.size()) - 1);

        return neighbors[randomIntDistribution(randomEngine)];
    }

    std::pair<Solution, double> SweepNeighborhood::generateBest(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const
    {
        const auto& neighbors = generateAll(solution, solutionCost, graph, criterion);

        Solution bestSolution = neighbors[0].first;
        double bestSolutionCost = neighbors[0].second;

        for (const auto& neighbor : neighbors)
        {
            double neighborCost = neighbor.second;
            if (neighborCost < bestSolutionCost)
            {
                bestSolution = neighbor.first;
                bestSolutionCost = neighborCost;
            }
        }

        return std::pair<Solution, double>(bestSolution, bestSolutionCost);
    }

    std::pair<Solution, double> SweepNeighborhood::generateBest(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion, const std::list<Solution>& exceptions) const
    {
        const auto& neighbors = generateAll(solution, solutionCost, graph, criterion);

        Solution bestSolution = neighbors[0].first;
        double bestSolutionCost = neighbors[0].second;

        for (const auto& neighbor : neighbors)
        {
            double neighborCost = neighbor.second;
            if (neighborCost < bestSolutionCost)
            {
                bool isInExceptions = (std::find(exceptions.begin(), exceptions.end(), bestSolution) != exceptions.end());
                if (!isInExceptions)
                {
                    bestSolution = neighbor.first;
                    bestSolutionCost = neighbor.second;
                }
            }
        }

        return std::pair<Solution, double>(bestSolution, bestSolutionCost);
    }
} // namespace Yolo
