#include "Graph.hpp"

#include "Core/Logger/Logger.hpp"

#include <random>

namespace Yolo
{
    Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree)
        : mAdjacencyList(std::vector<std::vector<Edge>>(nbVertices)), mVerticesDegrees(std::vector<int>(nbVertices))
    {
        int maxNbEdges = nbVertices * (nbVertices - 1);

        mNbEdges = nbEdges;
        if (nbEdges > maxNbEdges)
        {
            YOLO_WARN("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): Number of edges is too high for the number of vertices. {0} edges will be generated.", maxNbEdges);
            mNbEdges = maxNbEdges;
        }

        mMinDegree = minDegree;
        if (minDegree < 1)
        {
            YOLO_WARN("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): minDegree must be strictly positive. Setting it to 1.");
            mMinDegree = 1;
        }

        mMaxDegree = maxDegree;
        if (maxDegree > nbVertices - 1)
        {
            YOLO_WARN("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): maxDegree must be lower than or equal to nbVertices - 1. Setting it to {0}.", nbVertices - 1);
            mMaxDegree = nbVertices - 1;
        }

        if (mMinDegree > mMaxDegree)
        {
            YOLO_WARN("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): maxDegree must be higher than or equal to minDegree. Setting both of them to {0}.", std::min(mMinDegree, mMaxDegree));
            mMinDegree = std::min(mMinDegree, mMaxDegree);
            mMaxDegree = mMinDegree;
        }

        if (mNbEdges < mMinDegree * nbVertices / 2)
        {
            YOLO_WARN("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): nbEdges must be higher than or equal to minDegree * nbVertices. Setting it to {0}.", minDegree * nbVertices / 2);
            mNbEdges = mMinDegree * nbVertices / 2;
        }

        if (mNbEdges > mMaxDegree * nbVertices / 2)
        {
            YOLO_WARN("Graph::Graph(int nbVertices, int nbEdges, int minDegree, int maxDegree): nbEdges must be lower than or equal to maxDegree * nbVertices. Setting it to {0}.", maxDegree * nbVertices / 2);
            mNbEdges = mMaxDegree * nbVertices / 2;
        }

        std::random_device randomDevice;
        std::mt19937_64 randomEngine(randomDevice());

        std::uniform_real_distribution<double> randomRealDistribution(0, 1);

        int currentNbEdges = 0;

        /* Generate the minimum edges per vertex */
        for (int source = 0; source < mAdjacencyList.size(); ++source)
        {
            std::vector<int> underMinDegreeVertices;
            for (int vertex = 0; vertex < mAdjacencyList.size(); ++vertex)
            {
                /* No loops */
                if (source == vertex)
                {
                    continue;
                }

                if (mAdjacencyList[vertex].size() < static_cast<size_t>(mMinDegree))
                {
                    underMinDegreeVertices.push_back(vertex);
                }
            }

            std::shuffle(underMinDegreeVertices.begin(), underMinDegreeVertices.end(), randomEngine);

            for (int destination : underMinDegreeVertices)
            {
                if (mVerticesDegrees[source] >= mMinDegree)
                {
                    break;
                }

                /* Don't create paralel edges */
                if (std::find_if(mAdjacencyList[source].begin(), mAdjacencyList[source].end(), [destination](Edge edge) { return edge.getDestination() == destination; }) != mAdjacencyList[source].end())
                {
                    continue;
                }

                double weight = randomRealDistribution(randomEngine);

                Edge edge = Edge(source, destination, weight);
                Edge reverseEdge = Edge(destination, source, weight);

                mAdjacencyList[source].push_back(edge);
                mAdjacencyList[destination].push_back(reverseEdge);
                ++mVerticesDegrees[source];
                ++mVerticesDegrees[destination];
                ++currentNbEdges;
            }
        }

        /* Generate the others */
        while (currentNbEdges < mNbEdges)
        {
            std::vector<int> underMaxDegreeVertices;
            for (int vertex = 0; vertex < mAdjacencyList.size(); ++vertex)
            {
                if (mAdjacencyList[vertex].size() < static_cast<size_t>(mMaxDegree))
                {
                    underMaxDegreeVertices.push_back(vertex);
                }
            }

            std::shuffle(underMaxDegreeVertices.begin(), underMaxDegreeVertices.end(), randomEngine);

            for (size_t vertex = 0; vertex < underMaxDegreeVertices.size() - 1; vertex += 2)
            {
                double weight = randomRealDistribution(randomEngine);

                int source = underMaxDegreeVertices[vertex];
                int destination = underMaxDegreeVertices[vertex + 1];

                Edge edge = Edge(source, destination, weight);
                Edge reverseEdge = Edge(destination, source, weight);

                mAdjacencyList[source].push_back(edge);
                mAdjacencyList[destination].push_back(reverseEdge);
                ++mVerticesDegrees[source];
                ++mVerticesDegrees[destination];
                ++currentNbEdges;

                if (currentNbEdges >= mNbEdges)
                {
                    break;
                }
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

    CheckerOutput Graph::checkSolution(const Solution& solution, bool(std::vector<int>, int, int)) const
    {
        // TODO
        return {false, getSolutionCost(solution)};
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
