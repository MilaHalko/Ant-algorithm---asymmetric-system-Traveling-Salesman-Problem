#include "Ant algorithm.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//MAIN ALGORITHM~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void colonySearchProcess(vector<vector<DistanceAndPheromone>> &table) {
    vector<int> path (0);
    float lMin = 0;
    int v = 0;
    int nextV;
    float minD;
    for (int i = 0; i < SIZE; i++) {
        minD = 0;
        for (int j = 0; j < SIZE; j++) {
            if (v != j  &&  notVisited(j, path)  &&  minD < table[v][j].getDistance()) {
                nextV = j;
                minD = table[v][j].getDistance();
            }
        }
        int d = 1 / table[v][nextV].getDistance();
        lMin += d;
        path.push_back(nextV);
        v = nextV;
    }
    cout << "G = " << lMin << endl;
    
    int s_ants = S_ANTS;
    int e_ants = E_ANTS;
    int w_ants = W_ANTS;
    vector<vector<float>> newPheromones (SIZE, vector<float> (SIZE));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            newPheromones[i][j] = table[i][j].getPheromone();
        }
    }
    int startV = STATIC_START? (rand() % SIZE) : -1;
    for (int i = 0; i < ALL_ANTS; i++) {
        if (!STATIC_START) startV = rand() % SIZE;
        bool standard = false, elite = false, wild = false;
        
        vector<int> antsProportion = spreadAnts(s_ants, e_ants, w_ants);
        setAntType(antsProportion, standard, elite, wild);
        
        vector<int> path(SIZE + 1);
        if(standard) {
            path = getStandardPath(startV, table);
            s_ants--;
        }
        if (elite) {
            path = getElitePath(startV, table);
            e_ants--;
        }
        if (wild) {
            path = getWildPath(startV, table);
            w_ants--;
        }
        float distance = getPathDistance(path, table);
        upgradePheromones(path, distance, newPheromones, elite);
    }
    evaporate(table);
    addPheromones(table, newPheromones);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ANT CHOICE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
vector<int> spreadAnts(int s_ants, int e_ants, int w_ants) {                          // |    0    |  1  |  2  |  .. |       ...       | ...     |   all |
                                                    // | ant = 0 |      s_ants     |      e_ants     |      w_ants     |
    vector<int> ants (0);
    int point = s_ants;
    ants.push_back(point);
    point += e_ants;
    ants.push_back(point);
    point += w_ants;
    ants.push_back(point);
    return ants;
}

void setAntType(vector<int> antsProportion, bool &S, bool &E, bool &W) {
    int antType = 0;
    float percent = rand() % (antsProportion[antsProportion.size() - 1]) + 1;
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ANT'S PATH~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
    return path;
}

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
    return path;
}

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
    return path;
}

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
    float percent = (rand() % 101);
    percent = (scale[scale.size() - 1].first * percent) / float(100);
    int v = -1;
    for (int i = 0; i < scale.size(); i++) {
        if (percent <= scale[i].first) {
            v = scale[i].second;
            break;
        }
    }
    if (v == -1) v = scale[scale.size() - 1].second;
    return v;
}

float countSInterest(float distance, float pheromone) {
    return pow(pheromone, A) * pow(distance, B);
}

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


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//PHEROMONES' UPGRADING~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void evaporate(vector<vector<DistanceAndPheromone>> &table) {
    for (auto vertices: table) {
        for (auto v: vertices) {
            v.setPheromone(v.getPheromone() * P);
        }
    }
}

void upgradePheromones(vector<int> path, float distance, vector<vector<float>> &newPheromones, bool elite) {
    float newPh = Q / distance;
    newPh *= elite? 2 : 1;
    for (int i = 0; i < path.size() - 1; i++) {
        newPheromones[path[i]][path[i+1]] += newPh;
    }
}

void addPheromones(vector<vector<DistanceAndPheromone>> &table, vector<vector<float>> newPheromones) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            table[i][j].setPheromone(newPheromones[i][j]);
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ADDITIONAL FUNCTION~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool notVisited(int v, vector<int> path) {
    for (auto elem: path) {if (v == elem) return false;}
    return true;
}

float getPathDistance(vector<int> path, vector<vector<DistanceAndPheromone>> table) {
    float distance = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        distance += 1 / table[path[i]][path[i+1]].getDistance();
    }
    return distance;
}

vector<int> getBestPath(vector<vector<DistanceAndPheromone>> table) {
    vector<int> path (2);
    float theBiggestPh = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i != j  &&  theBiggestPh < table[i][j].getPheromone()) {
                theBiggestPh = table[i][j].getPheromone();
                path[0] = i;
                path[1] = j;
            }
        }
    }
    for (int i = 1; i < SIZE - 1; i++) {
        int bestV = -1;
        float bestPh = 0;
        for (int v = 0; v < SIZE; v++) {
            if (notVisited(v, path)) {
                float ph = table[path[i]][v].getPheromone();
                if (ph > bestPh) {
                    bestPh = ph;
                    bestV = v;
                }
            }
        }
        path.push_back(bestV);
    }
    path.push_back(path[0]);
    return path;
}
