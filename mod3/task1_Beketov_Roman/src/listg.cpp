#include "list_graph.h"
#include <iostream>

ListGraph::ListGraph(size_t vertices_count)
    : graph(vertices_count), graph_prev(vertices_count) {}

ListGraph::ListGraph(const IGraph& other)
    : graph(other.VerticesCount()), graph_prev(other.VerticesCount()) {
    for (size_t i = 0; i < VerticesCount(); ++i) {
        auto next = other.GetNextVertices(i);
        graph[i] = std::vector<int>(next.begin(), next.end());
        auto prev = other.GetPrevVertices(i);
        graph_prev[i] = std::vector<int>(prev.begin(), prev.end());
    }
}

void ListGraph::AddEdge(int from, int to) {
    assert(from < VerticesCount() && to < VerticesCount());
    graph[from].push_back(to);
    graph_prev[to].push_back(from);

}

size_t ListGraph::VerticesCount() const {
    return graph.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex < VerticesCount());
    return graph[vertex];  
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(vertex < VerticesCount());
    return graph_prev[vertex];  
}