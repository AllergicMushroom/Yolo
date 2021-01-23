#pragma once

#include <vector>

namespace Yolo
{
    class Solution
    {
    public:
        Solution(int nbClasses) : mSolution(std::vector<int>(nbClasses)) {}

    private:
        std::vector<int> mSolution;
    };
}