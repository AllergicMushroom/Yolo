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

        virtual inline std::string getName() const override { return "Gradient Descent"; }
        virtual std::string getDetails() const override;

        virtual Solution solve() override;

    private:
        Solution solve(Solution initialSolution);

        Solution generateValidSolution();

    protected:
        const Neighborhood* mNeighborhood;

        Solution mActualSolution;
        double mActualSolutionCost = 0.0f;
        int mCount = 0;
        double mEpsilon;
    };
} // namespace Yolo
