#pragma once
#include "Libs.hpp"

//MAIN ALGORITHM
void colonySearchProcess(vector<vector<DistanceAndPheromone>> &table);

//ANT CHOICE
vector<int> spreadAnts(int s_ants, int e_ants, int w_ants);
void setAntType(vector<int> antsProportion, bool &S, bool &E, bool &W);

//ANT'S PATH
vector<int> getStandardPath(int v, vector<vector<DistanceAndPheromone>> table);
vector<int> getElitePath(int v, vector<vector<DistanceAndPheromone>> table);
vector<int> getWildPath(int v, vector<vector<DistanceAndPheromone>> table);
int getSV(vector<DistanceAndPheromone> adjacent, vector<int> path);
float countSInterest(float distance, float pheromone);
int getEV(vector<DistanceAndPheromone> adjacent, vector<int> path);

//PHEROMONES' UPGRADING
void evaporate(vector<vector<DistanceAndPheromone>> &table);
void upgradePheromones(vector<int> path, float distance, vector<vector<float>> &newPheromones, bool elite);
void addPheromones(vector<vector<DistanceAndPheromone>> &table, vector<vector<float>> newPheromones);

//ADDITIONAL FUNCTION
bool notVisited(int v, vector<int> path);
float getPathDistance(vector<int> path, vector<vector<DistanceAndPheromone>> table);
vector<int> getBestPath(vector<vector<DistanceAndPheromone>> table);
