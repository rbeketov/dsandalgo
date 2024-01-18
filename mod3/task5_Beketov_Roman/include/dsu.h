#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <stack>
#include <set>


class DSU {
 public:
    explicit DSU(size_t vertices_count);

    int find_set(int v);
    void union_set(int v, int u);

 private:
    std::vector<int> rank;
    std::vector<int> parent;
};


struct Edge {
    Edge(int a_from, int a_to, double a_weight)
        : from(a_from), to(a_to), weight(a_weight) {}

    bool operator<(const Edge& other) const;

    int from;
    int to;
    double weight;
};


struct Vertex {
    Vertex(double a_x, double a_y)
        : x(a_x), y(a_y) {}
    double x;
    double y;
};