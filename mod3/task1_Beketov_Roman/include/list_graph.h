#pragma once

#include "traversal.h"
#include "igraph.h"

class ListGraph : public IGraph {
 public:
    ListGraph(size_t vertices_count);
    ListGraph(const IGraph& other);

    void AddEdge(int from, int to) override;

	size_t VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
 private:
    std::vector< std::vector<int> > graph;
    std::vector< std::vector<int> > graph_prev;
};
