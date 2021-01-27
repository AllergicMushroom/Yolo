#include "Criterion.hpp"

namespace Yolo
{
    bool cAll(std::vector<int> array, int nbVertices, int to){
        return true;
    }

    bool cEqual(std::vector<int> array, int nbVertices, int to){
        if(to == -1){
            to = array.size();
        }
        int elementPerClass = nbVertices / array.size();
        for (int i = 0; i < to; ++i)
        {
            if(array[i] > elementPerClass)
                return false;
        }
        
        return true;
    }
}