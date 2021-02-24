#include "SimilarSizeCriterion.hpp"

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
} // namespace Yolo
