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
    // graph[to].push_back({from, time});

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



void solution() {
    int cost_a = 0;
    int cost_b = 0;
    size_t cnt_vertex = 0;
    int start = 0;
    int stop = 0;
    std::cin >> cost_a >> cost_b >> cnt_vertex >> start >> stop;

    ListGraph graph(cnt_vertex);

    for (size_t i = 0; i < cnt_vertex; ++i) {
        graph.AddEdge(i, ((i+1) % cnt_vertex), cost_a);
        graph.AddEdge(i, ((i*i+1) % cnt_vertex), cost_b);
    }

    std::cout << Dijkstra(graph, start, stop) << std::endl;
}

int main() {

    solution();
    
    return 0;
}