#include "traversal.h"

#include <queue>
#include <stack>

void bfs(const IGraph& graph, const std::function<void(int)> &callback) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> que;
    for (size_t vertex = 0; vertex < graph.VerticesCount(); ++vertex) {
        if (!visited[vertex]) {
            que.push(vertex);
            visited[vertex] = true;
            while (!que.empty()) {
                int curr_vertex = que.front();
                que.pop();

                callback(curr_vertex);

                std::vector<int> children = graph.GetNextVertices(curr_vertex);
                for (auto child : children) {
                    if (!visited[child]) {
                        visited[child] = true;
                        que.push(child);
                    }
                }
            }
        }
    }
}

void dfs(const IGraph& graph, const std::function<void(int)> &callback) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::stack<int> stack;
    for (size_t vertex = 0; vertex < graph.VerticesCount(); ++vertex) {
        if (!visited[vertex]) {
            stack.push(vertex);
        }

        while (!stack.empty()) {
            int curr_vertex = stack.top();
            stack.pop();

            if (!visited[curr_vertex]) {
                callback(curr_vertex);
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
}
