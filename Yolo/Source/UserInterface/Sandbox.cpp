#include "Core/Core.hpp"

#include "Technical/Repositories/GraphFileRepository.hpp"

#include <iostream>

int main()
{
    Yolo::Core::initialize();

    Yolo::GraphFileRepository graphRepository;

    std::optional<Yolo::Graph> g = graphRepository.load("Instances/quatreSommets.txt");

    Yolo::Core::shutdown();

    return 0;
}