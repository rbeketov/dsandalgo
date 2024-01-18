#pragma once
#include "dsu.h"

void kruskal(std::vector<Edge*>& edges, size_t vertices_count, const std::function<void(const Edge*)>& callback);

std::vector<Vertex> getRandomNPoint(int n);
std::vector<Edge*> makeEdges(const std::vector<Vertex>& points);
double minLenSpanningTree(const std::vector<Vertex>& points);
void processCombination(const std::vector<int>& marks, const std::vector<Vertex>& points);
double claculatePath(const std::vector<int>& marks, const std::vector<Vertex>& points);