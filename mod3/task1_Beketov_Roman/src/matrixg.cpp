#include "matrix_graph.h"

MatrixGraph::MatrixGraph(size_t vertices_count)
    : graph(vertices_count, std::vector<bool>(vertices_count, false)) {}

MatrixGraph::MatrixGraph(const IGraph& other)
    : graph(other.VerticesCount(), std::vector<bool>(other.VerticesCount(), false)) {
    for (size_t i = 0; i < VerticesCount(); ++i) {
        auto next = other.GetNextVertices(i);
        for (auto it : next) {
            graph[i][it] = true;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    assert(from < VerticesCount() && to < VerticesCount());
    graph[from][to] = true;
}

size_t MatrixGraph::VerticesCount() const {
    return graph.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex < VerticesCount());
    std::vector<int> result;
    for (int i = 0; i < VerticesCount(); ++i) {
        if (graph[vertex][i]) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(vertex < VerticesCount());
    std::vector<int> result;
    for (int i = 0; i < VerticesCount(); ++i) {
        if (graph[i][vertex]) {
            result.push_back(i);
        }
    }
    return result;
}