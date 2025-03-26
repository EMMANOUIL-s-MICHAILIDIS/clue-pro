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
    // Βρες το μέγιστο rho για να εντοπίσεις το πιο πυκνό σημείο
    auto max_rho_it = std::max_element(points.begin(), points.end(),
        [](const Point& a, const Point& b) {
            return a.rho < b.rho;
        });

    // Για κάθε σημείο...
    for (auto& p : points) {
        double minDist = std::numeric_limits<double>::max();
        int nearest = -1;

        // Αντικείμενο με μεγαλύτερη πυκνότητα
        for (const auto& other : points) {
            if (other.rho > p.rho) {
                double d = distance(p, other);
                if (d < minDist) {
                    minDist = d;
                    nearest = other.id;
                }
            }
        }

        if (nearest != -1) {
            p.delta = minDist;
            p.nearest_higher = nearest;
        } else {
            // Αν δεν υπάρχει πιο πυκνό σημείο (δηλ. το πιο πυκνό από όλα)
            p.delta = 0.0;
            for (const auto& other : points) {
                if (p.id != other.id) {
                    double d = distance(p, other);
                    if (d > p.delta) {
                        p.delta = d;
                    }
                }
            }
            p.nearest_higher = -1;
        }
    }
}


void ClueClusterer::findClusterCenters(int numCenters) {
    // Υπολογισμός gamma = rho * delta
    std::vector<std::pair<int, double>> gammaScores;
    for (const auto& p : points) {
        gammaScores.emplace_back(p.id, p.rho * p.delta);
    }

    // Ταξινόμηση κατά φθίνουσα σειρά gamma
    std::sort(gammaScores.begin(), gammaScores.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });

    // Επιλογή των top-N κέντρων
    clusterCenters.clear();
    for (int i = 0; i < numCenters && i < (int)gammaScores.size(); ++i) {
        int center_id = gammaScores[i].first;
        clusterCenters.push_back(center_id);
        points[center_id].cluster_id = i;  // Δώσε του cluster ID
    }
}


void ClueClusterer::assignClusters() {
    // Ταξινόμηση κατά φθίνουσα πυκνότητα
    std::vector<Point*> sortedPoints;
    for (auto& p : points) {
        sortedPoints.push_back(&p);
    }

    std::sort(sortedPoints.begin(), sortedPoints.end(),
              [](const Point* a, const Point* b) {
                  return a->rho > b->rho;
              });

    for (auto& p : sortedPoints) {
        if (p->cluster_id == -1) {
            p->cluster_id = points[p->nearest_higher].cluster_id;
        }
    }
}


void ClueClusterer::printResults() {
    for (const auto& p : points) {
        std::cout << "Point " << p.id << ": Cluster " << p.cluster_id
                  << " | Rho: " << p.rho << " | Delta: " << p.delta << "\n";
    }
}