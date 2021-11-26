#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <time.h>
#include <fstream>
using namespace std;

//CONSTS
const int ITTERATIONS = 1;
    //MATRIX
const int SIZE = 300;  // goal 300
const int MIN_DISTANCE = 5; // goal 5
const int MAX_DISTANCE = 150; // goal 150
const float INITIAL_PHEROMONE = 0.1;
    //ANT PARAMETRS
const float A = 1;
const float B = 1;
const float P = 0.7;  // how many remains
const float Q = 4;    // LMin
const int S_ANTS = 7; // standard
const int E_ANTS = 2; // elite
const int W_ANTS = 1; // wild
const int ALL_ANTS = S_ANTS + E_ANTS + W_ANTS;
const bool STATIC_START = true;


//CLASS keeps distance and pheromone for 1 vertice
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

//TABLE MANIPULATIONS
void generateTable(vector<vector<DistanceAndPheromone>> &table);
void createFile(vector<vector<DistanceAndPheromone>> table);
void fullTable(vector<vector<DistanceAndPheromone>> &table);
void coutTable(vector<vector<DistanceAndPheromone>> table);
