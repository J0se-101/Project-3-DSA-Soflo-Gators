//
// Created by mikayla cohen on 4/21/25.
//

#ifndef HASHMAPS_H
#define HASHMAPS_H
#include <string>
#include "TVShow.h"
using namespace std;


class HashMaps {
private:
    struct Node {
        string key; //tv show title
        TVShow value; //actual tv show
        Node* next; //pointer to next node
    };

    //static const needed so compiler knows that it is constant
    //otherwise it will give an error
    static const int table_size = 550000;
    //want over 500,000 for less chance of collisions
    Node* table[table_size];

    int hashFunction(string key);

    public:
    //set all buckets to empty
    HashMaps();

    //destructor
    ~HashMaps();

    //adding tvshow to hash table
    void insert(string key, TVShow value);

    //gets TVShow
    TVShow* getShow(string key);

    bool checkifTitleExists(string key);

    //clears values in table
    void clear();
};



#endif //HASHMAPS_H
