// Требуется отыскать самый короткий маршрут между городами.
// Из города может выходить дорога, которая возвращается в этот же город.

// Требуемое время работы O((N + M)log N), где N – количество городов, M – известных дорог между ними.
// N ≤ 10000, M ≤ 250000.
// Длина каждой дороги ≤ 10000.

#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <limits>
#include <utility>
#include <set>

class ListGraph {
 public:
    ListGraph(size_t vertices_count);
 
    void AddEdge(int from, int to, int time);

	size_t VerticesCount() const;

    std::vector< std::pair<int, int> > GetNextVertices(int vertex) const;
 private:
    std::vector< std::vector<std::pair<int,int> > > graph;
};

ListGraph::ListGraph(size_t vertices_count)
    : graph(vertices_count) {}


void ListGraph::AddEdge(int from, int to, int time) {
    assert(from < VerticesCount() && to < VerticesCount());
    graph[from].push_back({to, time});
    graph[to].push_back({from, time});

}

size_t ListGraph::VerticesCount() const {
    return graph.size();
}

std::vector< std::pair<int, int> > ListGraph::GetNextVertices(int vertex) const {
    assert(vertex < VerticesCount());
    return graph[vertex];  
}


int Dijkstra(const ListGraph& graph, int from, int to) {
    std::vector<int> dist(graph.VerticesCount(), std::numeric_limits<int>::max());
    std::set< std::pair<int, int> > heap;
    dist[from] = 0;
    heap.insert({0, from});
    while (!heap.empty()) {
        auto curr_vertex = heap.begin()->second;
        heap.erase(heap.begin());
        for (auto child : graph.GetNextVertices(curr_vertex)) {
            if (dist[child.first] == std::numeric_limits<int>::max() 
                || dist[child.first] > dist[curr_vertex] + child.second) {
                dist[child.first] = dist[curr_vertex] + child.second;
                heap.insert({dist[child.first], child.first});
            }
        }
    }
    return dist[to];
}


void test() {
    ListGraph list_graph(6);

    list_graph.AddEdge(0, 3, 1);
    list_graph.AddEdge(0, 4, 2);
    list_graph.AddEdge(1, 2, 7);
    list_graph.AddEdge(1, 3, 2);
    list_graph.AddEdge(1, 4, 3);
    list_graph.AddEdge(1, 5, 3);
    list_graph.AddEdge(2, 5, 3);
    list_graph.AddEdge(3, 4, 4);
    list_graph.AddEdge(3, 5, 6);
    assert(Dijkstra(list_graph, 0, 2) == 9);
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
        int time = 0;
        std::cin >> from >> to >> time;
        list_graph.AddEdge(from, to, time);
    }

    int from = 0;
    int to = 0;
    std::cin >> from >> to;
    std::cout << Dijkstra(list_graph, from, to) << std::endl;
}

int main() {
    //test();
    solution();
    
    return 0;
}