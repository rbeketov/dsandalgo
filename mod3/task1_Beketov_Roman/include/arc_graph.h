#pragma once

#include "traversal.h"
#include "igraph.h"

#include <utility>

class ArcGraph : public IGraph {
 public:
    ArcGraph(size_t vertices_count);
    ArcGraph(const IGraph& other);

    void AddEdge(int from, int to) override;

	size_t VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
 private:
    std::vector< std::pair<int, int> > graph;
    size_t vertices_count_;
};
