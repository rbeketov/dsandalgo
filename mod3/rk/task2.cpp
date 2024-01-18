#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
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

bool checkExistEdges(const ListGraph& graph, int from, int to) {
    auto children_last_vertex = graph.GetNextVertices(from);
    for (auto it : children_last_vertex) {
        if (it == to) {
            return true;
        }
    }
    return false;
}


bool checkHamiltonianPath(const ListGraph& graph, const std::vector<int>& path) {
 
    if (!checkExistEdges(graph, path[path.size()-1], path[0])) {
        return false;
    }


    std::unordered_set<int> visited;
    for (int i = 0; i < path.size(); ++i) {
        if (visited.find(path[i]) != visited.end()) {
            return false;
        }
        visited.insert(path[i]);

        if (i < path.size() - 1 && !checkExistEdges(graph, path[i], path[i+1])) {
           return false; 
        }
    }
    return true;
}



void solution() {
    size_t cnt_vertex = 0;
    std::cin >> cnt_vertex;

    ListGraph graph(cnt_vertex);

    size_t cnt_edges = 0;
    std::cin >> cnt_edges;

    size_t cnt_edges_in_path = 0;
    std::cin >> cnt_edges_in_path;

    if (cnt_edges != cnt_edges_in_path || cnt_vertex != cnt_edges_in_path) {
        std::cout << 0 <<std::endl;
        return;
    }

    for (size_t i = 0; i < cnt_edges; ++i) {
        int from = 0;
        int to = 0;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    std::vector<int> path(cnt_edges_in_path);
    for (size_t i = 0; i < cnt_edges_in_path; ++i) {
        std::cin >> path[i];
    }

    if (checkHamiltonianPath(graph, path)) {
        std::cout << 1 <<std::endl;
    } else {
        std::cout << 0 << std::endl;
    }
    
}

int main() {
    solution();
    return 0;
}