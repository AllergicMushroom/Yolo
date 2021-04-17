#include "SweepNeighborhood.hpp"

#include <algorithm>
#include <random>

#include "Core/Logger/Logger.hpp"
#include <thread>

static std::random_device sRandomDevice;
static std::mt19937_64 sRandomEngine(sRandomDevice());

// Todo: Variables names

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

        for (int vertex1 = 0; vertex1 < solution.getNbVertices(); ++vertex1)
        {
            for (int vertex2 = vertex1 + 1; vertex2 < solution.getNbVertices(); ++vertex2)
            {
                Solution newSolution = solution;

                int cls2 = newSolution.getVertexClass(vertex2);

                for (int k = vertex2; k > vertex1; --k)
                {
                    newSolution.setVertexClass(k, newSolution.getVertexClass(k - 1));
                }

                newSolution.setVertexClass(vertex1, cls2);

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
        std::uniform_int_distribution<int> randomDoubleDistribution(0, solution.getNbVertices() - 1);

        constexpr int maxIterations = 1000;

        for (int iteration = 0; iteration < maxIterations; ++iteration)
        {
            int i = randomDoubleDistribution(sRandomEngine);
            int j = randomDoubleDistribution(sRandomEngine);

            if (i == j)
            {
                continue;
            }

            Solution newSolution = solution;

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

                newSolution.setVertexClass(vertex1, cls2);
                newSolution.setVertexClass(vertex2, cls1);
            }

            return newSolution;
        }

        return solution;
    }

    std::pair<Solution, double> SweepNeighborhood::generateRandom(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const
    {
        static std::uniform_int_distribution<int> randomDoubleDistribution(0, solution.getNbVertices() - 1);

        constexpr int maxIterations = 10000;

        for (int iteration = 0; iteration < maxIterations; ++iteration)
        {
            int i = randomDoubleDistribution(sRandomEngine);
            int j = randomDoubleDistribution(sRandomEngine);

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
                return std::pair<Solution, double>(newSolution, newSolutionCost);
            }
        }

        return std::pair<Solution, double>(solution, solutionCost);
    }

    std::pair<Solution, double> SweepNeighborhood::generateBest(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const
    {
        Solution bestSolution = solution;
        double bestSolutionCost = solutionCost;

        int previousSweepedVertices[2] = {0, 0};

        for (int vertex1 = 0; vertex1 < solution.getNbVertices(); ++vertex1)
        {
            for (int vertex2 = vertex1 + 1; vertex2 < solution.getNbVertices(); ++vertex2)
            {
                /* Revert to original solution */
                for (int k = previousSweepedVertices[0]; k < previousSweepedVertices[1]; ++k)
                {
                    const int& v1 = k;
                    const int& v2 = k + 1;

                    int cls1 = bestSolution.getVertexClass(v1);
                    int cls2 = bestSolution.getVertexClass(v2);

                    /* Don't swap elements of same class. */
                    if (cls1 == cls2)
                    {
                        continue;
                    }

                    bestSolution.setVertexClass(v1, cls2);
                    bestSolution.setVertexClass(v2, cls1);
                }

                /* Calculate new solution cost */
                double newSolutionCost = solutionCost;

                for (int k = vertex2; k > vertex1; --k)
                {
                    const int& v1 = k;
                    const int& v2 = k - 1;

                    int cls1 = bestSolution.getVertexClass(v1);
                    int cls2 = bestSolution.getVertexClass(v2);

                    /* Don't swap elements of same class. */
                    if (cls1 == cls2)
                    {
                        continue;
                    }

                    newSolutionCost += graph.getSolutionCostDifference(bestSolution, v1, cls2);
                    bestSolution.setVertexClass(v1, cls2);

                    newSolutionCost += graph.getSolutionCostDifference(bestSolution, v2, cls1);
                    bestSolution.setVertexClass(v2, cls1);
                }

                /* Revert to original solution */
                for (int k = vertex1; k < vertex2; ++k)
                {
                    const int& v1 = k;
                    const int& v2 = k + 1;

                    int cls1 = bestSolution.getVertexClass(v1);
                    int cls2 = bestSolution.getVertexClass(v2);

                    /* Don't swap elements of same class. */
                    if (cls1 == cls2)
                    {
                        continue;
                    }

                    bestSolution.setVertexClass(v1, cls2);
                    bestSolution.setVertexClass(v2, cls1);
                }

                if (newSolutionCost < bestSolutionCost)
                {
                    for (int k = vertex2; k > vertex1; --k)
                    {
                        const int& v1 = k;
                        const int& v2 = k - 1;

                        int cls1 = bestSolution.getVertexClass(v1);
                        int cls2 = bestSolution.getVertexClass(v2);

                        /* Don't swap elements of same class. */
                        if (cls1 == cls2)
                        {
                            continue;
                        }

                        bestSolution.setVertexClass(v1, cls2);
                        bestSolution.setVertexClass(v2, cls1);
                    }

                    if (criterion->evaluate(graph, bestSolution))
                    {
                        bestSolutionCost = newSolutionCost;

                        previousSweepedVertices[0] = vertex1;
                        previousSweepedVertices[1] = vertex2;
                    }
                    else
                    {
                        /* Revert to previous bestSolution */
                        for (int k = vertex1; k < vertex2; ++k)
                        {
                            const int& v1 = k;
                            const int& v2 = k + 1;

                            int cls1 = bestSolution.getVertexClass(v1);
                            int cls2 = bestSolution.getVertexClass(v2);

                            /* Don't swap elements of same class. */
                            if (cls1 == cls2)
                            {
                                continue;
                            }

                            bestSolution.setVertexClass(v1, cls2);
                            bestSolution.setVertexClass(v2, cls1);
                        }

                        for (int k = previousSweepedVertices[1]; k > previousSweepedVertices[0]; --k)
                        {
                            const int& v1 = k;
                            const int& v2 = k - 1;

                            int cls1 = bestSolution.getVertexClass(v1);
                            int cls2 = bestSolution.getVertexClass(v2);

                            /* Don't swap elements of same class. */
                            if (cls1 == cls2)
                            {
                                continue;
                            }

                            bestSolution.setVertexClass(v1, cls2);
                            bestSolution.setVertexClass(v2, cls1);
                        }
                    }
                }
                else
                {
                    for (int k = previousSweepedVertices[1]; k > previousSweepedVertices[0]; --k)
                    {
                        const int& v1 = k;
                        const int& v2 = k - 1;

                        int cls1 = bestSolution.getVertexClass(v1);
                        int cls2 = bestSolution.getVertexClass(v2);

                        /* Don't swap elements of same class. */
                        if (cls1 == cls2)
                        {
                            continue;
                        }

                        bestSolution.setVertexClass(v1, cls2);
                        bestSolution.setVertexClass(v2, cls1);
                    }
                }
            }
        }

        return std::pair<Solution, double>(bestSolution, bestSolutionCost);
    }

    std::pair<Solution, double> SweepNeighborhood::generateBest(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion, const std::list<Solution>& exceptions) const
    {
        Solution bestSolution = solution;
        double bestSolutionCost = solutionCost;

        bool isSet = false;

        int previousSweepedVertices[2] = {0, 0};

        for (int vertex1 = 0; vertex1 < solution.getNbVertices(); ++vertex1)
        {
            for (int vertex2 = vertex1 + 1; vertex2 < solution.getNbVertices(); ++vertex2)
            {
                /* Revert to original solution */
                for (int k = previousSweepedVertices[0]; k < previousSweepedVertices[1]; ++k)
                {
                    const int& v1 = k;
                    const int& v2 = k + 1;

                    int cls1 = bestSolution.getVertexClass(v1);
                    int cls2 = bestSolution.getVertexClass(v2);

                    /* Don't swap elements of same class. */
                    if (cls1 == cls2)
                    {
                        continue;
                    }

                    bestSolution.setVertexClass(v1, cls2);
                    bestSolution.setVertexClass(v2, cls1);
                }

                /* Calculate new solution cost */
                double newSolutionCost = solutionCost;

                for (int k = vertex2; k > vertex1; --k)
                {
                    const int& v1 = k;
                    const int& v2 = k - 1;

                    int cls1 = bestSolution.getVertexClass(v1);
                    int cls2 = bestSolution.getVertexClass(v2);

                    /* Don't swap elements of same class. */
                    if (cls1 == cls2)
                    {
                        continue;
                    }

                    newSolutionCost += graph.getSolutionCostDifference(bestSolution, v1, cls2);
                    bestSolution.setVertexClass(v1, cls2);

                    newSolutionCost += graph.getSolutionCostDifference(bestSolution, v2, cls1);
                    bestSolution.setVertexClass(v2, cls1);
                }

                /* Revert to original solution */
                for (int k = vertex1; k < vertex2; ++k)
                {
                    const int& v1 = k;
                    const int& v2 = k + 1;

                    int cls1 = bestSolution.getVertexClass(v1);
                    int cls2 = bestSolution.getVertexClass(v2);

                    /* Don't swap elements of same class. */
                    if (cls1 == cls2)
                    {
                        continue;
                    }

                    bestSolution.setVertexClass(v1, cls2);
                    bestSolution.setVertexClass(v2, cls1);
                }

                if (newSolutionCost < bestSolutionCost || !isSet)
                {
                    for (int k = vertex2; k > vertex1; --k)
                    {
                        const int& v1 = k;
                        const int& v2 = k - 1;

                        int cls1 = bestSolution.getVertexClass(v1);
                        int cls2 = bestSolution.getVertexClass(v2);

                        /* Don't swap elements of same class. */
                        if (cls1 == cls2)
                        {
                            continue;
                        }

                        bestSolution.setVertexClass(v1, cls2);
                        bestSolution.setVertexClass(v2, cls1);
                    }

                    bool isInExceptions = (exceptions.end() != std::find(exceptions.begin(), exceptions.end(), bestSolution));

                    if (!isInExceptions && criterion->evaluate(graph, bestSolution))
                    {
                        isSet = true;

                        bestSolutionCost = newSolutionCost;

                        previousSweepedVertices[0] = vertex1;
                        previousSweepedVertices[1] = vertex2;
                    }
                    else
                    {
                        /* Revert to previous bestSolution */
                        for (int k = vertex1; k < vertex2; ++k)
                        {
                            const int& v1 = k;
                            const int& v2 = k + 1;

                            int cls1 = bestSolution.getVertexClass(v1);
                            int cls2 = bestSolution.getVertexClass(v2);

                            /* Don't swap elements of same class. */
                            if (cls1 == cls2)
                            {
                                continue;
                            }

                            bestSolution.setVertexClass(v1, cls2);
                            bestSolution.setVertexClass(v2, cls1);
                        }

                        for (int k = previousSweepedVertices[1]; k > previousSweepedVertices[0]; --k)
                        {
                            const int& v1 = k;
                            const int& v2 = k - 1;

                            int cls1 = bestSolution.getVertexClass(v1);
                            int cls2 = bestSolution.getVertexClass(v2);

                            /* Don't swap elements of same class. */
                            if (cls1 == cls2)
                            {
                                continue;
                            }

                            bestSolution.setVertexClass(v1, cls2);
                            bestSolution.setVertexClass(v2, cls1);
                        }
                    }
                }
                else
                {
                    for (int k = previousSweepedVertices[1]; k > previousSweepedVertices[0]; --k)
                    {
                        const int& v1 = k;
                        const int& v2 = k - 1;

                        int cls1 = bestSolution.getVertexClass(v1);
                        int cls2 = bestSolution.getVertexClass(v2);

                        /* Don't swap elements of same class. */
                        if (cls1 == cls2)
                        {
                            continue;
                        }

                        bestSolution.setVertexClass(v1, cls2);
                        bestSolution.setVertexClass(v2, cls1);
                    }
                }
            }
        }

        return std::pair<Solution, double>(bestSolution, bestSolutionCost);
    }
} // namespace Yolo
