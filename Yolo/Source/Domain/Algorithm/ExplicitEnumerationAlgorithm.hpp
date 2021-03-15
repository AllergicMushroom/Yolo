#pragma once

#include "Algorithm.hpp"

namespace Yolo
{
    class ExplicitEnumerationAlgorithm : public Algorithm
    {
    public:
        ExplicitEnumerationAlgorithm(const Graph& graph, int nbClasses);

        virtual inline std::string getName() const override { return "Explicit Enumeration"; }

        virtual std::optional<Solution> solve() override;

    private:
        void enumerateFrom(Solution& solution, int vertex, double cost);

    private:
        Solution mBestSolution;
        bool mIsBestSolutionValid;
        double mBestSolutionCost;
    };
} // namespace Yolo
