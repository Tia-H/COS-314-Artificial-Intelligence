#include "simulatedAnnealing.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>
using namespace std;

bool simulatedAnnealing::readData(std::string fileName)
{
    std::vector<int> data;
    std::vector<int> x;
    std::vector<int> y;

    ifstream dataFile(fileName);
    string input="";
    getline(dataFile,input);
    if (input=="")
    {
        return false;
    }

    bool start=false;
    std::cout << " started simulated annealing search..." <<endl;
    while (input!="EOF")
    {
        if (start==true)
        {
            string dataPoint;
            int num;
            int start=0;
            int val=0;
            for (int i=0;i<input.length(); i++)
            {
                if (input[i]==' ' || input[i]=='\n')
                {
                    dataPoint = input.substr(start,i-start);
                    stringstream ss;
                    ss << dataPoint;
                    ss >> num;
                    data.push_back(num);
                    start = i+1;
                    val++;
                    if (val==2)
                    {
                        x.push_back(num);
                    }
                }
            }
            if (val==2)
                {
                    dataPoint = input.substr(start,input.length()-start);
                    stringstream ss;
                    ss << dataPoint;
                    ss >> num;
                    data.push_back(num);
                    y.push_back(num);
                    val++;
                }
            
        }
        if (input.find("DIMENSION") != string::npos)
        {
            int num;
            string cityNum = input.substr(11,input.length()-11);
                    stringstream ss;
                    ss << cityNum;
                    ss >> num;
            cities = num;
        }
        if (input.find("NODE_COORD_SECTION") != string::npos)
        {
            start=true;
        }
        getline(dataFile,input);
    }

    for (int i=0; i<x.size();i++)
    {
        city newCity;
        newCity.num = i+1;
        newCity.x = x.at(i);
        newCity.y = y.at(i);
        cityData.push_back(newCity);
    }
    dataFile.close();
    return true;
}

void simulatedAnnealing::search(std::string fileName,int seed)
{
    auto startTime = std::chrono::steady_clock::now();

    cityData.clear();
    randomisedCities.clear();
    if (readData(fileName)==true) //->get num cities + data values
    {
        //cout << "file read" <<endl;
    }else{
        std::cout << "No data found";
        return;
    }

    //start with a random solution s ->randomisedCities
    randomiseCities(seed);
    vector<city> s = randomisedCities;
    vector<city> currState = s;
    vector<city> best = s;
    int noImprovements=0;
    //initialTemp determined by looking at typical difference in "loss" between neighbouring solutions
    //higher T0 means more likely to accept worse solutions -> explore search space more
    float sum=0;
    for (int i=0 ;i<10; i++)
    {
        vector<city> next = getNeighbour(currState,seed+i);
        float diff = lossFunction(next)-lossFunction(currState);//98;//calcInitialTemp();
        sum+= abs(diff);
    }
    int initialTemp = sum/10;//98;//calcInitialTemp();
    float temp = initialTemp *1.8;
    int maxIterations = 500*cities;

    if (cities>=20)
    {
        //maxIterations*=2;
    }

    int i=0;
    //for (int i=0; i<maxIterations ; i++)//repeat til no improvement in BEST for N iterations ->max iterations?
    while (noImprovements<maxIterations )
    {
        //perturbation ->make small change to current solution
        vector<city> next = getNeighbour(currState,seed+i);
        //vector<city> *next = &neighbour; //randomly chosen adjacent state to current
        int distanceDiff = lossFunction(next)-lossFunction(currState);
        if (distanceDiff<0)
        {
            currState = next;
            //noImprovements=0;
        }else{
            srand(seed+i);
            float num = rand()/RAND_MAX;
            if (num <= exp(-distanceDiff/temp))
            {
                currState = next; //with probability min(1,exp(-distanceDiff/temp))
                //noImprovements=0;
            }else{
                //noImprovements++;
            }
        }
        if (lossFunction(currState)<lossFunction(best))
        {
            best = currState;
            noImprovements=0;
        }else{
            noImprovements++;
        }
        temp = temp*0.95;//initialTemp/log(t+1);
        i++;
    }

    auto endTime = std::chrono::steady_clock::now();
    auto diff = endTime - startTime;
    std::cout << "Cities: " << cities<< endl;
    std::cout << "Seed: " << seed <<endl;
    std::cout << "Cost: " << lossFunction(best) <<endl;
    std::cout << "Best solution: " ;
    for (int i=0;i<best.size();i++)
    {
        std::cout << best.at(i).num <<",";
    }
    std::cout <<endl;
    std::cout << "Runtime: " << std::chrono::duration<double, std::milli>(diff).count() << " ms" << endl;
    std::cout << endl;
}

float simulatedAnnealing::lossFunction(vector<city> cities)
{
    float totalDistance=0;
    for (int i=0; i<cities.size()-1;i++)
    {
        totalDistance += calcDistance(cities.at(i),cities.at(i+1));
    }
    totalDistance += calcDistance(cities.at(this->cities-1),cities.at(0));
    return totalDistance;
}

void simulatedAnnealing::randomiseCities(int seed)
{
    srand(seed);
    for (int i=0; i<cityData.size(); i++)
    {
        randomisedCities.push_back(cityData.at(i));
    }
    random_shuffle(randomisedCities.begin()+1,randomisedCities.end());
}

float simulatedAnnealing::calcDistance(city city1,city city2)
{
    float d=0;
        city startCity = (city1);
        city endCity = (city2);
        
        d = sqrt( pow(startCity.x-endCity.x,2) + pow(startCity.y-endCity.y,2));

    return d;
}

vector<city> simulatedAnnealing::getNeighbour(vector<city> cities,int i)
{
    srand(i);
    int index1 = rand() % this->cities;
    int index2= rand() % this->cities;
    while (index1==0)
    {
        index1= rand() % this->cities;
    }
    while (index2==index1 ||index2==0)
    {
        index2= rand() % this->cities;
    }
    vector<city> neighbour;
    for (int j=0; j<cities.size(); j++)
    {
        neighbour.push_back(cities.at(j));
    }
    swap(neighbour[index1],neighbour[index2]);

    return neighbour;
}