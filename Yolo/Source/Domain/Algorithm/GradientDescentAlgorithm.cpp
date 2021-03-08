#include "GradientDescentAlgorithm.hpp"

#include "Core/Logger/Logger.hpp"

#include "Domain/Neighborhood/PickNDropNeighborhood.hpp"

#include <sstream>

static const Yolo::PickNDropNeighborhood sDefaultNeighborhood;
static constexpr double sDefaultEpsilon = 0.01;
static constexpr int sDefaultMaxIterations = 1000;

namespace Yolo
{
    GradientDescentAlgorithm::GradientDescentAlgorithm(const Graph& graph, int nbClasses)
        : Algorithm(graph, nbClasses)
    {
        mNeighborhood = &sDefaultNeighborhood;
        mEpsilon = sDefaultEpsilon;
        mMaxIterations = sDefaultMaxIterations;

        mIterationCount = 0;
    }

    std::optional<Solution> GradientDescentAlgorithm::solve()
    {
        std::optional<Solution> currentSolutionOpt = mCriterion->generateInitialSolution(mGraph, mNbClasses);
        if (!currentSolutionOpt.has_value())
        {
            YOLO_DEBUG("GradientDescentAlgorithm::solve(): Infeasible instance.\n");
            return std::nullopt;
        }

        Solution currentSolution = *currentSolutionOpt;
        double currentSolutionCost = mGraph.getSolutionCost(currentSolution);

        Solution bestNeighbor = mNeighborhood->generateBest(currentSolution, mGraph, mCriterion);
        double bestNeighborCost = mGraph.getSolutionCost(bestNeighbor);

        while (abs(currentSolutionCost - bestNeighborCost) > mEpsilon)
        {
            currentSolution = bestNeighbor;
            currentSolutionCost = bestNeighborCost;

            bestNeighbor = mNeighborhood->generateBest(currentSolution, mGraph, mCriterion);
            bestNeighborCost = mGraph.getSolutionCost(bestNeighbor);

            ++mIterationCount;
        }

        return currentSolution;
    }

    std::string GradientDescentAlgorithm::getDetails() const
    {
        std::stringstream ss;
        ss << "\n\tNumber of iterations: " << mIterationCount;
        return ss.str();
    }
} // namespace Yolo
