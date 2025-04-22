
#include "hashmaps.h"
#include "TVShow.h"
//hash function, take the key and returns an index from our hash table
int HashMaps::hashFunction(string key) {
    //large prime number
    unsigned long hash = 5381;

       for(char myChar : key){
           hash = ((hash<<5)+hash) + myChar;
       }

    return static_cast<int>(hash % tableSize);
}
//constructor, with empty buckets
HashMaps::HashMaps() {
    tableSize = 30000;
    hashTable.resize(tableSize, nullptr);
    elementsAmount = 0;
}
//destructor
HashMaps::~HashMaps() {
    clear();
}
//inserting tv shows into hash table with our titles being the string key
void HashMaps::insert(string key, TVShow* value) {
    int index = hashFunction(key);
    Node* currentNode = hashTable[index];
    int steps=0;
    //check if key already exists here
    while(currentNode){
        steps++;
        if(currentNode->key==key){
            currentNode->value = value;
            return;
        }
        currentNode = currentNode->next;
    }
    //If the node isn't found, then we insert a new node in front
    Node* newNode = new Node(key,value);
    newNode->next=hashTable[index];
    hashTable[index]=newNode;
    elementsAmount++;
}
//gets ptr to a tv show by the key (show title)
TVShow * HashMaps::getShow(string key) {
    int index = hashFunction(key);
    Node* currentNode = hashTable[index];

    while(currentNode){
        if(currentNode->key==key){
            return currentNode->value;
        }
        currentNode= currentNode->next;
    }
    return nullptr; //if it wasn't found
}

bool HashMaps::checkifTitleExists(string key) {
    return getShow(key)!=nullptr;
}

void HashMaps::clear() {
    for (int i = 0; i < tableSize; i++) {
       Node* currentNode = hashTable[i];
       while(currentNode){
           Node* deleteNode = currentNode;
           currentNode=currentNode->next;
           delete deleteNode->value;
           delete deleteNode;
       }
       hashTable[i]=nullptr;
    }
    elementsAmount =0;
}

int HashMaps::size(){
    return elementsAmount;
}

int HashMaps::bucketCount(){
    return tableSize;
}

float HashMaps::loadFactor() {
    return static_cast<float>(elementsAmount)/tableSize;
}
