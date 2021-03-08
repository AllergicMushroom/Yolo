#pragma once

#include "Algorithm.hpp"

namespace Yolo
{
    class ImplicitEnumerationAlgorithm : public Algorithm
    {
    public:
        ImplicitEnumerationAlgorithm(const Graph& graph, int nbClasses);

        virtual inline std::string getName() const override { return "Implicit Enumeration"; }

        virtual std::optional<Solution> solve() override;

    private:
        void enumerateFrom(Solution solution, int vertex, double cost);

    private:
        Solution mBestSolution;
        bool mIsBestSolutionValid;
        double mBestSolutionCost;
    };
} // namespace Yolo
