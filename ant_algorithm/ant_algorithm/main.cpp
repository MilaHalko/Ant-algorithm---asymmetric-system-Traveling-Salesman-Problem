#include <iostream>
#include <iomanip>
#include <vector>
#include <time.h>
using namespace std;

const int ITTERATIONS = 1;

const int SIZE = 10;  // goal 300
const int MIN_DISTANCE = 2; // goal 5
const int MAX_DISTANCE = 7; // goal 150
const float INITIAL_PHEROMONE = 0.2;

const float A = 1;
const float B = 1;
const float P = 0.2;
const float Q = 34;   // LMin
const int S_ANTS = 7; // standard
const int E_ANTS = 2; // elite
const int W_ANTS = 1; // wild
const int ALL_ANTS = S_ANTS + E_ANTS + W_ANTS;

int s_ants = S_ANTS; // standard
int e_ants = E_ANTS; // elite
int w_ants = W_ANTS; // wild
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

vector<int> spreadAnts() {                          // |    0    |  1  |  2  |  .. |       ...       | ...     |   all |
                                                    // | ant = 0 |      s_ants     |      e_ants     |      w_ants     |
    vector<int> ants (0);
    int point = s_ants;
    ants.push_back(point);
    point += e_ants;
    ants.push_back(point);
    point += w_ants;
    ants.push_back(point);
    
    //OPTIONAL
    cout << "Spread: ";
    for (auto elem: ants) {cout << elem << "   ";}
    cout << endl;
    
    return ants;
}

void setAntType(vector<int> antsProportion, bool &S, bool &E, bool &W) {
    int antType = 0;
    float percent = rand() % (s_ants + e_ants + w_ants) + 1;
    
    for (int i = 0; i < antsProportion.size(); i++) {
        if (antsProportion[i] >= percent) {
            antType = i+1;
            break;
        }
    }
    
    //OPTIONAL
    cout << "AntType: " << antType << endl;
    
    switch (antType) {
        case 1:
            S = true;
            break;
        case 2:
            E = true;
            break;
        case 3:
            W = true;
            break;
        default:
            cout << "ERROR, WRONG ANT TYPE \n";
            break;
    }
}

void colonySearchProcess(vector<vector<DistanceAndPheromone>> table) {
    int startV = STATIC_START? (rand() % SIZE) : -1;
    
    //OPTIONAL
    cout << "Vertice: " << startV << "\n";
    
    for (int i = 0; i < ALL_ANTS; i++) {
        if (!STATIC_START) {startV = rand() % SIZE;}
        
        bool standard = false, elite = false, wild = false;
        vector<int> antsProportion = spreadAnts();
        setAntType(antsProportion, standard, elite, wild);
        
        if(standard) {
            //OPTIONAL
            cout << "standard\n";
            s_ants--;
        }
        if (elite) {
            //OPTIONAL
            cout << "elite\n";
            e_ants--;
        }
        if (wild) {
            //OPTIONAL
            cout << "wild\n";
            w_ants--;
        }
        
        //OPTIONAL
        cout << "All ants: " << s_ants << " + " << e_ants << " + " << w_ants << " = " << s_ants + e_ants + w_ants << endl << endl;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    vector<vector<DistanceAndPheromone>> table (0, vector<DistanceAndPheromone>(0)); //table of distances and pheromones
    //generateDistances(table);
    fullTable(table);
    
    //OPTIONAL
    coutTable(table);
    
    for (int i = 0; i < ITTERATIONS; i++) {
        s_ants = S_ANTS;
        e_ants = E_ANTS;
        w_ants = W_ANTS;
        colonySearchProcess(table);
    }
    
    return 0;
}
