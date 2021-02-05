#include "PickNDropNeighborhood.hpp"

namespace Yolo
{
    std::vector<Solution> PickNDropNeighborhood::generate(Solution solution) const
    {
        /*
            We have to include the current solution so that convergence is guaranteed.
            We can cut nbVertices solutions because they're the same ones as the current solution.
        */
        size_t size = static_cast<size_t>(solution.getNbVertices()) * static_cast<size_t>(solution.getNbClasses() - 1) + 1;
        std::vector<Solution> neighbors(size, Solution(0, 0));

        int index = 0;
        neighbors[index++] = solution;

        for (int i = 0; i < solution.getNbVertices(); i++)
        {
            for (int j = 0; j < solution.getNbClasses(); j++)
            {
                if (solution.getVertexClass(i) == j)
                {
                    continue;
                }

                Solution temp = solution;
                temp.setVertexClass(i, j);
                neighbors[index++] = temp;
                //neighbors[i * (solution.getNbClasses() - 1) + j] = temp;
            }
        }

        return neighbors;
    }
} // namespace Yolo
