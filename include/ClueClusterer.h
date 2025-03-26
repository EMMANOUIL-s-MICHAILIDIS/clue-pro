
#pragma once
#include <vector>
#include <string>
#include <cmath>

struct Point {
    int id;
    double x, y, z;
    double rho = 0.0;
    double delta = 0.0;
    int nearest_higher = -1;
    int cluster_id = -1;
};

class ClueClusterer {
public:
    ClueClusterer(double dc);
    bool loadCSV(const std::string& filepath);
    void computeLocalDensity();
    void computeDelta();
    void findClusterCenters(int numCenters);
    void assignClusters();
    void printResults();

private:
    double distance(const Point& a, const Point& b);
    double dc;
    std::vector<Point> points;
    std::vector<int> clusterCenters;
};
