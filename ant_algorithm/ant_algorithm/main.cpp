#include <iostream>
#include <iomanip>
#include <vector>
#include <time.h>
using namespace std;

const int SIZE = 10;  // goal 300
const int MIN_DISTANCE = 2; // goal 5
const int MAX_DISTANCE = 7; // goal 150
const float INITIAL_PHEROMONE = 0.2;

const float A = 1;
const float B = 1;
const float P = 0.2;
const float Q = 34;   // LMin
const int S_ANTS = 8; // standard
const int E_ANTS = 2; // elite
const int W_ANTS = 2; // wild
const bool STATIC_START = true;


class DistanceAndPheromone { //distance == 1 / real distance
    float distance;
    float pheromone;
    
public:
    float getDistance() {return distance;}
    float getPheromone() {return pheromone;}
    
    void setDistance(float distance) {this->distance = distance;}
    void setPheromone(float pheromone) {this->pheromone = pheromone;}
    
    DistanceAndPheromone(): distance(0), pheromone(INITIAL_PHEROMONE) {}
};

void fullTable(vector<vector<DistanceAndPheromone>> &table);
void fullTable(vector<vector<DistanceAndPheromone>> &table) {
    table.resize(SIZE);
    for (int i = 0; i < SIZE; i++) {
        table[i].resize(SIZE);
        for (int j = 0; j < SIZE; j++) {
            float distance = (i == j) ? 0 : (1 / float(rand() % (MAX_DISTANCE - MIN_DISTANCE) + MIN_DISTANCE));
            table[i][j].setDistance(distance);
        }
    }
}

void coutTable(vector<vector<DistanceAndPheromone>> table);
void coutTable(vector<vector<DistanceAndPheromone>> table) {
    for (int i = 0; i < SIZE*8 + 4; i++) {cout << "-";} cout << endl;
    for (auto row: table) {
        cout.precision(3);
        for (int i = 0; i < SIZE*8 + 4; i++) {cout << "-";} cout << endl;
        cout << "D: |";
        for (int i = 0; i < SIZE; i++) {cout << setw(7) << row[i].getDistance() << "|";} cout << endl;
        cout << "P: |";
        for (int i = 0; i < SIZE; i++) {cout << setw(7) << row[i].getPheromone() << "|";} cout << endl;
    }
    for (int i = 0; i < SIZE*8 + 4; i++) {cout << "-";} cout << endl;
    for (int i = 0; i < SIZE*8 + 4; i++) {cout << "-";} cout << endl;
    cout << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    vector<vector<DistanceAndPheromone>> table (0, vector<DistanceAndPheromone>(0)); //table of distances and pheromones
    //generateDistances(table);
    fullTable(table);
    coutTable(table);
    
    return 0;
}
