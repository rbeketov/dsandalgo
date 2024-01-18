#include "arc_graph.h"
#include "set_graph.h"
#include "list_graph.h"
#include "matrix_graph.h"
#include "traversal.h"

#include <iostream>

//
//          5
//          |
//      0 - 1 - 4 
//      |
//      2 - 3
//

void tests() {
    MatrixGraph matrix_graph(6);
    matrix_graph.AddEdge(0, 1);
    matrix_graph.AddEdge(1, 4);
    matrix_graph.AddEdge(1, 5);
    matrix_graph.AddEdge(0, 2);
    matrix_graph.AddEdge(2, 3);
    
    std::vector<int> expect_reslut_bfs{0, 1, 2, 4, 5, 3};
    std::vector<int> expect_reslut_dfs{0, 2, 3, 1, 5, 4}; // начинает идти в гулбину со "старшей вершины"
    // так как использую стек => результат

    std::vector<int> reslut_bfs;
    std::vector<int> reslut_dfs;

    dfs(matrix_graph, [&reslut_dfs](int vertex) { reslut_dfs.push_back(vertex); });
    bfs(matrix_graph, [&reslut_bfs](int vertex) { reslut_bfs.push_back(vertex); });
    
    assert(reslut_bfs == expect_reslut_bfs && reslut_dfs == expect_reslut_dfs);
    std::cout << "TESTS DFS AND BFS PASSED SUCCESSFULLY" << std::endl;

    SetGraph set_graph(matrix_graph);
    ListGraph list_graph(set_graph);
    ArcGraph arc_graph(list_graph);

    assert(matrix_graph == set_graph);
    assert(set_graph == list_graph);    
    assert(list_graph == arc_graph);

    std::cout << "TESTS EQUALITY PASSED SUCCESSFULLY " << std::endl;
}

int main() {
    tests();
    
    std::cout << "ALL TESTS PASSED SUCCESSFULLY" << std::endl;
    return 0;
}