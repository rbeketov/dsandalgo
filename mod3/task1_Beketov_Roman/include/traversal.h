#pragma once

#include <functional>

#include "igraph.h"

void bfs(const IGraph& graph, const std::function<void(int)> &callback);
void dfs(const IGraph& graph, const std::function<void(int)> &callback);
