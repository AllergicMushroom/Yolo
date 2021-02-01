#include "ImplicitEnumerationAlgorithm.hpp"

namespace Yolo
{
    Solution ImplicitEnumerationAlgorithm::solve(){
        enumerateFrom(mBest, 0);
        return mBest;
    }

    void ImplicitEnumerationAlgorithm::enumerateFrom(Solution sol, int from){
        if(from == mGraph.getNbVertices())
        {
            compareBest(sol);
            return;
        }

        for (int i = 0; i <= std::min(from, mNbClasses-1); ++i)
        {
            sol.setVertexClass(from, i);
            if(mGraph.isPartialSolutionValid(sol, mCriterion, from))
                enumerateFrom(sol, from+1);
        }
    }

    void ImplicitEnumerationAlgorithm::compareBest(Solution sol){
        if(mGraph.isValid(sol, mCriterion)){
            
            if(!mGraph.isValid(mBest, mCriterion)){
                mBest = sol.clone();
                return;
            }
            if(mGraph.getSolutionCost(sol) < mGraph.getSolutionCost(mBest))
                mBest = sol.clone();
            }
    }
}