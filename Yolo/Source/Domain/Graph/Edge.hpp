#pragma once

namespace Yolo
{
    class Edge
    {
    public:
        Edge(int source, int destination, double weight) :
            mSource(source), mDestination(destination), mWeight(weight) {}

        inline int getSource() const { return mSource; }
        inline int getDestination() const { return mDestination; }

        inline double getWeight() const { return mWeight; }

    private:
        int mSource;
        int mDestination;

        double mWeight;
    };
}