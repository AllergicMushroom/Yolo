#pragma once

#include "Algorithm.hpp"

namespace Yolo
{
    class GradientDescentAlgorithm : public Algorithm
    {
    public:
        GradientDescentAlgorithm(const Graph& graph, int nbClasses, double epsilon, std::vector<Solution> (*neigh)(Solution sol), bool (*criterion)(std::vector<int>, int, int)) : Algorithm(graph, nbClasses, criterion), mActual(Solution(mGraph.getNbVertices(), mNbClasses)){
            mNeigh = neigh;
            mEpsilon = epsilon;
        }

        virtual Solution solve() override;
        virtual Solution solve(Solution initialSolution);

        Solution findBestNeighbour(Solution sol, bool real);
        Solution generateValidSolution();

        std::string getName() override {return "Gradient Descent";}

    protected:
        Solution mActual;
        double mActualCost = 0;
        double mEpsilon;
        std::vector<Solution> (*mNeigh)(Solution sol);
    };
}