#pragma once

#include "Domain/Algorithm/Algorithm.hpp"

#include "Domain/Neighborhood/Neighborhood.hpp"

namespace Yolo
{
    class GradientDescentAlgorithm : public Algorithm
    {
    public:
        GradientDescentAlgorithm(const Graph& graph, int nbClasses, const Criterion* criterion, const Neighborhood* neighborhood, double epsilon);

        virtual inline std::string getName() const override { return "Gradient Descent"; }
        virtual std::string getDetails() const override;

        virtual std::optional<Solution> solve() override;

    protected:
        const Neighborhood* mNeighborhood;

        double mEpsilon;

        int mIterationCount;
    };
} // namespace Yolo
