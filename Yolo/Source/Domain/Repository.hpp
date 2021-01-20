#pragma once

#include <optional>
#include <string>

namespace Yolo
{
    template<typename T>
    class Repository
    {
    public:
        virtual void save(std::string ID, T object) = 0;
        virtual std::optional<T> load(std::string ID) = 0;
    };
}