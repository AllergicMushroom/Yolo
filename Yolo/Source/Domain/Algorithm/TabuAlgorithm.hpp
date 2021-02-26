#pragma once

#include "Domain/Algorithm/Algorithm.hpp"

#include "Domain/Neighborhood/Neighborhood.hpp"

#include <list>

namespace Yolo
{
    class TabuAlgorithm : public Algorithm
    {
    public:
        TabuAlgorithm(const Graph& graph, int nbClasses, const Criterion* criterion, const Neighborhood* neighborhood, int tabuListSize, int maxIterations, bool storeAll, bool aspiration);

        virtual inline std::string getName() const override { return "Tabu"; }
        virtual std::string getDetails() const override;

        virtual std::optional<Solution> solve() override;

    private:
        Solution solve(Solution initialSolution);

        Solution generateValidSolution();

    protected:
        const Neighborhood* mNeighborhood;

        std::list<Solution> mTabuList;
        int mTabuListSize;

        int mMaxIterations;
        bool mStoreAll;
        bool mAspiration;

        Solution mActualSolution;
        double mActualSolutionCost = 0.0f;
    };
} // namespace Yolo
