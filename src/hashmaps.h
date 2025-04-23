#ifndef HASHMAPS_H
#define HASHMAPS_H
#include <string>
#include <vector>
using namespace std;

//forward declaration to fix error
class TVShow;


class HashMaps {
private:
    struct Node {
        string key; //tv show title
        TVShow* value; //actual tv show
        Node* next; //pointer to next node
        //Constructor
        Node(string myKey, TVShow* myValue){
            key = myKey;
            value = myValue;
            next = nullptr;
        }
    };
    //our hash table with the buckets being a linked list
    vector<Node*> hashTable;
    //number of buckets for now, large to have a lower load factor since we have large data set
    int tableSize = 30000;
    //total elements in hashTable
    int elementsAmount = 0;

    //the hash function that turns the key into our table's indexes
    int hashFunction(string key);

    public:
    //set all buckets to empty
    HashMaps();

    //destructor
    ~HashMaps();

    //adding tvshow to hash table
    void insert(string key, TVShow* value);

    //gets TVShow
    TVShow* getShow(string key);


    //clears values in table
    void clear();
};

#endif //HASHMAPS_H
