#pragma once

#include <cmath>

#include "Domain/Graph/Graph.hpp"

#include "Domain/Solution/Solution.hpp"

namespace Yolo
{
    bool cAll(std::vector<int> array, int nbVertices, int to);
    bool cEqual(std::vector<int> array, int nbVertices, int to);
    
    // must be called with lambda function.
    // ex: [](std::vector<int> array, int nbVertices, int to) { return Yolo::cEqualDelta(array, nbVertices, to, 1); })
    bool cEqualDelta(std::vector<int> array, int nbVertices, int to, int delta);
    bool cEqualPercentage(std::vector<int> array, int nbVertices, int to, double percentage);
}