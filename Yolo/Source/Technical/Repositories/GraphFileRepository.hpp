#pragma once

#include "Domain/Repository.hpp"

#include "Domain/Graph/Graph.hpp"

namespace Yolo
{
    class GraphFileRepository : public Repository<Graph>
    {
    public:
        virtual void save(std::string ID, Graph graph) override;
        virtual std::optional<Graph> load(std::string ID) override;

    private:
        std::vector<std::string> tokenizeString(std::string string) const;
    };
}