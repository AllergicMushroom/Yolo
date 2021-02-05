#include "Domain/Algorithm/GradientDescentAlgorithm.hpp"

namespace Yolo
{
        Solution GradientDescentAlgorithm::solve(){
            Solution initSol = generateValidSolution();
            return GradientDescentAlgorithm::solve(initSol);
        }

        Solution GradientDescentAlgorithm::solve(Solution initialSolution){
            mActual = initialSolution;
            bool isValid = mGraph.isValid(initialSolution, mCriterion);
            if(!isValid)
                printf("\nSolution not valid in gradient descent algo.\n");
            mActualCost = mGraph.getSolutionCost(initialSolution);

            Solution bestNeigh = findBestNeighbour(mActual, true);
            double bestNeighCost = mGraph.getSolutionCost(bestNeigh);

            while(mActualCost - bestNeighCost > mEpsilon )
            {
                mActual = bestNeigh;
                mActualCost = bestNeighCost;

                bestNeigh = findBestNeighbour(mActual, true);
                bestNeighCost = mGraph.getSolutionCost(bestNeigh);

            }
            return mActual;
        }
        Solution GradientDescentAlgorithm::findBestNeighbour(Solution sol, bool real){
            std::vector<Solution> neighboroud = mNeigh(sol);
            double cost;
            Solution best = sol;
            bool isSet = false;

            for (unsigned int i = 0; i < neighboroud.size(); i++)
            {
                if(!real || mGraph.isValid(neighboroud[i], mCriterion)){

                    if(!isSet){
                        cost = mGraph.getSolutionCost(neighboroud[i]);
                        best = neighboroud[i];
                        isSet = true;
                    }
                    else {
                        double newCost = mGraph.getSolutionCost(neighboroud[i]);
                        if(newCost < cost){
                            cost = newCost;
                            best = neighboroud[i];
                        }
                    }
                }
            }
            return best;
        }

        Solution GradientDescentAlgorithm::generateValidSolution(){
            Solution sol = Solution(mGraph.getNbVertices(), mNbClasses);
            for (int i = 0; i < mGraph.getNbVertices(); i++)
            {
                sol.setVertexClass(i, i%mNbClasses);
            }
            return sol;
        }
}