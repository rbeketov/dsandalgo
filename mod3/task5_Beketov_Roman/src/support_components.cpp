#include "utils.h"

#include <cstdlib>
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>

std::vector<Vertex> getRandomNPoint(int n) {
    std::vector<Vertex> result;
    std::random_device r;
    std::default_random_engine generator(r());
    std::uniform_real_distribution<double> dist(-1,1);

    dist(generator);
    for (int i = 0; i < n; ++i) {
        double s = 6;
        double u = 0;
        double v = 0;
        while (s <= 0 || s > 1) {
            u = dist(generator);
            v = dist(generator);
            s = u*u + v*v;
        }
        
        double x = u * std::sqrt((-2 * std::log(s)) / s);
        double y = v * std::sqrt((-2 * std::log(s)) / s);
        result.push_back({x, y});
    }
    return result;
}





double claculatePath(const std::vector<int>& marks, const std::vector<Vertex>& points) {
    double result = 0;
    for (int i = 0; i < points.size() - 1; ++i) {
        double distance = std::sqrt(std::pow(points[marks[i]].x - points[marks[i+1]].x, 2) + std::pow(points[marks[i]].y - points[marks[i+1]].y, 2));
        result += distance;
    }
    return result;
}



std::vector<Edge*> makeEdges(const std::vector<Vertex>& points) {
    std::vector<Edge*> result;
    for (int i = 0; i < points.size() - 1; ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            double distance = std::sqrt(std::pow(points[i].x - points[j].x, 2) + std::pow(points[i].y - points[j].y, 2));
            result.push_back(new Edge(i, j, distance));
        }
    }
    return result;
}


void processCombination(const std::vector<int>& marks, const std::vector<Vertex>& points) {
    std::cout << "============" << std::endl;
    for (auto it : marks) {
        std::cout << points[it].x << " " << points[it].y << " ";
    }
    std::cout << std::endl;
    std::cout << claculatePath(marks, points) << std::endl;
    std::cout << "============" << std::endl;
}

double minLenSpanningTree(const std::vector<Vertex>& points) {
    
    double min_path = INT_MAX;
    
    std::vector<int> marks(points.size());
    for (int i = 0; i < points.size(); ++i) {
        marks[i] = i;
    }
    do {
        double curr_path = claculatePath(marks, points);
        if (curr_path < min_path) {
            min_path = curr_path;
        }
    } while (std::next_permutation(marks.begin(), marks.end()));
    return min_path;
}