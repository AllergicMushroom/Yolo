#include "Graph.hpp"

#include "Core/Logger/Logger.hpp"

#include <random>

namespace Yolo
{
    Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree)
    {
        if (nbVertices < 0)
        {
            YOLO_ERROR("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): nbVertices must be positive.");
            nbVertices = 0;
        }

        if (nbEdges < 0)
        {
            YOLO_ERROR("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): nbEdges must be positive.");
            nbEdges = 0;
        }

        if (minDegree < 0)
        {
            YOLO_ERROR("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): minDegree must be positive.");
            minDegree = 0;
        }

        if (maxDegree < 0)
        {
            YOLO_ERROR("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): maxDegree must be positive.");
            maxDegree = 0;
        }

        mAdjacencyList = std::vector<std::vector<Edge>>(nbVertices);
        mVerticesDegrees = std::vector<int>(nbVertices);

        mNbEdges = nbEdges;

        mMinDegree = minDegree;
        mMaxDegree = maxDegree;

        /* Zero and one vertex-graphs cannot have edges. */
        if (nbVertices > 1)
        {
            int maxNbEdges = nbVertices * (nbVertices - 1) / 2;

            if (mNbEdges > maxNbEdges)
            {
                YOLO_WARN("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): Number of edges is too high for the number of vertices. {0} edges will be generated.", maxNbEdges);
                mNbEdges = maxNbEdges;
            }

            if (mMinDegree < 1)
            {
                YOLO_WARN("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): minDegree must be at least one. Setting it to 1.");
                mMinDegree = 1;
            }

            if (mMaxDegree > getNbVertices() - 1)
            {
                YOLO_WARN("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): maxDegree must be lower than or equal to nbVertices - 1. Setting it to {0}.", getNbVertices() - 1);
                mMaxDegree = getNbVertices() - 1;
            }

            const int nbMinEdges = mMinDegree * getNbVertices() / 2;
            if (mNbEdges < nbMinEdges)
            {
                YOLO_WARN("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): nbEdges must be higher than or equal to minDegree * nbVertices / 2. Setting it to {0}.", nbMinEdges);
                mNbEdges = mMinDegree * nbMinEdges;
            }

            const int nbMaxEdges = mMaxDegree * getNbVertices() / 2;
            if (mNbEdges > nbMaxEdges)
            {
                YOLO_WARN("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): nbEdges must be lower than or equal to maxDegree * nbVertices. Setting it to {0}.", nbMaxEdges);
                mNbEdges = nbMaxEdges;
            }

            if (mMaxDegree < mMinDegree)
            {
                YOLO_WARN("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): maxDegree must be higher than or equal to minDegree. Setting both of them to {0}.", mMinDegree);
                mMaxDegree = mMinDegree;
            }

            std::random_device randomDevice;
            std::mt19937_64 randomEngine(randomDevice());

            std::uniform_real_distribution<double> randomRealDistribution(0, 1);

            int currentNbEdges = 0;

            std::vector<int> underMinDegreeVertices(mAdjacencyList.size());
            for (int vertex = 0; vertex < mAdjacencyList.size(); ++vertex)
            {
                underMinDegreeVertices[vertex] = vertex;
            }

            std::vector<int> underMaxDegreeVertices(underMinDegreeVertices);

            std::shuffle(underMinDegreeVertices.begin(), underMinDegreeVertices.end(), randomEngine);
            std::shuffle(underMaxDegreeVertices.begin(), underMaxDegreeVertices.end(), randomEngine);

            /*
            * Todo
            * 
            * I'm sure there is a simpler and more efficient way to construct the graph.
            * I'll come back to this later.
            * 
            * Also it would seems MSVC compiler cannot optimize this code well.
            * 
            * - Luis
            */

            /* Generate the minimum edges per vertex */

            int minIndex = 0;
            int maxIndex = 0;
            while (underMinDegreeVertices.size() > 0)
            {
                int source = underMinDegreeVertices[minIndex];
                int destination = underMaxDegreeVertices[maxIndex];

                if (source != destination)
                {
                    if (std::find_if(mAdjacencyList[source].begin(), mAdjacencyList[source].end(), [destination](Edge edge) { return edge.getDestination() == destination; }) == mAdjacencyList[source].end())
                    {
                        double weight = randomRealDistribution(randomEngine);

                        Edge edge = Edge(source, destination, weight);
                        Edge reverseEdge = Edge(destination, source, weight);

                        mAdjacencyList[source].push_back(edge);
                        mAdjacencyList[destination].push_back(reverseEdge);
                        ++currentNbEdges;

                        ++mVerticesDegrees[source];
                        if (mVerticesDegrees[source] == mMinDegree)
                        {
                            underMinDegreeVertices.erase(underMinDegreeVertices.begin() + minIndex);
                        }

                        if (mMinDegree == mMaxDegree)
                        {
                            auto iterator = std::find_if(underMaxDegreeVertices.begin(), underMaxDegreeVertices.end(), [source](int vertex) { return vertex == source; });
                            if (iterator != underMaxDegreeVertices.end())
                            {
                                int iteratorIndex = static_cast<int>(iterator - underMaxDegreeVertices.begin());

                                underMaxDegreeVertices.erase(iterator);

                                if (iteratorIndex < maxIndex)
                                {
                                    --maxIndex;
                                }
                            }
                        }

                        ++mVerticesDegrees[destination];
                        if (mVerticesDegrees[destination] == mMaxDegree)
                        {
                            underMaxDegreeVertices.erase(underMaxDegreeVertices.begin() + maxIndex);
                        }

                        if (mVerticesDegrees[destination] == mMinDegree)
                        {
                            auto iterator = std::find_if(underMinDegreeVertices.begin(), underMinDegreeVertices.end(), [destination](int vertex) { return vertex == destination; });
                            if (iterator != underMinDegreeVertices.end())
                            {
                                underMinDegreeVertices.erase(iterator);
                            }
                        }

                        minIndex = (minIndex + 1) % std::max(static_cast<int>(underMinDegreeVertices.size()), 1);
                    }
                }

                maxIndex = (maxIndex + 1) % std::max(static_cast<int>(underMaxDegreeVertices.size()), 1);
            }

            /* Generate the others */
            while (currentNbEdges < mNbEdges)
            {
                /*
                * Todo:
                * 
                * This variable is a hotfix to avoid infinite loops because of the following problem:
                * 
                * Suppose we want a graph with vertices of degree 4.
                * Suppose the last vertices to create edges have degrees 3 3 2 3 3.
                * 
                * Then the loop could create an edge between the two couples of 3's and get stuck trying to create a loop.
                */
                bool hasEdgeBeenAdded = false;

                int sourceIndex = 0;
                int destinationIndex = 0;

                while (sourceIndex < underMaxDegreeVertices.size())
                {
                    while (destinationIndex < underMaxDegreeVertices.size())
                    {
                        if (sourceIndex != destinationIndex)
                        {
                            int source = underMaxDegreeVertices[sourceIndex];
                            int destination = underMaxDegreeVertices[destinationIndex];

                            /* Don't create paralel edges */
                            if (std::find_if(mAdjacencyList[source].begin(), mAdjacencyList[source].end(), [destination](Edge edge) { return edge.getDestination() == destination; }) == mAdjacencyList[source].end())
                            {

                                double weight = randomRealDistribution(randomEngine);
                                Edge edge = Edge(source, destination, weight);
                                Edge reverseEdge = Edge(destination, source, weight);

                                mAdjacencyList[source].push_back(edge);
                                mAdjacencyList[destination].push_back(reverseEdge);

                                hasEdgeBeenAdded = true;

                                ++mVerticesDegrees[source];
                                if (mVerticesDegrees[source] == mMaxDegree)
                                {
                                    underMaxDegreeVertices.erase(underMaxDegreeVertices.begin() + sourceIndex);
                                    if (sourceIndex < destinationIndex)
                                    {
                                        --destinationIndex;
                                    }
                                }

                                ++mVerticesDegrees[destination];
                                if (mVerticesDegrees[destination] == mMaxDegree)
                                {
                                    underMaxDegreeVertices.erase(underMaxDegreeVertices.begin() + destinationIndex);
                                }

                                ++currentNbEdges;
                            }
                        }

                        ++destinationIndex;

                        if (currentNbEdges >= mNbEdges)
                        {
                            break;
                        }
                    }

                    ++sourceIndex;

                    if (currentNbEdges >= mNbEdges)
                    {
                        break;
                    }
                }

                if (!hasEdgeBeenAdded)
                {
                    break;
                }
            }

            /* Recalculate min and max degrees */
            for (const auto& sourceAdjacents : mAdjacencyList)
            {
                mMinDegree = std::min(mMinDegree, static_cast<int>(sourceAdjacents.size()));
                mMaxDegree = std::max(mMaxDegree, static_cast<int>(sourceAdjacents.size()));
            }
        }
    }

    Graph::Graph(const std::vector<std::vector<Edge>>& adjacencyList)
        : mAdjacencyList(std::vector<std::vector<Edge>>(adjacencyList))
    {
        mVerticesDegrees = std::vector<int>(mAdjacencyList.size());
        mMaxDegree = static_cast<int>(mAdjacencyList[0].size());
        mMinDegree = static_cast<int>(mAdjacencyList[0].size());

        mNbEdges = 0;

        for (unsigned int i = 0; i < mAdjacencyList.size(); ++i)
        {
            mVerticesDegrees[i] = static_cast<int>(mAdjacencyList[i].size());
            if (mVerticesDegrees[i] > mMaxDegree)
                mMaxDegree = mVerticesDegrees[i];
            if (mVerticesDegrees[i] < mMinDegree)
                mMinDegree = mVerticesDegrees[i];

            mNbEdges += static_cast<int>(mAdjacencyList[i].size());
        }

        mNbEdges /= 2; /* Undirected graph */
    }

    bool Graph::operator==(const Graph& graph2) const
    {
        if (this->getNbVertices() != graph2.getNbVertices())
        {
            return false;
        }

        if (this->getNbEdges() != graph2.getNbEdges())
        {
            return false;
        }

        /* Check if there's a edge in the first graph that's not in the second one. */
        for (int source = 0; source < this->getNbVertices(); ++source)
        {
            for (int i = 0; i < this->mAdjacencyList[source].size(); ++i)
            {
                const Edge& edgeToCompare = this->mAdjacencyList[source][i];

                if (std::find_if(graph2.mAdjacencyList[source].begin(), graph2.mAdjacencyList[source].end(), [edgeToCompare](Edge edge) { return edge == edgeToCompare; }) == graph2.mAdjacencyList[source].end())
                {
                    return false;
                }
            }
        }

        // Todo: This is optimizable.
        /* Same thing but from second graph to first graph. */
        for (int source = 0; source < graph2.getNbVertices(); ++source)
        {
            for (int i = 0; i < graph2.mAdjacencyList[source].size(); ++i)
            {
                const Edge& edgeToCompare = graph2.mAdjacencyList[source][i];

                if (std::find_if(this->mAdjacencyList[source].begin(), this->mAdjacencyList[source].end(), [edgeToCompare](Edge edge) { return edge == edgeToCompare; }) == this->mAdjacencyList[source].end())
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool Graph::operator!=(const Graph& graph2) const
    {
        return !((*this) == graph2);
    }

    double Graph::getEdgeWeight(int source, int destination) const
    {
        auto iterator = std::find_if(mAdjacencyList[source].begin(), mAdjacencyList[source].end(), [destination](Edge edge) { return edge.getDestination() == destination; });

        if (iterator != mAdjacencyList[source].end())
        {
            return iterator->getWeight();
        }

        return 0;
    }

    double Graph::getSolutionCost(const Solution& solution) const
    {
        double cost = 0;

        for (unsigned int i = 0; i < mAdjacencyList.size(); ++i)
        {
            for (unsigned int j = 0; j < mAdjacencyList[i].size(); ++j)
            {
                if (solution.getVertexClass(mAdjacencyList[i][j].getSource()) != solution.getVertexClass(mAdjacencyList[i][j].getDestination()))
                {
                    cost += mAdjacencyList[i][j].getWeight();
                }
            }
        }

        /* Since the graph is inderected, we've gone through each arc twice. */
        return cost / 2;
    }

    double Graph::getSolutionCostDifference(const Solution& solution, int vertex, int newClass) const
    {
        int currentClass = solution.getVertexClass(vertex);
        double delta = 0.0;
        for (unsigned int j = 0; j < mAdjacencyList[vertex].size(); ++j)
        {
            int adjacentClass = solution.getVertexClass(mAdjacencyList[vertex][j].getDestination());
            if (adjacentClass == currentClass)
            {
                delta += mAdjacencyList[vertex][j].getWeight();
            }
            else if (adjacentClass == newClass)
            {
                delta -= mAdjacencyList[vertex][j].getWeight();
            }
        }
        return delta;
    }
} // namespace Yolo
