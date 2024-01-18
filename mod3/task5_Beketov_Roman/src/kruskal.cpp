#include "utils.h"


void kruskal(std::vector<Edge*>& edges, size_t vertices_count, const std::function<void(const Edge*)>& callback) {
    DSU dsu(vertices_count);

    std::sort(edges.begin(), edges.end(), [](const Edge* l, const Edge* r) {
        return l->weight < r->weight;
    });

    for (const auto edge : edges) {
    
        int v = dsu.find_set(edge->from);
        int u = dsu.find_set(edge->to);

        if (v != u) {
            dsu.union_set(v, u);
            callback(edge);
        }
    }
}
