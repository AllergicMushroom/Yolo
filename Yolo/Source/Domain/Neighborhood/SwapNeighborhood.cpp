#include "SwapNeighborhood.hpp"

#include "Core/Logger/Logger.hpp"

#include <algorithm>
#include <random>

static std::random_device sRandomDevice;
static std::mt19937_64 sRandomEngine(sRandomDevice());

namespace Yolo
{
    std::vector<Solution> SwapNeighborhood::generateAll(const Solution& solution) const
    {
        /*
            We have to include the current solution so that convergence is guaranteed.
            We can cut nbVertices solutions because they're the same ones as the current solution.
         */
        size_t size = static_cast<size_t>(solution.getNbVertices()) * (static_cast<size_t>(solution.getNbVertices()) - 1) / 2 + 1;
        std::vector<Solution> neighbors = std::vector<Solution>(size, Solution(0, 0));

        size_t index = 0;
        neighbors[index++] = solution;

        for (int vertex1 = 0; vertex1 < solution.getNbVertices(); ++vertex1)
        {
            /* Swapping vertex1 with vertex2 is the same as swapping vertex2 with vertex1, and there is no interest in swapping vertex1 with itself, so we can consider only vertex2 > vertex1. */
            for (int vertex2 = vertex1 + 1; vertex2 < solution.getNbVertices(); ++vertex2)
            {
                int cls1 = solution.getVertexClass(vertex1);
                int cls2 = solution.getVertexClass(vertex2);

                /* Don't swap elements of same class. */
                if (cls1 == cls2)
                {
                    continue;
                }

                Solution newSolution = solution;
                newSolution.setVertexClass(vertex1, cls2);
                newSolution.setVertexClass(vertex2, cls1);

                neighbors[index++] = newSolution;
            }
        }

        neighbors.erase(neighbors.begin() + index, neighbors.end());
        return neighbors;
    }

    std::vector<std::pair<Solution, double>> SwapNeighborhood::generateAll(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const
    {
        /*
            We have to include the current solution so that convergence is guaranteed.
            We can cut nbVertices solutions because it's swapping an element with itself, then divide by two because the graph is symmetric.
        */
        size_t size = static_cast<size_t>(solution.getNbVertices()) * (static_cast<size_t>(solution.getNbVertices()) - 1) / 2 + 1;
        std::vector<std::pair<Solution, double>> neighbors = std::vector<std::pair<Solution, double>>(size);

        size_t index = 0;
        neighbors[index++] = std::pair<Solution, double>(solution, solutionCost);

        for (int vertex1 = 0; vertex1 < solution.getNbVertices(); ++vertex1)
        {
            /* Swapping vertex1 with vertex2 is the same as swapping vertex2 with vertex1, and there is no interest in swapping vertex1 with itself, so we can consider only vertex2 > vertex1. */
            for (int vertex2 = vertex1 + 1; vertex2 < solution.getNbVertices(); ++vertex2)
            {
                int cls1 = solution.getVertexClass(vertex1);
                int cls2 = solution.getVertexClass(vertex2);

                /* Don't swap elements of same class. */
                if (cls1 == cls2)
                {
                    continue;
                }

                Solution newSolution = solution;
                double newSolutionCost = solutionCost;

                newSolutionCost += graph.getSolutionCostDifference(newSolution, vertex1, cls2);
                newSolution.setVertexClass(vertex1, cls2);

                newSolutionCost += graph.getSolutionCostDifference(newSolution, vertex2, cls1);
                newSolution.setVertexClass(vertex2, cls1);

                if (criterion->evaluate(graph, newSolution))
                {
                    neighbors[index++] = std::pair<Solution, double>(newSolution, newSolutionCost);
                }
            }
        }

        neighbors.erase(neighbors.begin() + index, neighbors.end());
        return neighbors;
    }

    Solution SwapNeighborhood::generateRandom(const Solution& solution) const
    {
        std::uniform_int_distribution<int> randomDoubleDistribution(0, solution.getNbVertices() - 1);

        constexpr int maxIterations = 1000;

        for (int iteration = 0; iteration < maxIterations; ++iteration)
        {
            int vertex1 = randomDoubleDistribution(sRandomEngine);
            int vertex2 = randomDoubleDistribution(sRandomEngine);

            int cls1 = solution.getVertexClass(vertex1);
            int cls2 = solution.getVertexClass(vertex2);

            if (cls1 != cls2)
            {
                Solution newSolution = solution;
                newSolution.setVertexClass(vertex1, cls2);
                newSolution.setVertexClass(vertex2, cls1);
                return newSolution;
            }
        }

        return solution;
    }

    std::pair<Solution, double> SwapNeighborhood::generateRandom(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const
    {
        static std::uniform_int_distribution<int> randomDoubleDistribution(0, solution.getNbVertices() - 1);

        constexpr int maxIterations = 10000;

        for (int iteration = 0; iteration < maxIterations; ++iteration)
        {
            int vertex1 = randomDoubleDistribution(sRandomEngine);
            int vertex2 = randomDoubleDistribution(sRandomEngine);

            int cls1 = solution.getVertexClass(vertex1);
            int cls2 = solution.getVertexClass(vertex2);

            if (cls1 != cls2)
            {
                Solution newSolution = solution;
                double newSolutionCost = solutionCost;

                newSolutionCost += graph.getSolutionCostDifference(newSolution, vertex1, cls2);
                newSolution.setVertexClass(vertex1, cls2);

                newSolutionCost += graph.getSolutionCostDifference(newSolution, vertex2, cls1);
                newSolution.setVertexClass(vertex2, cls1);

                if (criterion->evaluate(graph, newSolution))
                {
                    return std::pair<Solution, double>(newSolution, newSolutionCost);
                }
            }
        }

        return std::pair<Solution, double>(solution, solutionCost);
    }

    std::pair<Solution, double> SwapNeighborhood::generateBest(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const
    {
        Solution bestSolution = solution;
        double bestSolutionCost = solutionCost;

        int previousModifiedVertices[2] = {0, 0};
        int previousModifiedVerticesClasses[2] = {bestSolution.getVertexClass(previousModifiedVertices[0]), bestSolution.getVertexClass(previousModifiedVertices[1])};

        for (int vertex1 = 0; vertex1 < solution.getNbVertices(); ++vertex1)
        {
            /* Swapping vertex1 with vertex2 is the same as swapping vertex2 with vertex1, and there is no interest in swapping vertex1 with itself, so we can consider only vertex2 > vertex1. */
            for (int vertex2 = vertex1 + 1; vertex2 < solution.getNbVertices(); ++vertex2)
            {
                int cls1 = solution.getVertexClass(vertex1);
                int cls2 = solution.getVertexClass(vertex2);

                /* Don't swap elements of same class. */
                if (cls1 == cls2)
                {
                    continue;
                }

                /* Revert to original solution */
                bestSolution.setVertexClass(previousModifiedVertices[0], solution.getVertexClass(previousModifiedVertices[0]));
                bestSolution.setVertexClass(previousModifiedVertices[1], solution.getVertexClass(previousModifiedVertices[1]));

                /* Calculate new solution cost */
                double newSolutionCost = solutionCost;

                newSolutionCost += graph.getSolutionCostDifference(bestSolution, vertex1, cls2);
                bestSolution.setVertexClass(vertex1, cls2);

                newSolutionCost += graph.getSolutionCostDifference(bestSolution, vertex2, cls1);

                /* Revert to original solution */
                bestSolution.setVertexClass(vertex1, cls1);
                bestSolution.setVertexClass(vertex2, cls2);

                if (newSolutionCost < bestSolutionCost)
                {
                    bestSolution.setVertexClass(vertex1, cls2);
                    bestSolution.setVertexClass(vertex2, cls1);

                    if (criterion->evaluate(graph, bestSolution))
                    {
                        bestSolutionCost = newSolutionCost;

                        previousModifiedVertices[0] = vertex1;
                        previousModifiedVertices[1] = vertex2;

                        previousModifiedVerticesClasses[0] = cls2;
                        previousModifiedVerticesClasses[1] = cls1;
                    }
                    else
                    {
                        /* Revert to previous bestSolution */
                        bestSolution.setVertexClass(vertex1, cls1);
                        bestSolution.setVertexClass(vertex2, cls2);

                        bestSolution.setVertexClass(previousModifiedVertices[0], previousModifiedVerticesClasses[0]);
                        bestSolution.setVertexClass(previousModifiedVertices[1], previousModifiedVerticesClasses[1]);
                    }
                }
                else
                {
                    bestSolution.setVertexClass(previousModifiedVertices[0], previousModifiedVerticesClasses[0]);
                    bestSolution.setVertexClass(previousModifiedVertices[1], previousModifiedVerticesClasses[1]);
                }
            }
        }

        return std::pair<Solution, double>(bestSolution, bestSolutionCost);
    }

    std::pair<Solution, double> SwapNeighborhood::generateBest(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion, const std::list<Solution>& exceptions) const
    {
        Solution bestSolution = solution;
        double bestSolutionCost = solutionCost;

        bool isSet = false;

        int previousModifiedVertices[2] = {0, 0};
        int previousModifiedVerticesClasses[2] = {bestSolution.getVertexClass(previousModifiedVertices[0]), bestSolution.getVertexClass(previousModifiedVertices[1])};

        for (int i = 0; i < solution.getNbVertices(); ++i)
        {
            /* Swapping vertex1 with vertex2 is the same as swapping vertex2 with vertex1, and there is no interest in swapping vertex1 with itself, so we can consider only vertex2 > vertex1. */
            for (int j = i + 1; j < solution.getNbVertices(); ++j)
            {
                int iClass = solution.getVertexClass(i);
                int jClass = solution.getVertexClass(j);

                /* Don't swap elements of same class. */
                if (iClass == jClass)
                {
                    continue;
                }

                /* Revert to original solution */
                bestSolution.setVertexClass(previousModifiedVertices[0], solution.getVertexClass(previousModifiedVertices[0]));
                bestSolution.setVertexClass(previousModifiedVertices[1], solution.getVertexClass(previousModifiedVertices[1]));

                double newSolutionCost = solutionCost;

                newSolutionCost += graph.getSolutionCostDifference(bestSolution, i, jClass);
                bestSolution.setVertexClass(i, jClass);

                newSolutionCost += graph.getSolutionCostDifference(bestSolution, j, iClass);

                bestSolution.setVertexClass(i, iClass);
                bestSolution.setVertexClass(j, jClass);

                if (newSolutionCost < bestSolutionCost || !isSet)
                {
                    bestSolution.setVertexClass(i, jClass);
                    bestSolution.setVertexClass(j, iClass);

                    bool isInExceptions = (exceptions.end() != std::find(exceptions.begin(), exceptions.end(), bestSolution));

                    if (!isInExceptions && criterion->evaluate(graph, bestSolution))
                    {
                        isSet = true;
                        bestSolutionCost = newSolutionCost;

                        previousModifiedVertices[0] = i;
                        previousModifiedVertices[1] = j;

                        previousModifiedVerticesClasses[0] = jClass;
                        previousModifiedVerticesClasses[1] = iClass;
                    }
                    else
                    {
                        /* Revert to previous bestSolution */
                        bestSolution.setVertexClass(i, iClass);
                        bestSolution.setVertexClass(j, jClass);

                        bestSolution.setVertexClass(previousModifiedVertices[0], previousModifiedVerticesClasses[0]);
                        bestSolution.setVertexClass(previousModifiedVertices[1], previousModifiedVerticesClasses[1]);
                    }
                }
                else
                {
                    bestSolution.setVertexClass(previousModifiedVertices[0], previousModifiedVerticesClasses[0]);
                    bestSolution.setVertexClass(previousModifiedVertices[1], previousModifiedVerticesClasses[1]);
                }
            }
        }

        return std::pair<Solution, double>(bestSolution, bestSolutionCost);
    }
} // namespace Yolo
