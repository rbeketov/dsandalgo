#include "arc_graph.h"

ArcGraph::ArcGraph(size_t vertices_count)
    : graph(), vertices_count_(vertices_count) {}

ArcGraph::ArcGraph(const IGraph& other)
    : graph(), vertices_count_(other.VerticesCount()) {
    for (size_t i = 0; i < other.VerticesCount(); ++i) {
        for (auto it : other.GetNextVertices(i)) {
            AddEdge(i, it);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    assert(from < vertices_count_ && to < vertices_count_);
    graph.push_back({from, to});
}

size_t ArcGraph::VerticesCount() const {
    return vertices_count_;    
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(vertex < vertices_count_);
    std::vector<int> result;
    for (const auto& it : graph) {
        if (it.first == vertex) {
            result.push_back(it.second);
        }
    }
    return result;
}
std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(vertex < vertices_count_);
    std::vector<int> result;
    for (const auto& it : graph) {
        if (it.second == vertex) {
            result.push_back(it.first);
        }
    }
    return result;
}