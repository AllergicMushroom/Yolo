#pragma once

#include "Domain/Graph/Graph.hpp"

#include "Domain/Solution/Solution.hpp"

namespace Yolo
{
    static bool cAll(std::vector<int> array, int nbVertices, int to){
        return true;
    }

    static bool cEqual(std::vector<int> array, int nbVertices, int to){
        if(to == -1){
            to = array.size();
        }
        int elementPerClass = nbVertices / array.size();
        for (int i = 0; i < to; ++i)
        {
            if(array[i] != elementPerClass)
                return false;
        }
        
        return true;
    }
}