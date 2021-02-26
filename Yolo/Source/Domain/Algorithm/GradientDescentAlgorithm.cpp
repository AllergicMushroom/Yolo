#include "GradientDescentAlgorithm.hpp"

#include "Core/Logger/Logger.hpp"

#include <sstream>

namespace Yolo
{
    GradientDescentAlgorithm::GradientDescentAlgorithm(const Graph& graph, int nbClasses, const Criterion* criterion, const Neighborhood* neighborhood, double epsilon)
        : Algorithm(graph, nbClasses, criterion)
    {
        mNeighborhood = neighborhood;
        mEpsilon = epsilon;
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
