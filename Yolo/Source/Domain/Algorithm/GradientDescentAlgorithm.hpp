#pragma once

#include "Domain/Algorithm/Algorithm.hpp"

#include "Domain/Neighborhood/Neighborhood.hpp"

namespace Yolo
{
    class GradientDescentAlgorithm : public Algorithm
    {
    public:
        GradientDescentAlgorithm(const Graph& graph, int nbClasses);

        virtual inline std::string getName() const override { return "Gradient Descent"; }
        virtual std::string getDetails() const override;

        inline void setNeighborhood(std::shared_ptr<const Neighborhood> neighborhood) { mNeighborhood = neighborhood; }
        inline void setMaxIterations(int maxIterations) { mMaxIterations = std::max(1, maxIterations); }
        inline void setEpsilon(double epsilon) { mEpsilon = std::max(0.0, epsilon); }

        virtual std::optional<Solution> solve() override;

    protected:
        std::shared_ptr<const Neighborhood> mNeighborhood;

        double mEpsilon;

        int mMaxIterations;

        int mIterationCount;
    };
} // namespace Yolo
