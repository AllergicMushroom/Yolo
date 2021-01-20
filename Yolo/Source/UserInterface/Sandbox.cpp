#include "Core/Core.hpp"

#include "Technical/Repositories/GraphFileRepository.hpp"

#include <iostream>

int main()
{
    Yolo::Core::initialise();

    Yolo::GraphFileRepository graphRepository;

    std::optional<Yolo::Graph> g = graphRepository.load("Data/quatreSommets.txt");

    Yolo::Core::shutdown();

    return 0;
}