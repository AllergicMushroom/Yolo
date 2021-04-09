#include "Solution.hpp"

#include "Core/Logger/Logger.hpp"

namespace Yolo
{
    Solution::Solution(int nbVertices, int nbClasses)
    {
        mSolution = std::vector<int>(std::max(nbVertices, 0), sVertexNoClass);
        mClassCardinals = std::vector<int>(std::max(nbClasses, 0), 0);
    }

    void Solution::setVertexClass(int vertex, int vertexClass)
    {
        if (vertex < 0 || vertex > mSolution.size() - 1)
        {
            YOLO_DEBUG("Solution::setVertexClass(int vertex, int vertexClass): Changing out-of-bounds vertex' class, vertex == {0}", vertex);
            return;
        }

        if (vertexClass == sVertexNoClass)
        {
            if (mSolution[vertex] != sVertexNoClass)
            {
                --mClassCardinals[mSolution[vertex]];
            }

            mSolution[vertex] = vertexClass;
        }
        else
        {
            if (vertexClass < 0 || vertexClass > mClassCardinals.size() - 1)
            {
                YOLO_DEBUG("Solution::setVertexClass(int vertex, int vertexClass): Changing vertex {0} to forbidden class {1}.", vertex, vertexClass);
                return;
            }

            if (mSolution[vertex] != sVertexNoClass)
            {
                --mClassCardinals[mSolution[vertex]];
            }

            mSolution[vertex] = vertexClass;
            ++mClassCardinals[mSolution[vertex]];
        }
    }

    int Solution::getVertexClass(int vertex) const
    {
        if (vertex < 0 || vertex > mSolution.size() - 1)
        {
            YOLO_DEBUG("Solution::getVertexClass(int vertex): Querying class from out-of-bounds vertex: {0}", vertex);
            return sVertexNoClass;
        }

        return mSolution[vertex];
    }

    int Solution::getClassCardinal(int c) const
    {
        if (c < 0 || c > mClassCardinals.size() - 1)
        {
            YOLO_DEBUG("Solution::getClassCardinal(int c): Querying cardinal from out-of-bounds class: {0}", c);
            return 0;
        }

        return mClassCardinals[c];
    }

    bool Solution::operator==(const Solution& sol) const
    {
        if (sol.getNbVertices() != getNbVertices())
            return false;

        for (int i = 0; i < sol.getNbVertices(); i++)
        {
            if (sol.getVertexClass(i) != getVertexClass(i))
                return false;
        }
        return true;
    }

    std::string Solution::toString() const
    {
        std::string str = "";
        for (unsigned int i = 0; i < mSolution.size(); ++i)
        {
            str += std::to_string(mSolution[i]) + " ";
        }
        return str;
    }

} // namespace Yolo
