#pragma once

#include "Domain/Algorithm/Algorithm.hpp"

#include "Domain/Neighborhood/Neighborhood.hpp"
#include <list>
 
namespace Yolo
{
    class TabuAlgorithm : public Algorithm
    {
    public:
        TabuAlgorithm(const Graph& graph, int nbClasses, int tabuSize, int iterMax, bool each, bool aspiration, const Neighborhood* neighborhood, const Criterion* criterion)
            : Algorithm(graph, nbClasses, criterion), mActualSolution(Solution(mGraph.getNbVertices(), mNbClasses))
        {
            mTabu = std::list<Solution>();
            mTabuSize = tabuSize;
            mNeighborhood = neighborhood;
            mEach = each;
            mAspiration = aspiration;
            mIterMax = iterMax;
        }

        virtual Solution solve() override;

        std::string getName() override { return "Tabu"; }

    private:
        Solution solve(Solution initialSolution);

        Solution generateValidSolution();
        
    protected:
        size_t mTabuSize;
        const Neighborhood* mNeighborhood;
        int mIterMax;
        std::list<Solution> mTabu;
        bool mEach;
        bool mAspiration;
        Solution mActualSolution;
        double mActualSolutionCost = 0.0f;
    };
} // namespace Yolo
