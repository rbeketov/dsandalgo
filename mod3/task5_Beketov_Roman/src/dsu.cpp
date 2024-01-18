#include "dsu.h"


DSU::DSU(size_t vertices_count)
    : rank(vertices_count, 1), parent(vertices_count) {
    for (int i = 0; i < vertices_count; ++i) {
        parent[i] = i;
    }
}

int DSU::find_set(int v) {
    if (v == parent[v]) {
        return v;
    }
    return parent[v] = find_set(parent[v]);
}

void DSU::union_set(int v, int u) {

    if (rank[v] < rank[u]) {
        std::swap(v, u);
    }

    parent[u] = v;
    rank[v] += rank[u];
}


bool Edge::operator<(const Edge& other) const {
    return weight < other.weight;
}
