// main.cpp
#include "ClueClusterer.h"
#include <iostream>

int main()
{
    double dc = 0.05;    // Απόσταση cutoff για υπολογισμό πυκνότητας
    int numClusters = 3; // Πλήθος αναμενόμενων clusters
    std::string filepath = "data/particles.csv";

    ClueClusterer clue(dc);

    if (!clue.loadCSV(filepath))
    {
        std::cerr << "Error loading CSV file." << std::endl;
        return 1;
    }

    clue.computeLocalDensity();
    clue.computeDelta();
    clue.findClusterCenters(numClusters);
    clue.assignClusters();
    clue.printResults();

    return 0;
}