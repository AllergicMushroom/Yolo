#include "Domain/Algorithm/ExplicitEnumerationAlgorithm.hpp"

namespace Yolo
{
         Solution ExplicitEnumerationAlgorithm::solve(){
            enumerateFrom(mBest, 0);
            return mBest;
        }

        void ExplicitEnumerationAlgorithm::enumerateFrom(Solution sol, int from){
            if(from == mGraph.getNbVertices())
            {
                compareBest(sol);
                return;
            }

            for (int i = 0; i < mNbClasses; ++i)
            {
                sol.setVertexClass(from, i);
                enumerateFrom(sol, from+1);
            }
        }

        void ExplicitEnumerationAlgorithm::compareBest(Solution sol){
            if(mGraph.isValid(sol, mCriterion)){
                std::cout << sol.toString()<<"\n";
                
                if(!mGraph.isValid(mBest, mCriterion)){
                    mBest = sol.clone();
                    return;
                }
                if(mGraph.getSolutionCost(sol) < mGraph.getSolutionCost(mBest))
                    mBest = sol.clone();
                }
        }

}