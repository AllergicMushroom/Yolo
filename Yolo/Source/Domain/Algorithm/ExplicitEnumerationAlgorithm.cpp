#include "Domain/Algorithm/ExplicitEnumerationAlgorithm.hpp"

namespace Yolo
{
        Solution ExplicitEnumerationAlgorithm::solve(){
            enumerateFrom(mBest, 1); // we can fix the first class as the class of the first vertex.
            return mBest;
        }

        void ExplicitEnumerationAlgorithm::enumerateFrom(Solution sol, int from){
            if(from == mGraph.getNbVertices())
            {
                compareBest(sol);
                return;
            }

            for (int i = 0; i <= std::min(from, mNbClasses-1); ++i)
            {
                sol.setVertexClass(from, i);
                enumerateFrom(sol, from+1);
            }
        }

        void ExplicitEnumerationAlgorithm::compareBest(Solution sol){
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