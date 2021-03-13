#include "SwapNeighborhood.hpp"

#include <algorithm>
#include <random>

namespace Yolo
{
    std::vector<Solution> SwapNeighborhood::generateAll(const Solution& solution) const
    {
        /*
            We have to include the current solution so that convergence is guaranteed.
            We can cut nbVertices solutions because they're the same ones as the current solution.
        */
        size_t size = static_cast<size_t>(solution.getNbVertices()) * (static_cast<size_t>(solution.getNbVertices()) - 1) + 1;
        std::vector<Solution> neighbors = std::vector<Solution>(size, Solution(0, 0));

        size_t index = 0;
        neighbors[index++] = solution;

        for (int i = 0; i < solution.getNbVertices(); ++i)
        {
            /* Swapping i with j is the same as swapping j with i, and there is no interest in swapping i with itself, so we can consider only j > i. */
            for (int j = i + 1; j < solution.getNbVertices(); ++j)
            {
                int iClass = solution.getVertexClass(i);
                int jClass = solution.getVertexClass(j);

                /* Don't swap elements of same class. */
                if (iClass == jClass)
                {
                    continue;
                }

                Solution temp = solution;
                temp.setVertexClass(i, jClass);
                temp.setVertexClass(j, iClass);

                neighbors[index++] = temp;
            }
        }

        neighbors.erase(neighbors.begin() + index, neighbors.end());
        return neighbors;
    }

    std::vector<Solution> SwapNeighborhood::generateAll(const Solution& solution, const Graph& graph, const Criterion* criterion) const
    {
        /*
            We have to include the current solution so that convergence is guaranteed.
            We can cut nbVertices solutions because they're the same ones as the current solution.
        */
        size_t size = static_cast<size_t>(solution.getNbVertices()) * (static_cast<size_t>(solution.getNbVertices()) - 1) + 1;
        std::vector<Solution> neighbors = std::vector<Solution>(size, Solution(0, 0));

        size_t index = 0;
        neighbors[index++] = solution;

        for (int i = 0; i < solution.getNbVertices(); ++i)
        {
            /* Swapping i with j is the same as swapping j with i, and there is no interest in swapping i with itself, so we can consider only j > i. */
            for (int j = i + 1; j < solution.getNbVertices(); ++j)
            {
                int iClass = solution.getVertexClass(i);
                int jClass = solution.getVertexClass(j);

                /* Don't swap elements of same class. */
                if (iClass == jClass)
                {
                    continue;
                }

                Solution temp = solution;
                temp.setVertexClass(i, jClass);
                temp.setVertexClass(j, iClass);

                if (criterion->evaluate(graph, temp))
                {
                    neighbors[index++] = temp;
                }
            }
        }

        neighbors.erase(neighbors.begin() + index, neighbors.end());
        return neighbors;
    }

    Solution SwapNeighborhood::generateRandom(const Solution& solution) const
    {
        // Todo: optimise
        const auto& neighbors = generateAll(solution);

        std::random_device randomDevice;
        std::mt19937_64 randomEngine(randomDevice());
        std::uniform_int_distribution<int> randomIntDistribution(0, neighbors.size() - 1);

        return neighbors[randomIntDistribution(randomEngine)];
    }

    Solution SwapNeighborhood::generateRandom(const Solution& solution, const Graph& graph, const Criterion* criterion) const
    {
        // Todo: optimise
        const auto& neighbors = generateAll(solution, graph, criterion);

        std::random_device randomDevice;
        std::mt19937_64 randomEngine(randomDevice());
        std::uniform_int_distribution<int> randomIntDistribution(0, neighbors.size() - 1);

        return neighbors[randomIntDistribution(randomEngine)];
    }

    Solution SwapNeighborhood::generateBest(const Solution& solution, const Graph& graph, const Criterion* criterion) const
    {
        Solution best = solution;
        double bestDeltaCost = 0.0;

        int previousModifiedVertices[2] = {0, 0};
        int previousModifiedVerticesClasses[2] = {best.getVertexClass(previousModifiedVertices[0]), best.getVertexClass(previousModifiedVertices[1])};

        for (int i = 0; i < solution.getNbVertices(); ++i)
        {
            /* Swapping i with j is the same as swapping j with i, and there is no interest in swapping i with itself, so we can consider only j > i. */
            for (int j = i + 1; j < solution.getNbVertices(); ++j)
            {
                int iClass = solution.getVertexClass(i);
                int jClass = solution.getVertexClass(j);

                /* Don't swap elements of same class. */
                if (iClass == jClass)
                {
                    continue;
                }

                /* Revert to solution */
                best.setVertexClass(previousModifiedVertices[0], solution.getVertexClass(previousModifiedVertices[0]));
                best.setVertexClass(previousModifiedVertices[1], solution.getVertexClass(previousModifiedVertices[1]));

                double currentDeltacost = 0;

                currentDeltacost += graph.getSolutionCostDifference(best, i, jClass);
                best.setVertexClass(i, jClass);

                currentDeltacost += graph.getSolutionCostDifference(best, j, iClass);

                best.setVertexClass(i, iClass);
                best.setVertexClass(j, jClass);

                if (currentDeltacost < bestDeltaCost)
                {
                    best.setVertexClass(i, jClass);
                    best.setVertexClass(j, iClass);

                    if (criterion->evaluate(graph, best))
                    {
                        bestDeltaCost = currentDeltacost;

                        previousModifiedVertices[0] = i;
                        previousModifiedVertices[1] = j;

                        previousModifiedVerticesClasses[0] = jClass;
                        previousModifiedVerticesClasses[1] = iClass;
                    }
                    else
                    {
                        /* Revert to previous best */
                        best.setVertexClass(i, iClass);
                        best.setVertexClass(j, jClass);

                        best.setVertexClass(previousModifiedVertices[0], previousModifiedVerticesClasses[0]);
                        best.setVertexClass(previousModifiedVertices[1], previousModifiedVerticesClasses[1]);
                    }
                }
                else
                {
                    best.setVertexClass(previousModifiedVertices[0], previousModifiedVerticesClasses[0]);
                    best.setVertexClass(previousModifiedVertices[1], previousModifiedVerticesClasses[1]);
                }
            }
        }

        return best;
    }

    Solution SwapNeighborhood::generateBest(const Solution& solution, const Graph& graph, const Criterion* criterion, const std::list<Solution>& exceptions) const
    {
        Solution best = solution;
        double bestDeltaCost = 0.0;
        bool isSet = false;

        int previousModifiedVertices[2] = {0, 0};
        int previousModifiedVerticesClasses[2] = {best.getVertexClass(previousModifiedVertices[0]), best.getVertexClass(previousModifiedVertices[1])};

        for (int i = 0; i < solution.getNbVertices(); ++i)
        {
            /* Swapping i with j is the same as swapping j with i, and there is no interest in swapping i with itself, so we can consider only j > i. */
            for (int j = i + 1; j < solution.getNbVertices(); ++j)
            {
                int iClass = solution.getVertexClass(i);
                int jClass = solution.getVertexClass(j);

                /* Don't swap elements of same class. */
                if (iClass == jClass)
                {
                    continue;
                }

                /* Revert to solution */
                best.setVertexClass(previousModifiedVertices[0], solution.getVertexClass(previousModifiedVertices[0]));
                best.setVertexClass(previousModifiedVertices[1], solution.getVertexClass(previousModifiedVertices[1]));

                double currentDeltacost = 0;

                currentDeltacost += graph.getSolutionCostDifference(best, i, jClass);
                best.setVertexClass(i, jClass);

                currentDeltacost += graph.getSolutionCostDifference(best, j, iClass);

                best.setVertexClass(i, iClass);
                best.setVertexClass(j, jClass);

                if (currentDeltacost < bestDeltaCost || !isSet)
                {
                    best.setVertexClass(i, jClass);
                    best.setVertexClass(j, iClass);

                    bool isInExceptions = (exceptions.end() != std::find(exceptions.begin(), exceptions.end(), best));

                    if (!isInExceptions && criterion->evaluate(graph, best))
                    {
                        isSet = true;
                        bestDeltaCost = currentDeltacost;

                        previousModifiedVertices[0] = i;
                        previousModifiedVertices[1] = j;

                        previousModifiedVerticesClasses[0] = jClass;
                        previousModifiedVerticesClasses[1] = iClass;
                    }
                    else
                    {
                        /* Revert to previous best */
                        best.setVertexClass(i, iClass);
                        best.setVertexClass(j, jClass);

                        best.setVertexClass(previousModifiedVertices[0], previousModifiedVerticesClasses[0]);
                        best.setVertexClass(previousModifiedVertices[1], previousModifiedVerticesClasses[1]);
                    }
                }
                else
                {
                    best.setVertexClass(previousModifiedVertices[0], previousModifiedVerticesClasses[0]);
                    best.setVertexClass(previousModifiedVertices[1], previousModifiedVerticesClasses[1]);
                }
            }
        }
        return best;
    }
} // namespace Yolo
