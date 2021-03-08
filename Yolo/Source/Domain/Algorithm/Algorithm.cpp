#include "Algorithm.hpp"

#include "Domain/Criterion/SimilarSizeCriterion.hpp"

static const Yolo::SimilarSizeCriterion sDefaultCriterion = Yolo::SimilarSizeCriterion(1);

namespace Yolo
{
    Algorithm::Algorithm(const Graph& graph, int nbClasses)
        : mGraph(graph), mNbClasses(nbClasses)
    {
        mCriterion = &sDefaultCriterion;
    }

} // namespace Yolo
