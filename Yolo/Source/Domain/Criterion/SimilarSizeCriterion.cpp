#include "SimilarSizeCriterion.hpp"

#include "Core/Logger/Logger.hpp"

namespace Yolo
{
    SimilarSizeCriterion::SimilarSizeCriterion(int maxSlack)
        : Criterion(), mUseIntSlack(true)
    {
        mIntSlack = std::max(0, maxSlack);
    }

    SimilarSizeCriterion::SimilarSizeCriterion(double percentage)
        : Criterion(), mUseIntSlack(false)
    {
        mPercentageSlack = std::min(0.0, percentage);
    }

    bool SimilarSizeCriterion::evaluate(const Graph& graph, const Solution& solution) const
    {
        int nbClasses = solution.getNbClasses();
        if (nbClasses == 0)
        {
            return false;
        }

        int elementPerClass = static_cast<int>(static_cast<size_t>(graph.getNbVertices()) / nbClasses);

        int nbAssignedVertices = 0;
        for (int i = 0; i < nbClasses; ++i)
        {
            int classCardinal = solution.getClassCardinal(i);
            if (mUseIntSlack)
            {
                if (classCardinal < elementPerClass - mIntSlack || classCardinal > elementPerClass + mIntSlack)
                {
                    return false;
                }
            }
            else
            {
                if (classCardinal < elementPerClass * (1 - mPercentageSlack) || classCardinal > elementPerClass * (1 + mPercentageSlack))
                {
                    return false;
                }
            }

            nbAssignedVertices += classCardinal;
        }

        if (nbAssignedVertices != solution.getNbVertices())
        {
            return false;
        }

        return true;
    }

    std::optional<Solution> SimilarSizeCriterion::generateInitialSolution(const Graph& graph, int nbClasses) const
    {
        Solution solution = Solution(graph.getNbVertices(), nbClasses);

        const int verticesPerClass = graph.getNbVertices() / nbClasses;

        int index = 0;
        for (int i = 0; i < nbClasses; ++i)
        {
            for (int j = 0; j < verticesPerClass; ++j)
            {
                solution.setVertexClass(index, i);
                ++index;
            }
        }

        int classToSet = 0;
        while (index < graph.getNbVertices())
        {
            solution.setVertexClass(index, classToSet);

            classToSet = (classToSet + 1) % nbClasses;
            ++index;
        }

        if (!evaluate(graph, solution))
        {
            YOLO_WARN("SimilarSizeCriterion::generateInitialSolution(const Graph& graph, int nbClasses): Could not generate a valid solution with given parameters. Please consider incrementing the slack.\n");
            return std::nullopt;
        }

        return solution;
    }
} // namespace Yolo
