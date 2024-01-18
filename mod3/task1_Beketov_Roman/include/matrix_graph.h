#pragma once

#include "igraph.h"

class MatrixGraph : public IGraph {
 public:
    MatrixGraph(size_t vertices_count);
    MatrixGraph(const IGraph& other);

    void AddEdge(int from, int to) override;

	size_t VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
 private:
    std::vector< std::vector<bool> > graph;
};
