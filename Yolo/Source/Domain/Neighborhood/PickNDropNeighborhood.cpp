#include "PickNDropNeighborhood.hpp"

#include <algorithm>
#include <random>

static std::random_device sRandomDevice;
static std::mt19937_64 sRandomEngine(sRandomDevice());

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

        for (int vertex = 0; vertex < solution.getNbVertices(); vertex++)
        {
            for (int cls = 0; cls < solution.getNbClasses(); cls++)
            {
                if (solution.getVertexClass(vertex) == cls)
                {
                    continue;
                }

                Solution newSolution = solution;
                newSolution.setVertexClass(vertex, cls);
                neighbors[index++] = newSolution;
            }
        }

        return neighbors;
    }

    std::vector<std::pair<Solution, double>> PickNDropNeighborhood::generateAll(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const
    {
        /*
            We have to include the current solution so that convergence is guaranteed.
            We can cut nbVertices solutions because they're the same ones as the current solution.
        */
        size_t size = static_cast<size_t>(solution.getNbVertices()) * (static_cast<size_t>(solution.getNbClasses()) - 1) + 1;
        std::vector<std::pair<Solution, double>> neighbors(size);

        int index = 0;
        neighbors[index++] = std::pair<Solution, double>(solution, solutionCost);

        for (int vertex = 0; vertex < solution.getNbVertices(); vertex++)
        {
            for (int cls = 0; cls < solution.getNbClasses(); cls++)
            {
                if (solution.getVertexClass(vertex) == cls)
                {
                    continue;
                }

                Solution newSolution = solution;
                newSolution.setVertexClass(vertex, cls);

                if (criterion->evaluate(graph, newSolution))
                {
                    double newSolutionCost = solutionCost + graph.getSolutionCostDifference(solution, vertex, cls);
                    neighbors[index++] = std::pair<Solution, double>(newSolution, newSolutionCost);
                }
            }
        }

        neighbors.erase(neighbors.begin() + index, neighbors.end());
        return neighbors;
    }

    Solution PickNDropNeighborhood::generateRandom(const Solution& solution) const
    {
        std::uniform_int_distribution<int> vertexDistribution(0, solution.getNbVertices() - 1);
        std::uniform_int_distribution<int> classDistribution(0, solution.getNbClasses() - 1);

        constexpr int maxIterations = 1000;

        for (int iteration = 0; iteration < maxIterations; ++iteration)
        {
            int vertex = vertexDistribution(sRandomEngine);
            int cls = classDistribution(sRandomEngine);

            if (solution.getVertexClass(vertex) != cls)
            {
                Solution newSolution = solution;
                newSolution.setVertexClass(vertex, cls);
                return newSolution;
            }
        }

        return solution;
    }

    std::pair<Solution, double> PickNDropNeighborhood::generateRandom(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const
    {
        std::uniform_int_distribution<int> vertexDistribution(0, solution.getNbVertices() - 1);
        std::uniform_int_distribution<int> classDistribution(0, solution.getNbClasses() - 1);

        constexpr int maxIterations = 1000;

        for (int iteration = 0; iteration < maxIterations; ++iteration)
        {
            int vertex = vertexDistribution(sRandomEngine);
            int cls = classDistribution(sRandomEngine);

            if (solution.getVertexClass(vertex) != cls)
            {
                Solution newSolution = solution;
                double newSolutionCost = solutionCost + graph.getSolutionCostDifference(solution, vertex, cls);
                newSolution.setVertexClass(vertex, cls);

                if (criterion->evaluate(graph, newSolution))
                {
                    return std::pair<Solution, double>(newSolution, newSolutionCost);
                }
            }
        }

        return std::pair<Solution, double>(solution, solutionCost);
    }

    std::pair<Solution, double> PickNDropNeighborhood::generateBest(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion) const
    {
        Solution bestSolution = solution;
        double bestSolutionCost = solutionCost;

        int previousModifiedVertex = 0;

        for (int vertex = 0; vertex < solution.getNbVertices(); vertex++)
        {
            for (int cls = 0; cls < solution.getNbClasses(); cls++)
            {
                if (solution.getVertexClass(vertex) == cls)
                {
                    continue;
                }

                double newSolutionCost = solutionCost + graph.getSolutionCostDifference(solution, vertex, cls);

                if (newSolutionCost < bestSolutionCost)
                {
                    int previousModifiedVertexClass = bestSolution.getVertexClass(previousModifiedVertex);

                    bestSolution.setVertexClass(previousModifiedVertex, solution.getVertexClass(previousModifiedVertex));

                    bestSolution.setVertexClass(vertex, cls);

                    if (criterion->evaluate(graph, bestSolution))
                    {
                        bestSolutionCost = newSolutionCost;

                        previousModifiedVertex = vertex;
                    }
                    else
                    {
                        bestSolution.setVertexClass(vertex, solution.getVertexClass(vertex));
                        bestSolution.setVertexClass(previousModifiedVertex, previousModifiedVertexClass);
                    }
                }
            }
        }

        return std::pair<Solution, double>(bestSolution, bestSolutionCost);
    }

    std::pair<Solution, double> PickNDropNeighborhood::generateBest(const Solution& solution, double solutionCost, const Graph& graph, std::shared_ptr<const Criterion> criterion, const std::list<Solution>& exceptions) const
    {
        Solution bestSolution = solution;
        double bestSolutionCost = solutionCost;

        bool isSet = false;

        int previousModifiedVertex = 0;

        for (int vertex = 0; vertex < solution.getNbVertices(); vertex++)
        {
            for (int cls = 0; cls < solution.getNbClasses(); cls++)
            {
                if (solution.getVertexClass(vertex) == cls)
                {
                    continue;
                }

                double currentSolutionCost = solutionCost + graph.getSolutionCostDifference(solution, vertex, cls);

                if (currentSolutionCost < bestSolutionCost || !isSet)
                {
                    int previousModifiedVertexClass = bestSolution.getVertexClass(previousModifiedVertex);

                    bestSolution.setVertexClass(previousModifiedVertex, solution.getVertexClass(previousModifiedVertex));
                    bestSolution.setVertexClass(vertex, cls);

                    bool isInExceptions = false;

                    bool isValid = criterion->evaluate(graph, bestSolution);
                    if (isValid)
                    {
                        isInExceptions = (std::find(exceptions.begin(), exceptions.end(), bestSolution) != exceptions.end());
                    }

                    if (isValid && !isInExceptions)
                    {
                        bestSolutionCost = currentSolutionCost;
                        previousModifiedVertex = vertex;
                        isSet = true;
                    }
                    else
                    {
                        bestSolution.setVertexClass(vertex, solution.getVertexClass(vertex));
                        bestSolution.setVertexClass(previousModifiedVertex, previousModifiedVertexClass);
                    }
                }
            }
        }

        return std::pair<Solution, double>(bestSolution, bestSolutionCost);
    }
} // namespace Yolo
