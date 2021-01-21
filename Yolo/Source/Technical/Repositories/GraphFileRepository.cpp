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

        std::optional<std::string> currentLine = fileReader.getNextLine();
        while (currentLine.has_value())
        {
            std::string& lineString = *currentLine;

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
        const auto checkPositivity = [&isGraphValid](int x, const std::string& msg)
        {
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
            checkPositivity(tokenizedLines[lineIndex].size() - sNbEdgeProperties, "Error: Missing a property in arc in line " + std::to_string(lineIndex) + std::string(" in instance ") + ID);

            int source = std::stoi(tokenizedLines[lineIndex][0]);
            checkPositivity(source, "Error: Arc in line #" + std::to_string(lineIndex) + std::string(" in instance ") + ID + std::string(" must have a strictly positive source vertex."));
            checkPositivity(nbVertices - source, "Error: Arc in line #" + std::to_string(lineIndex) + std::string(" in instance ") + ID + std::string(" must be lower than or equal to number of vertices."));

            int destination = std::stoi(tokenizedLines[lineIndex][1]);
            checkPositivity(destination, "Error: Arc in line #" + std::to_string(lineIndex) + std::string(" in instance ") + ID + std::string(" must have a strictly positive destination vertex."));
            checkPositivity(nbVertices - destination, "Error: Arc in line #" + std::to_string(lineIndex) + std::string(" in instance ") + ID + std::string(" must be lower than or equal to number of vertices."));

            double weight = stod(tokenizedLines[lineIndex][2]);
            checkPositivity(source, "Error: Arc in line #" + std::to_string(lineIndex) + std::string(" in instance ") + ID + std::string(" must have a positive weight."));

            if (!isGraphValid)
            {
                return std::nullopt;
            }

            /* Vertices indices from [1, n] to [0, n-1] */
            --source;
            --destination;

            adjacencyList[source].push_back(Edge(source, destination, weight));
            adjacencyList[destination].push_back(Edge(destination, source, weight));
        }

        for (int i = 0; i < adjacencyList.size(); ++i)
        {
            int lineIndex = i + nbEdges + sNbSpecialLines;
            checkPositivity(tokenizedLines[lineIndex].size() - sNbVertexProperties, "Error: Missing a property in vertex in line " + std::to_string(i) + std::string(" in instance ") + ID);

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
}