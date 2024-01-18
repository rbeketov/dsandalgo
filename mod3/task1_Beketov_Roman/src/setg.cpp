#include "set_graph.h"


SetGraph::SetGraph(size_t vertices_count)
    : graph(vertices_count), graph_prev(vertices_count) {}

SetGraph::SetGraph(const IGraph& other)
    : graph(other.VerticesCount()), graph_prev(other.VerticesCount()) {
    for (size_t i = 0; i < VerticesCount(); ++i) {
        auto next = other.GetNextVertices(i);
        graph[i] = std::unordered_set<int>(next.begin(), next.end());
        auto prev = other.GetPrevVertices(i);
        graph_prev[i]= std::unordered_set<int>(prev.begin(), prev.end());
    }
}

void SetGraph::AddEdge(int from, int to) {
    assert(from < VerticesCount() && to < VerticesCount());
    graph[from].insert(to);
    graph_prev[to].insert(from);
}

size_t SetGraph::VerticesCount() const {
    return graph.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex < VerticesCount());
    return std::vector(graph[vertex].begin(), graph[vertex].end());
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex < VerticesCount());
    return std::vector(graph_prev[vertex].begin(), graph_prev[vertex].end());
}