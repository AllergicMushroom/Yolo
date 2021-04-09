#include "SimulatedAnnealingAlgorithm.hpp"

#include "Core/Logger/Logger.hpp"

#include "Domain/Neighborhood/PickNDropNeighborhood.hpp"

#include <random>

static const Yolo::PickNDropNeighborhood sDefaultNeighborhood;
static constexpr int sDefaultMaxIterations = 1000;
static constexpr double sDefaultTemperature = 1000;

namespace Yolo
{
    SimulatedAnnealingAlgorithm::SimulatedAnnealingAlgorithm(const Graph& graph, int nbClasses)
        : Algorithm(graph, nbClasses)
    {
        mNeighborhood = std::make_shared<PickNDropNeighborhood>(sDefaultNeighborhood);

        mMaxIterations = sDefaultMaxIterations;

        mTemperature = sDefaultTemperature;
    }

    std::string SimulatedAnnealingAlgorithm::getDetails() const
    {
        // Todo
        return std::string();
    }

    std::optional<Solution> SimulatedAnnealingAlgorithm::solve()
    {
        std::optional<Solution> currentSolutionOpt = mCriterion->generateInitialSolution(mGraph, mNbClasses);
        if (!currentSolutionOpt)
        {
            return std::nullopt;
        }

        Solution currentSolution = *currentSolutionOpt;
        double currentSolutionCost = mGraph.getSolutionCost(currentSolution);

        Solution bestSolution = currentSolution;
        double bestSolutionCost = currentSolutionCost;

        std::random_device randomDevice;
        std::mt19937_64 randomEngine(randomDevice());
        std::uniform_real_distribution<double> randomRealDistribution(0, 1);

        int iteration1 = 0;
        while (iteration1 < mMaxIterations)
        {
            int iteration2 = 0;
            while (iteration2 < mGraph.getNbVertices() * mGraph.getNbVertices())
            {
                auto [nextSolution, nextSolutionCost] = mNeighborhood->generateRandom(currentSolution, currentSolutionCost, mGraph, mCriterion);

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

                    double threshold = exp(-(nextSolutionCost - currentSolutionCost) / mTemperature);

                    if (p <= threshold)
                    {
                        currentSolution = nextSolution;
                        currentSolutionCost = nextSolutionCost;
                    }
                }

                ++iteration2;
            }

            ++iteration1;
            mTemperature *= 0.911; // Todo: We can improve on this.
        }

        return bestSolution;
    }
} // namespace Yolo
