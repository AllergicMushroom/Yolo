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
        for (unsigned int i = 0; i < array.size(); ++i)
        {
            if(array[i] > elementPerClass){
                return false;
            }
        }
        
        return true;
    }

    bool cEqualDelta(std::vector<int> array, int nbVertices, int to, int delta){
        if(to == -1){
            to = array.size();
        }
        int elementPerClass = floor(nbVertices / array.size());
        for (unsigned int i = 0; i < array.size(); ++i)
        {
            if(array[i] > elementPerClass + delta)
                return false;
        }
        
        return true;
    }

    bool cEqualPercentage(std::vector<int> array, int nbVertices, int to, double percentage){
        if(to == -1){
            to = array.size();
        }
        int elementPerClass = floor(nbVertices / array.size());
        for (unsigned int i = 0; i < array.size(); ++i)
        {
            if(static_cast<double>(array[i]) > elementPerClass*(1+percentage))
                return false;
        }
        
        return true;
    }
}