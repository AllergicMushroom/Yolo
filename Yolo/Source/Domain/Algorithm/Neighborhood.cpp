#include "Neighborhood.hpp"

namespace Yolo
{

    std::vector<Solution> pickNDrop(Solution sol){
        std::vector<Solution> neigh = std::vector<Solution>((sol.getNbVertices()*(sol.getNbClasses()-1)), Solution(0,0));
        // std::cout<<"pick n drop\n"<<(sol.getNbClasses());
        for (int i = 0; i < sol.getNbVertices(); i++)
        {
            for (int j = 0; j < sol.getNbClasses()-1; j++)
            {
                Solution tmp = sol.clone();
                tmp.setVertexClass((std::floor((tmp.getVertexClass(i)+j+1)/tmp.getNbClasses())), (tmp.getVertexClass(i)+j+1)%tmp.getNbClasses());
                neigh[i*(sol.getNbClasses()-1)+j] = tmp;
            }
        }
        return neigh;
    }
}