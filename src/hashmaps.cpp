//
// Created by mikayla cohen on 4/21/25.
//

#include "hashmaps.h"

//hash function
int HashMaps::hashFunction(string key) {
    //idk if this is right, double check me
    int hash = 0;
    int prime = 37;
    for (int i = 0; i < key.length(); i++) {
        hash = hash * prime + key[i];
    }
    return hash % table_size;
}

HashMaps::HashMaps() {
}

HashMaps::~HashMaps() {
    clear();
}

void HashMaps::insert(string key, TVShow value) {
}

TVShow * HashMaps::getShow(string key) {
}

bool HashMaps::checkifTitleExists(string key) {
}

void HashMaps::clear() {
    for (int i = 0; i < table_size; i++) {
        // table[i].clear();
    }
}
