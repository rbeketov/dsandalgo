#include "utils.h"


void experiment() {
    for (int i = 2; i < 11; ++i) {
        std::vector<double> p_array;
        for (int j = 0; j < 10; ++j) {
            std::vector<Vertex> points = getRandomNPoint(i);
            double min_len_enumeration = minLenSpanningTree(points);
            std::vector<Edge*> edges =  makeEdges(points);
            double min_len_kruskal = 0;
            kruskal(edges, i, [&min_len_kruskal] (const Edge* edge) {
                min_len_kruskal += edge->weight;
            });
            p_array.push_back((double) min_len_kruskal / (double) min_len_enumeration);
        }
        double mean = 0;
        for (auto it : p_array) {
            mean += (double)it;
        }
        mean = (double) mean / (double) p_array.size();

        double sigma = 0;
        for (auto it : p_array) {
            sigma += (double) std::pow(it - mean, 2);
        }
        sigma = std::sqrt((double) sigma / (double) p_array.size());

        std::cout << "====================================" << std::endl;
        std::cout << "Количесвто точек: " << i << std::endl;
        std::cout << "Среднее приближение: " << (double) mean << std::endl;
        std::cout << "Среднее квадратичное отклонение: " << (double) sigma << std::endl;
    }
}


int main() {
    experiment();
    return 0;
}