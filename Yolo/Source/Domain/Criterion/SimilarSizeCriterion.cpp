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

    bool SimilarSizeCriterion::evaluate(const Graph& graph, const Solution& solution, bool isPartial) const
    {
        const int nbClasses = solution.getNbClasses();
        if (nbClasses == 0)
        {
            return false;
        }

        const int elementsPerClass = static_cast<int>(static_cast<size_t>(graph.getNbVertices()) / nbClasses);

        int cardinalsLowerBound = 0;
        int cardinalsUpperBound = 0;
        if (mUseIntSlack)
        {
            cardinalsLowerBound = elementsPerClass - mIntSlack;
            cardinalsUpperBound = elementsPerClass + mIntSlack;
        }
        else
        {
            cardinalsLowerBound = static_cast<int>(elementsPerClass * (1 - mPercentageSlack));
            cardinalsUpperBound = static_cast<int>(elementsPerClass * (1 + mPercentageSlack));
        }

        if (isPartial)
        {
            int nbAssignedVertices = 0;

            for (int i = 0; i < nbClasses; ++i)
            {
                const int classCardinal = solution.getClassCardinal(i);
                if (classCardinal > cardinalsUpperBound)
                {
                    return false;
                }

                nbAssignedVertices += classCardinal;
            }

            const int nbUnassignedVertices = solution.getNbVertices() - nbAssignedVertices;

            int cardinalsGapToLowerBoundSum = 0;

            for (int i = 0; i < nbClasses; ++i)
            {
                const int cardinalGapToLowerBound = std::max(0, cardinalsLowerBound - solution.getClassCardinal(i));
                if (cardinalGapToLowerBound > nbUnassignedVertices)
                {
                    return false;
                }

                cardinalsGapToLowerBoundSum += cardinalGapToLowerBound;
            }

            if (cardinalsGapToLowerBoundSum > nbUnassignedVertices)
            {
                return false;
            }
        }
        else
        {
            int nbAssignedVertices = 0;
            for (int i = 0; i < nbClasses; ++i)
            {
                const int classCardinal = solution.getClassCardinal(i);
                if (classCardinal < cardinalsLowerBound || classCardinal > cardinalsUpperBound)
                {
                    return false;
                }

                nbAssignedVertices += classCardinal;
            }

            if (nbAssignedVertices != solution.getNbVertices())
            {
                return false;
            }
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
