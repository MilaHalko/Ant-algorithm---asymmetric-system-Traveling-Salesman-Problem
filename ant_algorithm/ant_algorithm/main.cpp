#include <iostream>
#include <iomanip>
#include <vector>
#include <time.h>
using namespace std;

const int ITTERATIONS = 1;

const int SIZE = 5;  // goal 300
const int MIN_DISTANCE = 1; // goal 5
const int MAX_DISTANCE = 10; // goal 150
const float INITIAL_PHEROMONE = 0.1;

const float A = 1;
const float B = 1;
const float P = 0.2;
const float Q = 34;   // LMin
const int S_ANTS = 1; // standard
const int E_ANTS = 1; // elite
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
    
    DistanceAndPheromone(): distance(0), pheromone(INITIAL_PHEROMONE * (rand() % 10 + 0.1)) {}
};

void generateTable(vector<vector<DistanceAndPheromone>> &table);
void generateTable(vector<vector<DistanceAndPheromone>> &table) {
    table.resize(SIZE);
    for (int i = 0; i < SIZE; i++) {
        table[i].resize(SIZE);
        for (int j = 0; j < SIZE; j++) {
            float distance = (i == j) ? 0 : (1 / float(rand() % (MAX_DISTANCE - MIN_DISTANCE + 1) + MIN_DISTANCE));
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
    //cout << "Spread: ";
    //for (auto elem: ants) {cout << elem << "   ";}
    //cout << endl;
    
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

bool notVisited(int v, vector<int> path);
bool notVisited(int v, vector<int> path) {
    for (auto elem: path) {
        if (v == elem) return false;
    }
    return true;
}

float countSInterest(float distance, float pheromone);
float countSInterest(float distance, float pheromone) {
    //OPTIONAL
    //cout << "Interest: " << pheromone << "^" << A << " * " << distance << "^" << B << " = " << pow(pheromone, A) * pow(distance, B) << endl;
    //cout << "Interest: " << pheromone << " * " << distance << " = " << pheromone * distance << endl;
    return pow(pheromone, A) * pow(distance, B);
}

int getSV(vector<DistanceAndPheromone> adjacent, vector<int> path);
int getSV(vector<DistanceAndPheromone> adjacent, vector<int> path) {
    vector<pair<float, int>> scale (0);
    for (int v = 0; v < SIZE; v++) {
        if (adjacent[v].getDistance() != 0  &&  notVisited(v, path)) {
            scale.push_back(make_pair(countSInterest(adjacent[v].getDistance(), adjacent[v].getPheromone()), v));
        }
    }
    float prevSum = 0;
    for (int i = 0; i < scale.size(); i++) {
        scale[i].first = scale[i].first + prevSum;
        prevSum = scale[i].first;
    }
    //OPTIONAL
    //cout << "Scale:\n";
    //for (int i = 0; i < scale.size(); i++) {cout << scale[i].second << ": " << scale[i].first << endl;}
    
    float percent = (rand() % 101);
    percent = (scale[scale.size() - 1].first * percent) / float(100);
    
    int v = -1;
    for (int i = 0; i < scale.size(); i++) {
        if (percent <= scale[i].first) {
            v = scale[i].second;
            break;
        }
    }
    return v;
}

int getEV(vector<DistanceAndPheromone> adjacent, vector<int> path);
int getEV(vector<DistanceAndPheromone> adjacent, vector<int> path) {
    int bestV;
    do { bestV = rand() % SIZE;} while (adjacent[bestV].getDistance() == 0);
    for (int v = 0; v < SIZE; v++) {
        if (adjacent[v].getDistance() != 0  &&  notVisited(v, path)) {
            if (adjacent[bestV].getPheromone() < adjacent[v].getPheromone()) { bestV = v;}
        }
    }
    return bestV;
}

vector<int> getStandardPath(int v, vector<vector<DistanceAndPheromone>> table);
vector<int> getStandardPath(int v,  vector<vector<DistanceAndPheromone>> table) {
    vector<int> path (0);
    path.push_back(v);
    for (int i = 0; i < SIZE - 2; i++) {
        v = getSV(table[v], path);
        path.push_back(v);
    }
    int penultimate = 0;
    for (int i = 0; i < path.size(); i++) {
        if(path[i] == penultimate) {
            penultimate++;
            i = -1;
        }
    }
    path.push_back(penultimate);
    path.push_back(path[0]);
    //OPTIONAL
    cout << "Path: ";
    for (auto v: path) cout << v << " -> ";
    cout << endl;
    
    return path;
}

vector<int> getElitePath(int v, vector<vector<DistanceAndPheromone>> table);
vector<int> getElitePath(int v, vector<vector<DistanceAndPheromone>> table) {
    vector<int> path (0);
    path.push_back(v);
    for (int i = 0; i < SIZE - 2; i++) {
        v = getEV(table[v], path);
        path.push_back(v);
    }
    int penultimate = 0;
    for (int i = 0; i < path.size(); i++) {
        if(path[i] == penultimate) {
            penultimate++;
            i = -1;
        }
    }
    path.push_back(penultimate);
    path.push_back(path[0]);
    //OPTIONAL
    cout << "Path: ";
    for (auto v: path) cout << v << " -> ";
    cout << endl;
    
    return path;
}

vector<int> getWildPath(int v, vector<vector<DistanceAndPheromone>> table);
vector<int> getWildPath(int v, vector<vector<DistanceAndPheromone>> table) {
    vector<int> vertices (0);
    for (int i = 0; i < SIZE; i++) {if (i != v) {vertices.push_back(i);}}
    
    vector<int> path (0);
    path.push_back(v);
    for (int i = 0; i < SIZE - 1; i++) {
        int randomV = rand() % vertices.size();
        path.push_back(vertices[randomV]);
        vertices.erase(vertices.begin() + randomV);
    }
    path.push_back(v);
    //OPTIONAL
    cout << "Path: ";
    for (auto v: path) cout << v << " -> ";
    cout << endl;
    
    return path;
}

void colonySearchProcess(vector<vector<DistanceAndPheromone>> table) {
    int startV = STATIC_START? (rand() % SIZE) : -1;
    for (int i = 0; i < ALL_ANTS; i++) {
        if (!STATIC_START) startV = rand() % SIZE;
        bool standard = false, elite = false, wild = false;
        vector<int> antsProportion = spreadAnts();
        setAntType(antsProportion, standard, elite, wild);
        
        if(standard) {
            getStandardPath(startV, table);
            //OPTIONAL
            cout << "standard\n\n";
            s_ants--;
        }
        if (elite) {
            getElitePath(startV, table);
            //OPTIONAL
            cout << "elite\n\n";
            e_ants--;
        }
        if (wild) {
            getWildPath(startV, table);
            //OPTIONAL
            cout << "wild\n\n";
            w_ants--;
        }
        //OPTIONAL
        //cout << "All ants: " << s_ants << " + " << e_ants << " + " << w_ants << " = " << s_ants + e_ants + w_ants << endl << endl;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    vector<vector<DistanceAndPheromone>> table (0, vector<DistanceAndPheromone>(0)); //table of distances and pheromones
    generateTable(table);
    
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
