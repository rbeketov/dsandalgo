// Дан невзвешенный неориентированный граф.
// В графе может быть несколько кратчайших путей между какими-то вершинами.
// Найдите количество различных кратчайших путей между заданными вершинами.
// Требуемая сложность O(V+E).

#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <limits>


#include <sstream>

class ListGraph {
 public:
    ListGraph(size_t vertices_count);
 
    void AddEdge(int from, int to);

	size_t VerticesCount() const;

    std::vector<int> GetNextVertices(int vertex) const;
    std::vector<int> GetPrevVertices(int vertex) const;
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


int cntMinPaths(const ListGraph& graph, int from, int to) {
    std::vector<int> path_count(graph.VerticesCount(), 0);
    std::vector<int> dist(graph.VerticesCount(), std::numeric_limits<int>::max());
    std::queue<int> que;
    path_count[from] = 1;
    dist[from] = 0;
    que.push(from);
    while (!que.empty()) {
        int curr_vertex = que.front();
        que.pop();
        for (auto child : graph.GetNextVertices(curr_vertex)) {
            if (dist[child] > dist[curr_vertex] + 1) {
                dist[child] = dist[curr_vertex] + 1;
                path_count[child] = path_count[curr_vertex];
                que.push(child);
            } else if (dist[child] == dist[curr_vertex] + 1) {
                path_count[child] += path_count[curr_vertex];
            }
        }
    }
    return path_count[to];
}


void test() {
    ListGraph list_graph(4);

    list_graph.AddEdge(0, 1);
    list_graph.AddEdge(0, 2);
    list_graph.AddEdge(1, 2);
    list_graph.AddEdge(1, 3);
    list_graph.AddEdge(2, 3);
    assert(cntMinPaths(list_graph, 0, 3) == 2);
    std::cout << "TEST PASSED SUCCESSFULLY" << std::endl;
}


void solution() {
    size_t cnt_vertex = 0;
    std::cin >> cnt_vertex;
    ListGraph list_graph(cnt_vertex);
    size_t cnt_edge = 0;
    std::cin >> cnt_edge;
    for (size_t i = 0; i < cnt_edge; ++i) {
        int from = 0;
        int to = 0;
        std::cin >> from >> to;
        list_graph.AddEdge(from, to);
    }

    int from = 0;
    int to = 0;
    std::cin >> from >> to;
    std::cout << cntMinPaths(list_graph, from, to) << std::endl;
}

int main() {
    //test();
    solution();
    
    return 0;
}