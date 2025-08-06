#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <iostream>
#include <vector>
using namespace std;

struct city
{
    int num;
    int x;
    int y;
};

class simulatedAnnealing
{
    private:
        std::vector<city> cityData;
        std::vector<city> randomisedCities;
        int cities;
    public:
        bool readData(std::string fileName);
        void search(std::string fileName,int seed);
        float lossFunction(vector<city> cities);
        void randomiseCities(int seed);
        float calcDistance(city city1,city city2);
        vector<city> getNeighbour(vector<city> cities,int i);
};

#endif