#pragma once

#include "traversal.h"
#include "igraph.h"

#include <unordered_set>

class SetGraph : public IGraph {
public:
	SetGraph(size_t vertices_count);
    SetGraph(const IGraph& other);

    void AddEdge(int from, int to) override;

	size_t VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
 private:
    std::vector< std::unordered_set<int> > graph;
    std::vector< std::unordered_set<int> > graph_prev;
};
