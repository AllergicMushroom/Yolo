#pragma once

namespace Yolo
{
    class Edge
    {
    public:
        Edge(int source, int destination, double weight) :
            mSource(source), mDestination(destination), mWeight(weight) {}

    private:
        int mSource;
        int mDestination;

        double mWeight;
    };
}