#pragma once

#include <cmath>

#include "Domain/Solution/Solution.hpp"

namespace Yolo
{
    class Neighborhood
    {
    public:
        Neighborhood() {}

        virtual std::vector<Solution> generate(Solution solution) const = 0;
    };
} // namespace Yolo
