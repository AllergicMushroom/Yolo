#pragma once

#include "Domain/Algorithm/Algorithm.hpp"

#include "Domain/Neighborhood/Neighborhood.hpp"

#include <list>

namespace Yolo
{
    class TabuListAlgorithm : public Algorithm
    {
    public:
        TabuListAlgorithm(const Graph& graph, int nbClasses);

        virtual inline std::string getName() const override { return "Tabu"; }
        virtual std::string getDetails() const override;

        inline void setNeighborhood(const Neighborhood* neighborhood) { mNeighborhood = neighborhood; }
        inline void setMaxIterations(int maxIterations) { mMaxIterations = std::max(1, maxIterations); }
        inline void setTabuListSize(int tabuListSize) { mTabuListSize = std::max(1, tabuListSize); }
        inline void setStoreAll(bool storeAll) { mStoreAll = storeAll; }

        virtual std::optional<Solution> solve() override;

    private:
        Solution solve(Solution initialSolution);

    protected:
        const Neighborhood* mNeighborhood;

        // Todo: Look at Deque
        std::list<Solution> mTabuList;
        int mTabuListSize;

        int mMaxIterations;
        bool mStoreAll;
        bool mAspiration;

        Solution mActualSolution;
        double mActualSolutionCost = 0.0f;
    };
} // namespace Yolo
