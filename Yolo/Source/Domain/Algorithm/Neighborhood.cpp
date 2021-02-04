#include "Neighborhood.hpp"

namespace Yolo
{

    std::vector<Solution> pickNDrop(Solution sol){
        std::vector<Solution> neigh = std::vector<Solution>((sol.getNbVertices()*(sol.getNbClasses()-1)), Solution(0,0));
        // std::cout<<"pick n drop\n"<<(sol.getNbClasses());
        for (unsigned int i = 0; i < neigh.size(); i++)
        {
            for (int j = 0; j < sol.getNbClasses()-1; j++)
            {
                // std::cout<<i<<" "<<j<<" "<< i*sol.getNbClasses()+j<< " "<<neigh.size()<<" lol\n";
                Solution tmp = sol.clone();
                tmp.setVertexClass(i, (tmp.getVertexClass(i)+j+1)%tmp.getNbClasses());
                neigh[i*(sol.getNbClasses()-1)+j] = tmp;
                // std::cout<<tmp.toString()<<"\n";
            }
        }
        // std::cout<<"out\n";
        return neigh;
        
    }
}