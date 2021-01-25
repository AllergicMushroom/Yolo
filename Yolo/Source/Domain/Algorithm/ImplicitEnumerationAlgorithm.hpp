#pragma once

#include "Algorithm.hpp"

namespace Yolo
{
    class ImplicitEnumerationAlgorithm : public Algorithm
    {
    public:
        ImplicitEnumerationAlgorithm(const Graph& graph, int nbClasses, bool (*criterion)(std::vector<int>, int, int)) : Algorithm(graph, nbClasses, criterion){}

        virtual Solution solve() override{
            enumerateFrom(mBest, 0);
            return mBest;
        }

        void enumerateFrom(Solution sol, int from){
            if(from == mGraph.getNbVertices())
            {
                compareBest(sol);
                return;
            }

            for (int i = 0; i < mNbClasses; ++i)
            {
                sol.setVertexClass(from, i);
                if(mGraph.isPartialSolutionValid(sol, mCriterion, from))
                    enumerateFrom(sol, from+1);
            }
        }

        void compareBest(Solution sol){
            // std::cout << sol.toString()<<"\n";
            if(mGraph.isValid(sol, mCriterion)){
                
                if(!mGraph.isValid(mBest, mCriterion)){
                    mBest = sol.clone();
                    return;
                }
                if(mGraph.getSolutionCost(sol) < mGraph.getSolutionCost(mBest))
                    mBest = sol.clone();
                }
        }

    private:
        Solution mBest = Solution(mGraph.getNbVertices(), mNbClasses);
    };
}