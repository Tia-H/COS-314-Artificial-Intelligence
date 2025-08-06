#include "simulatedAnnealing.cpp"
#include "tabuSearch.cpp"

#include <unistd.h>

int genSeed()
{
    /*auto t = chrono::system_clock::now();
    auto convert = t.time_since_epoch();
    auto seed = chrono::duration_cast<chrono::milliseconds>(convert).count();*/
    time_t seed;
    time(&seed);
    return seed;
}

int main()
{
    //implement Simulated Annealing local search
    simulatedAnnealing search1;
    tabuSearch search2;
    int seed = genSeed();
    cout << "Seeded with randomly generated values: " <<endl;
    search1.search("8.txt",seed);
    search1.search("12.txt",seed); 
    search1.search("15.txt",seed); 
    search1.search("20.txt",seed); 
    search1.search("25.txt",seed);
    search2.search("8.txt",seed,8*100);
    search2.search("12.txt",seed,12*100);
    search2.search("15.txt",seed,15*100);
    search2.search("20.txt",seed,20*100);
    search2.search("25.txt",seed,25*100);

    cout << "Seeded with best solution values: " <<endl;
    //SA
    search1.search("8.txt",1742666829);
    search1.search("12.txt",1742667964);//1742667710
    search1.search("15.txt",1742669716); 
    search1.search("20.txt",1742669343); 
    search1.search("25.txt",1742669142);
    //TABU
    search2.search("8.txt",1742666829,8*100);
    search2.search("12.txt",1742668281,12*100);
    search2.search("15.txt",1742668417,15*100);
    search2.search("20.txt",1742668976,20*100); 
    search2.search("25.txt",1742667127,25*100);
}