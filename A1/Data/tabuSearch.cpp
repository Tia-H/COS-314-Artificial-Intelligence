#include "tabuSearch.h"

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

bool tabuSearch::readData(std::string fileName)
{
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
    cout << " started tabu search..." <<endl;
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

void tabuSearch::search(std::string fileName,int seed,int max)
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
    vector<city> s = randomisedCities; //initial candidate sol
    int maxLength = cities*2; //max length of tabu list
    vector<city> tabuList; //initialise tabu list
    //int noImprovements=0;
    int maxIterations = max;

    for (int i=0; i<maxIterations ; i++)//repeat til no improvement in BEST for N iterations ->max iterations?
    {
        //perturbation ->make small change to current solution
        vector<city> next = getNeighbour(s,seed+i);
        //if next is not an elemet of tabuList
        if (isElement(tabuList,next)==false)
        {
            if (tabuList.size() > maxLength)
            {
                for (int j=0; j<cities ;j++)
                {
                    tabuList.erase(tabuList.begin());
                }
            }
            //add next to tabuList
            for (int k=0;k<next.size();k++)
            {
                tabuList.push_back(next.at(k));
            }
        }
        //if next<s
        if (calcCost(next) < calcCost(s))
        {
            s.clear();
            s = next;
            //noImprovements=0;
        }
    }


    auto endTime = std::chrono::steady_clock::now();
    auto diff = endTime - startTime;
    cout << "Cities: " << cities<< endl;
    cout << "Seed: " << seed <<endl;
    cout << "Cost: " << calcCost(s) <<endl;
    cout << "Best solution: " ;
    for (int i=0;i<s.size();i++)
    {
        cout << s.at(i).num <<",";
    }
    cout <<endl;
    cout << "Runtime: " << std::chrono::duration<double, std::milli>(diff).count() << " ms" << endl;
    cout << endl;
}

void tabuSearch::randomiseCities(int seed)
{
    srand(seed);
    for (int i=0; i<cityData.size(); i++)
    {
        randomisedCities.push_back(cityData.at(i));
    }
    random_shuffle(randomisedCities.begin()+1,randomisedCities.end());
}

bool valid(vector<city> tabuList,vector<city> neighbour,int index)
{
    
    for (int i=0;i<neighbour.size();i++)
    {
        if (index+i < tabuList.size())
        {
            if (tabuList.at(index+i).num!=neighbour.at(i).num)
            {
                return false;
            }
        }else{
            return false;
        }
    }
    return true;
}

bool tabuSearch::isElement(vector<city> tabuList,vector<city> neighbour)
{
    bool startFound=false;
    int index=0;
    for (int i=0;i<tabuList.size();i++)
    {
        if ((tabuList.at(i).num==neighbour.at(0).num))
        {
            startFound=true;
            index=i;
            if (valid(tabuList,neighbour,index)==true)
            {
                return true;
            }

        }
    }
    return false;
}

vector<city> tabuSearch::getNeighbour(vector<city> cities,int i)
{
    srand(i);
    int index1 = rand() %this->cities;
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

float tabuSearch::calcDistance(city city1,city city2)
{
    float d=0;
        city startCity = (city1);
        city endCity = (city2);
        
        d = sqrt( pow(startCity.x-endCity.x,2) + pow(startCity.y-endCity.y,2));

    return d;
}

float tabuSearch::calcCost(vector<city> cities)
{
    float totalDistance=0;
    for (int i=0; i<cities.size()-1;i++)
    {
        totalDistance += calcDistance(cities.at(i),cities.at(i+1));
    }
    totalDistance += calcDistance(cities.at(this->cities-1),cities.at(0));
    return totalDistance;
}