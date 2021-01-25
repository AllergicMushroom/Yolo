#pragma once

#include "Domain/Algorithm/Criterion.hpp"
namespace Yolo
{
    static bool cAll(std::vector<int> array, int nbVertices, int to = -1){
        return true;
    }

    static bool cEqual(std::vector<int> array, int nbVertices, int to = -1){
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