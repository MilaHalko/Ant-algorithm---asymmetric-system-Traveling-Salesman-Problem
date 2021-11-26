#include "Ant algorithm.hpp"
extern int s_ants, e_ants, w_ants;
int main() {
    srand(static_cast<unsigned int>(time(0)));
    vector<vector<DistanceAndPheromone>> table (SIZE, vector<DistanceAndPheromone>(SIZE)); //table of distances and pheromones
    fullTable(table);
    for (int i = 0; i < ITTERATIONS; i++) {
        colonySearchProcess(table);
        cout << "iteration " << i+1 << ": ";
        vector<int> bestPath = getBestPath(table);
        cout << getPathDistance(bestPath, table) << endl;
        //cout << "Best path: ";
        //for (int i = 0; i < SIZE; i++) cout << bestPath[i] << " -> "; cout << bestPath[0] << endl;
    }
    return 0;
}

/*int main() { // For Creating File
    srand(static_cast<unsigned int>(time(0)));
    vector<vector<DistanceAndPheromone>> table (SIZE, vector<DistanceAndPheromone>(SIZE)); //table of distances and pheromones
    //generateTable(table);
    //createFile(table);
    fullTable(table);
    coutTable(table);
    return 0;
} */
