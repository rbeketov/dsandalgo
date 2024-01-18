#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <stack>
#include <set>

class DSU {
 public:
    explicit DSU(size_t vertices_count)
        : rank(vertices_count, 1), parent(vertices_count) {
        for (int i = 0; i < vertices_count; ++i) {
            parent[i] = i;
        }
    }

    int find_set(int v) {
        if (v == parent[v]) {
            return v;
        }
        return parent[v] = find_set(parent[v]);
    }

    void union_set(int v, int u) {

        if (rank[v] < rank[u]) {
            std::swap(v, u);
        }

        parent[u] = v;
        rank[v] += rank[u];
    }

 private:
    std::vector<int> rank;
    std::vector<int> parent;
};


struct Edge {
    Edge(int a_from, int a_to, int a_weight)
        : from(a_from), to(a_to), weight(a_weight) {}

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }

    int from;
    int to;
    int weight;
};

void kruskal(std::vector<Edge*>& edges, size_t vertices_count, const std::function<void(const Edge*)>& callback) {
    DSU dsu(vertices_count);

    for (const auto edge : edges) {
    
        int v = dsu.find_set(edge->from);
        int u = dsu.find_set(edge->to);

        if (v != u) {
            dsu.union_set(v, u);
            callback(edge);
        }
    }
}


void solution() {
    int cnt_edges = 0;
    int vertices_count = 0;
    
    std::cin >> vertices_count >> cnt_edges;
    std::vector<Edge*> edges(cnt_edges);

    int from = 0;
    int to = 0;
    int weight = 0;
    for (int i = 0; i < cnt_edges; ++i) {
        std::cin >> from >> to >> weight;
        edges[i] = new Edge{from - 1, to - 1, weight};
    }
 
    int sum = 0;
    std::sort(edges.begin(), edges.end(), [](const Edge* l, const Edge* r) {
        return l->weight < r->weight;
    });
    kruskal(edges, vertices_count, [&sum](const Edge* edge) { 
         sum += edge->weight;
    });
    std::cout << sum << std::endl;
}


int main() {
    solution();
    return 0;
}