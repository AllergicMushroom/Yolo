#include "Edge.hpp"

#include "Core/Logger/Logger.hpp"

namespace Yolo
{
    Edge::Edge(int source, int destination, double weight)
        : mSource(source), mDestination(destination), mWeight(weight)
    {
        if (source < 0)
        {
            YOLO_DEBUG("Edge::Edge(int source, int destination, double weight): source is negative when it shouldn't.\n");
        }

        if (destination < 0)
        {
            YOLO_DEBUG("Edge::Edge(int source, int destination, double weight): destination is negative when it shouldn't.\n");
        }
    }

    bool Edge::operator==(const Edge& edge2) const
    {
        return this->getSource() == edge2.getSource() && this->getDestination() == edge2.getDestination() && this->getWeight() == edge2.getWeight();
    }

    bool Edge::operator!=(const Edge& edge2) const
    {
        return !((*this) == edge2);
    }

} // namespace Yolo
