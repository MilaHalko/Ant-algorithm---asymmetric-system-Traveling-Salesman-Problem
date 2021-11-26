#include "Libs.hpp"
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

void createFile(vector<vector<DistanceAndPheromone>> table) {
    ofstream fout ("MATRIX.txt");
    for (auto row: table) {
        for (int i = 0; i < SIZE; i++) {fout << "|" << row[i].getDistance();} fout << endl;
    }
    fout.close();
}

void fullTable(vector<vector<DistanceAndPheromone>> &table) {
    ifstream fin ("MATRIX.txt");
    for (int i = 0; i < SIZE; i++) {
        string line;
        getline(fin, line);
        int index = 0;
        for (int j = 0; j < SIZE; j++) {
            line = line.substr(index + 1);
            index = line.find("|");
            table[i][j].setDistance(stof(line.substr(0, index)));
        }
    }
    fin.close();
}

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
