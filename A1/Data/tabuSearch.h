#ifndef TABU_SEARCH_H
#define TABU_SEARCH_H

#include <iostream>
#include <vector>
using namespace std;

class tabuSearch
{
    private:
        std::vector<city> cityData;
        std::vector<city> randomisedCities;
        int cities;
    public:
        bool readData(std::string fileName);
        void search(std::string fileName,int seed,int max);
        bool isElement(vector<city> tabuList,vector<city> neighbour);
        void randomiseCities(int seed);
        vector<city> getNeighbour(vector<city> cities,int i);
        float calcCost(vector<city> cities);
        float calcDistance(city city1,city city2);
};

#endif