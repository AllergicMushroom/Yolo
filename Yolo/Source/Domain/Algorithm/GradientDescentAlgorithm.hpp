#pragma once

#include "Domain/Algorithm/Algorithm.hpp"

#include "Domain/Neighborhood/Neighborhood.hpp"

namespace Yolo
{
    class GradientDescentAlgorithm : public Algorithm
    {
    public:
        GradientDescentAlgorithm(const Graph& graph, int nbClasses, double epsilon, const Neighborhood* neighborhood, const Criterion* criterion)
            : Algorithm(graph, nbClasses, criterion), mActualSolution(Solution(mGraph.getNbVertices(), mNbClasses))
        {
            mNeighborhood = neighborhood;
            mEpsilon = epsilon;
        }

        virtual Solution solve() override;

        std::string getName() override { return "Gradient Descent"; }

    private:
        Solution solve(Solution initialSolution);

        Solution generateValidSolution();

    protected:
        const Neighborhood* mNeighborhood;

        Solution mActualSolution;
        double mActualSolutionCost = 0.0f;

        double mEpsilon;
    };
} // namespace Yolo
