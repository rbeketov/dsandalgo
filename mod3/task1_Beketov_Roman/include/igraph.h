#pragma once

#include <vector>
#include <cassert>

class IGraph {
 public:
    virtual ~IGraph() {}
	
    bool operator==(const IGraph& other) const;

    virtual void AddEdge(int from, int to) = 0;

	virtual size_t VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};
