#include "PickNDropNeighborhood.hpp"

#include <algorithm>
#include <random>

namespace Yolo
{
    std::vector<Solution> PickNDropNeighborhood::generateAll(const Solution& solution) const
    {
        /*
            We have to include the current solution so that convergence is guaranteed.
            We can cut nbVertices solutions because they're the same ones as the current solution.
        */
        size_t size = static_cast<size_t>(solution.getNbVertices()) * (static_cast<size_t>(solution.getNbClasses()) - 1) + 1;
        std::vector<Solution> neighbors(size, Solution(0, 0));

        int index = 0;
        neighbors[index++] = solution;

        for (int i = 0; i < solution.getNbVertices(); i++)
        {
            for (int j = 0; j < solution.getNbClasses(); j++)
            {
                if (solution.getVertexClass(i) == j)
                {
                    continue;
                }

                Solution temp = solution;
                temp.setVertexClass(i, j);
                neighbors[index++] = temp;
            }
        }

        return neighbors;
    }

    std::vector<Solution> PickNDropNeighborhood::generateAll(const Solution& solution, const Graph& graph, const Criterion* criterion) const
    {
        /*
            We have to include the current solution so that convergence is guaranteed.
            We can cut nbVertices solutions because they're the same ones as the current solution.
        */
        size_t size = static_cast<size_t>(solution.getNbVertices()) * (static_cast<size_t>(solution.getNbClasses()) - 1) + 1;
        std::vector<Solution> neighbors(size, Solution(0, 0));

        int index = 0;
        neighbors[index++] = solution;

        for (int i = 0; i < solution.getNbVertices(); i++)
        {
            for (int j = 0; j < solution.getNbClasses(); j++)
            {
                if (solution.getVertexClass(i) == j)
                {
                    continue;
                }

                Solution temp = solution;
                temp.setVertexClass(i, j);

                if (criterion->evaluate(graph, temp))
                {
                    neighbors[index++] = temp;
                }
            }
        }

        neighbors.erase(neighbors.begin() + index, neighbors.end());
        return neighbors;
    }

    Solution PickNDropNeighborhood::generateRandom(const Solution& solution) const
    {
        // Todo: optimise
        const auto& neighbors = generateAll(solution);

        std::random_device randomDevice;
        std::mt19937_64 randomEngine(randomDevice());
        std::uniform_int_distribution<int> randomIntDistribution(0, neighbors.size() - 1);

        return neighbors[randomIntDistribution(randomEngine)];
    }

    Solution PickNDropNeighborhood::generateRandom(const Solution& solution, const Graph& graph, const Criterion* criterion) const
    {
        // Todo: optimise
        const auto& neighbors = generateAll(solution, graph, criterion);

        std::random_device randomDevice;
        std::mt19937_64 randomEngine(randomDevice());
        std::uniform_int_distribution<int> randomIntDistribution(0, neighbors.size() - 1);

        return neighbors[randomIntDistribution(randomEngine)];
    }

    Solution PickNDropNeighborhood::generateBest(const Solution& solution, const Graph& graph, const Criterion* criterion) const
    {
        Solution best = solution;
        double bestDeltaCost = 0;

        int previousModifiedVertex = 0;

        for (int i = 0; i < solution.getNbVertices(); i++)
        {
            for (int j = 0; j < solution.getNbClasses(); j++)
            {
                if (solution.getVertexClass(i) == j)
                {
                    continue;
                }

                double currentDeltaCost = graph.getSolutionCostDifference(solution, i, j);
                if (currentDeltaCost < bestDeltaCost)
                {
                    int previousModifiedVertexClass = best.getVertexClass(previousModifiedVertex);

                    best.setVertexClass(previousModifiedVertex, solution.getVertexClass(previousModifiedVertex));

                    best.setVertexClass(i, j);

                    if (criterion->evaluate(graph, best))
                    {
                        bestDeltaCost = currentDeltaCost;

                        previousModifiedVertex = i;
                    }
                    else
                    {
                        best.setVertexClass(i, solution.getVertexClass(i));
                        best.setVertexClass(previousModifiedVertex, previousModifiedVertexClass);
                    }
                }
            }
        }
        return best;
    }

    Solution PickNDropNeighborhood::generateBest(const Solution& solution, const Graph& graph, const Criterion* criterion, const std::list<Solution>& exceptions) const
    {
        Solution best = solution;
        double bestDeltaCost = 0;
        bool isSet = false;
        int previousModifiedVertex = 0;

        for (int i = 0; i < solution.getNbVertices(); i++)
        {
            for (int j = 0; j < solution.getNbClasses(); j++)
            {
                if (solution.getVertexClass(i) == j)
                {
                    continue;
                }
                double currentDeltaCost = graph.getSolutionCostDifference(solution, i, j);
                if (currentDeltaCost < bestDeltaCost || !isSet)
                {
                    int previousModifiedVertexClass = best.getVertexClass(previousModifiedVertex);

                    best.setVertexClass(previousModifiedVertex, solution.getVertexClass(previousModifiedVertex));
                    best.setVertexClass(i, j);

                    bool isInExceptions = false;
                    bool isValid = criterion->evaluate(graph, best);
                    if (isValid)
                        isInExceptions = (exceptions.end() != std::find(exceptions.begin(), exceptions.end(), best));

                    if (isValid && !isInExceptions)
                    {
                        bestDeltaCost = currentDeltaCost;
                        previousModifiedVertex = i;
                        isSet = true;
                    }
                    else
                    {
                        best.setVertexClass(i, solution.getVertexClass(i));
                        best.setVertexClass(previousModifiedVertex, previousModifiedVertexClass);
                    }
                }
            }
        }
        return best;
    }
} // namespace Yolo
