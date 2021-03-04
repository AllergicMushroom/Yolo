#include "GraphFileRepository.hpp"

#include "Core/Core.hpp"

#include "Domain/Graph/Graph.hpp"

#include "Technical/FileReader/FileReader.hpp"

#include <fstream>

/*
* nbVertices nbEdges
* minDegree maxDegree
*/
static constexpr int sNbSpecialLines = 2;

/*
* source destination weight
*/
static constexpr int sNbEdgeProperties = 3;

/*
* index degree
*/
static constexpr int sNbVertexProperties = 2;

namespace Yolo
{
    void GraphFileRepository::save(std::string ID, Graph graph)
    {
        std::ofstream file(ID);
        if (file.is_open())
        {
            file << "# nbVertices nbEdges\n";
            file << graph.getNbVertices() << ' ' << graph.getNbEdges() << '\n';

            file << "# minDegree maxDegree\n";
            file << graph.getMinDegree() << ' ' << graph.getMaxDegree() << '\n';

            file << "# source destination weight\n";
            for (int source = 0; source < graph.getNbVertices(); ++source)
            {
                for (int destination = source + 1; destination < graph.getNbVertices(); ++destination)
                {
                    double weight = graph.getEdgeWeight(source, destination);
                    if (weight != 0)
                    {
                        /* [0, n] to [1, n+1]*/
                        file << source + 1 << ' ' << destination + 1 << ' ' << weight << '\n';
                    }
                }
            }

            file << "# vertex degree\n";
            for (int vertex = 0; vertex < graph.getNbVertices(); ++vertex)
            {
                file << vertex + 1 << ' ' << graph.getVertexDegree(vertex) << '\n';
            }

            file.close();
        }
        else
        {
            YOLO_ERROR("{0}: Could not open file {1} for writing.\n", __func__, ID);
        }
    }

    std::optional<Graph> GraphFileRepository::load(std::string ID)
    {
        std::optional<Yolo::FileReader> fileOptional = FileReader::create(ID);

        if (!fileOptional.has_value())
        {
            return std::nullopt;
        }

        Yolo::FileReader& fileReader = *fileOptional;

        std::vector<std::vector<std::string>> tokenizedLines;

        std::string currentLine = fileReader.getNextLine();
        while (!currentLine.empty())
        {
            std::string& lineString = currentLine;

            const size_t firstCharIndex = lineString.find_first_not_of(" ");

            if (firstCharIndex != std::string::npos)
            {
                if (lineString[firstCharIndex] != '#')
                {
                    tokenizedLines.push_back(tokenizeString(lineString));
                }
            }

            currentLine = fileReader.getNextLine();
        }

        bool isGraphValid = true;
        const auto checkPositivity = [&isGraphValid](int x, const std::string& msg) {
            if (x < 0)
            {
                YOLO_ERROR(msg);
                isGraphValid = false;
            }
        };

        const int nbVertices = std::stoi(tokenizedLines[0][0]);
        checkPositivity(nbVertices, "Error: Number of vertices in instance " + ID + "  must be positive.");

        const int nbEdges = std::stoi(tokenizedLines[0][1]);
        checkPositivity(nbEdges, "Error: Number of edges in instance " + ID + "  must be positive.");

        const int minDegree = std::stoi(tokenizedLines[1][0]);
        checkPositivity(minDegree, "Error: Minimum degree in instance " + ID + "  must be positive.");

        const int maxDegree = std::stoi(tokenizedLines[1][1]);
        checkPositivity(maxDegree, "Error: Maximum degree in instance " + ID + "  must be positive.");

        checkPositivity(maxDegree - minDegree, "Error: Maximum degree must be higher than or equal to minimum degree in instance " + ID);

        if (!isGraphValid)
        {
            return std::nullopt;
        }

        std::vector<std::vector<Edge>> adjacencyList(nbVertices);

        for (int i = 0; i < nbEdges; ++i)
        {
            int lineIndex = i + sNbSpecialLines;
            checkPositivity(static_cast<int>(tokenizedLines[lineIndex].size()) - sNbEdgeProperties, "Error: Missing a property in arc in line " + std::to_string(lineIndex) + std::string(" in instance ") + ID);

            int source = std::stoi(tokenizedLines[lineIndex][0]);
            checkPositivity(source, "Error: Arc in line #" + std::to_string(lineIndex) + std::string(" in instance ") + ID + std::string(" must have a strictly positive source vertex."));
            checkPositivity(nbVertices - source, "Error: Arc in line #" + std::to_string(lineIndex) + std::string(" in instance ") + ID + std::string(" must be lower than or equal to number of vertices."));

            int destination = std::stoi(tokenizedLines[lineIndex][1]);
            checkPositivity(destination, "Error: Arc in line #" + std::to_string(lineIndex) + std::string(" in instance ") + ID + std::string(" must have a strictly positive destination vertex."));
            checkPositivity(nbVertices - destination, "Error: Arc in line #" + std::to_string(lineIndex) + std::string(" in instance ") + ID + std::string(" must be lower than or equal to number of vertices."));

            double weight = std::stod(tokenizedLines[lineIndex][2]);
            checkPositivity(source, "Error: Arc in line #" + std::to_string(lineIndex) + std::string(" in instance ") + ID + std::string(" must have a positive weight."));

            if (!isGraphValid)
            {
                return std::nullopt;
            }

            /* Vertices indices from [1, n] to [0, n-1] */
            --source;
            --destination;

            auto iterator = std::find_if(adjacencyList[source].begin(), adjacencyList[source].end(), [source, destination](Edge edge) { return edge.getSource() == source && edge.getDestination() == destination; });
            if (iterator != adjacencyList[source].end())
            {
                *iterator = Edge(source, destination, iterator->getWeight() + weight);
            }
            else
            {
                adjacencyList[source].push_back(Edge(source, destination, weight));
                adjacencyList[destination].push_back(Edge(destination, source, weight));
            }
        }

        for (unsigned int i = 0; i < adjacencyList.size(); ++i)
        {
            int lineIndex = i + nbEdges + sNbSpecialLines;
            checkPositivity(static_cast<int>(tokenizedLines[lineIndex].size()) - sNbVertexProperties, "Error: Missing a property in vertex in line " + std::to_string(i) + std::string(" in instance ") + ID);

            int source = stoi(tokenizedLines[lineIndex][0]);
            checkPositivity(source, "Error: Vertex in line #" + std::to_string(lineIndex) + std::string(" in instance ") + ID + std::string(" must have a strictly positive index."));
            checkPositivity(nbVertices - source, "Error: Vertex in line #" + std::to_string(lineIndex) + std::string(" in instance ") + ID + std::string(" must be lower than or equal to number of vertices."));

            int degree = stoi(tokenizedLines[lineIndex][1]);
            if (degree != adjacencyList[i].size())
            {
                YOLO_ERROR("Error: Vertex in line #{0}  in instance {1} has different degree than the one calculated from the adjacency list.", lineIndex, ID);
                return std::nullopt;
            }
        }

        if (!isGraphValid)
        {
            return std::nullopt;
        }

        return Graph(adjacencyList);
    }

    std::vector<std::string> GraphFileRepository::tokenizeString(std::string string) const
    {
        std::vector<std::string> elements;

        size_t position = 0;
        size_t previousPosition = 0;

        while (position != std::string::npos)
        {
            position = string.find_first_of(" ", previousPosition);

            elements.push_back(string.substr(previousPosition, position - previousPosition));

            previousPosition = position + 1;
        }

        return elements;
    }
} // namespace Yolo
