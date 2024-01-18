#include <iostream>
#include <vector>
#include <stack>
#include <cassert>

class ListGraph {
 public:
    ListGraph(size_t vertices_count);
 
    void AddEdge(int from, int to);

	size_t VerticesCount() const;

    std::vector<int> GetNextVertices(int vertex) const;
 private:
    std::vector< std::vector<int> > graph;
};

ListGraph::ListGraph(size_t vertices_count)
    : graph(vertices_count) {}


void ListGraph::AddEdge(int from, int to) {
    assert(from < VerticesCount() && to < VerticesCount());
    graph[from].push_back(to);
    graph[to].push_back(from);

}

size_t ListGraph::VerticesCount() const {
    return graph.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex < VerticesCount());
    return graph[vertex];  
}


size_t countConnectivityComponent(const ListGraph& graph) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::stack<int> stack;
    size_t result = 0;
    for (size_t vertex = 0; vertex < graph.VerticesCount(); ++vertex) {
        if (!visited[vertex]) {
            ++result;
            stack.push(vertex);
        }

        while (!stack.empty()) {
            int curr_vertex = stack.top();
            stack.pop();

            if (!visited[curr_vertex]) {
                visited[curr_vertex] = true;

                std::vector<int> children = graph.GetNextVertices(curr_vertex);
                for (auto child : children) {
                    if (!visited[child]) {
                        stack.push(child);
                    }
                }
            }
        }
    }
    return result;

}


void solution() {
    size_t cnt_vertex = 0;
    std::cin >> cnt_vertex;

    ListGraph graph(cnt_vertex);

    size_t cnt_edges = 0;
    std::cin >> cnt_edges;
    for (size_t i = 0; i < cnt_edges; ++i) {
        int from = 0;
        int to = 0;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    std::cout << countConnectivityComponent(graph) << std::endl;
}

int main() {
    solution();
    return 0;
}