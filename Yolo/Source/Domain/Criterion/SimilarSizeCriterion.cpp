#include "SimilarSizeCriterion.hpp"

namespace Yolo
{
    SimilarSizeCriterion::SimilarSizeCriterion(int maxSlack)
        : Criterion()
    {
        mIntSlack = std::max(0, maxSlack);
        mUseIntSlack = true;
    }

    SimilarSizeCriterion::SimilarSizeCriterion(double percentage)
        : Criterion()
    {
        mPercentageSlack = std::min(0.0, percentage);
        mUseIntSlack = false;
    }

    bool SimilarSizeCriterion::evaluate(Graph graph, Solution solution) const
    {
        int nbClasses = solution.getNbClasses();

        int elementPerClass = static_cast<int>(static_cast<size_t>(graph.getNbVertices()) / nbClasses);

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
        }

        return true;
    }
} // namespace Yolo
