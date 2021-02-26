#include "SimulatedAnnealingAlgorithm.hpp"

#include "Core/Logger/Logger.hpp"

#include <random>

namespace Yolo
{
    SimulatedAnnealingAlgorithm::SimulatedAnnealingAlgorithm(const Graph& graph, int nbClasses, const Criterion* criterion, const Neighborhood* neighborhood, int maxIterations)
        : Algorithm(graph, nbClasses, criterion)
    {
        mNeighborhood = neighborhood;
        mMaxIterations = maxIterations;
    }

    std::string SimulatedAnnealingAlgorithm::getDetails() const
    {
        // Todo
        return std::string();
    }

    std::optional<Solution> SimulatedAnnealingAlgorithm::solve()
    {
        std::optional<Solution> currentSolutionOpt = mCriterion->generateInitialSolution(mGraph, mNbClasses);
        if (!currentSolutionOpt.has_value())
        {
            YOLO_DEBUG("SimulatedAnnealingAlgorithm::solve(): Infeasible instance.\n");
            return std::nullopt;
        }

        Solution currentSolution = *currentSolutionOpt;
        double currentSolutionCost = mGraph.getSolutionCost(currentSolution);

        Solution bestSolution = currentSolution;
        double bestSolutionCost = currentSolutionCost;

        double T = 1000; // Todo: We can improve this.

        std::random_device randomDevice;
        std::mt19937_64 randomEngine(randomDevice());
        std::uniform_real_distribution<double> randomRealDistribution(0, 1);

        int iteration1 = 0;
        while (iteration1 < mMaxIterations)
        {
            int iteration2 = 0;
            while (iteration2 < mGraph.getNbVertices() * mGraph.getNbVertices())
            {
                Solution nextSolution = mNeighborhood->generateRandom(currentSolution, mGraph, mCriterion);
                double nextSolutionCost = mGraph.getSolutionCost(nextSolution);

                if (nextSolutionCost < currentSolutionCost)
                {
                    currentSolution = nextSolution;
                    currentSolutionCost = nextSolutionCost;
                    if (nextSolutionCost < bestSolutionCost)
                    {
                        bestSolution = currentSolution;
                        bestSolutionCost = currentSolutionCost;
                    }
                }
                else
                {
                    double p = randomRealDistribution(randomEngine);

                    double threshold = exp(-(nextSolutionCost - currentSolutionCost) / T);

                    if (p <= threshold)
                    {
                        currentSolution = nextSolution;
                        currentSolutionCost = nextSolutionCost;
                    }
                }

                ++iteration2;
            }

            ++iteration1;
            T *= 0.911; // Todo: We can improve on this.
        }

        return bestSolution;
    }
} // namespace Yolo
