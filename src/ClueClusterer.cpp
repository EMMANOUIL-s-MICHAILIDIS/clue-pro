#include "ClueClusterer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <algorithm>

ClueClusterer::ClueClusterer(double dc) : dc(dc) {}

bool ClueClusterer::loadCSV(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    int id = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Point p;
        p.id = id++;

        std::getline(ss, token, ','); p.x = std::stod(token);
        std::getline(ss, token, ','); p.y = std::stod(token);
        std::getline(ss, token, ','); p.z = std::stod(token);

        points.push_back(p);
    }
    return true;
}

double ClueClusterer::distance(const Point& a, const Point& b) {
    return std::sqrt(
        (a.x - b.x) * (a.x - b.x) +
        (a.y - b.y) * (a.y - b.y) +
        (a.z - b.z) * (a.z - b.z)
    );
}

void ClueClusterer::computeLocalDensity() {
    for (auto& pi : points) {
        for (const auto& pj : points) {
            if (pi.id != pj.id && distance(pi, pj) < dc) {
                pi.rho += 1.0;
            }
        }
    }
}

void ClueClusterer::computeDelta() {
    for (auto& pi : points) {
        pi.delta = std::numeric_limits<double>::max();
        for (const auto& pj : points) {
            if (pj.rho > pi.rho) {
                double d = distance(pi, pj);
                if (d < pi.delta) {
                    pi.delta = d;
                    pi.nearest_higher = pj.id;
                }
            }
        }
    }

    // Handle the max-rho point (no one has higher density)
    auto max_rho_it = std::max_element(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.rho < b.rho;
    });
    max_rho_it->delta = 0.0;
    for (const auto& p : points) {
        double d = distance(*max_rho_it, p);
        if (d > max_rho_it->delta) {
            max_rho_it->delta = d;
        }
    }
}

void ClueClusterer::findClusterCenters(int numCenters) {
    std::vector<Point> sorted = points;
    std::sort(sorted.begin(), sorted.end(), [](const Point& a, const Point& b) {
        return (a.rho * a.delta) > (b.rho * b.delta);
    });

    for (int i = 0; i < numCenters; ++i) {
        clusterCenters.push_back(sorted[i].id);
        points[sorted[i].id].cluster_id = i;
    }
}

void ClueClusterer::assignClusters() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto& pi : points) {
            if (pi.cluster_id == -1 && pi.nearest_higher != -1) {
                int nh_id = pi.nearest_higher;
                if (points[nh_id].cluster_id != -1) {
                    pi.cluster_id = points[nh_id].cluster_id;
                    changed = true;
                }
            }
        }
    }
}

void ClueClusterer::printResults() {
    for (const auto& p : points) {
        std::cout << "Point " << p.id << ": Cluster " << p.cluster_id
                  << " | Rho: " << p.rho << " | Delta: " << p.delta << "\n";
    }
}