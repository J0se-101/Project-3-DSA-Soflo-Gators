//
// Created by mikayla cohen on 4/21/25.
//

#include "graph.h"

int Graph::findVertex(string title) {
    for (int i = 0; i < showCount; i++) {
        //looping through the shows
        if (vertices[i].title == title) {
            /*if that show matches title
            return the index that its at */
            return i;
        }
    }
    return -1; //if not found
}

Graph::~Graph() {
    clear();
}

Graph::Graph(int maxShows) {
    this -> maxShows = maxShows;
    showCount = 0;
    vertices = new Vertex[maxShows]; //array of vertices
}

void Graph::addShow(string title) {
    //want to ensure theres enough space
    if (showCount < maxShows) {
        //checking if it has been added (if -1, it hasnt)
        if (findVertex(title) == -1) {
            //add title at the showCount index
            vertices[showCount].title = title;
            vertices[showCount].connections = nullptr;
            showCount++;
        }
    }
}

void Graph::addConnection(string showOne, string showTwo) {
    //get indexes of the shows
    int indexOne = findVertex(showOne);
    int indexTwo = findVertex(showTwo);

    //want to make sure that the shows exist
    if (indexOne != -1 && indexTwo != -1) {
        //creating an edge from showOne to showTwo
        Edge* connectionOne = new Edge;
        connectionOne -> connectedShow = showTwo;
        //show two stored as a show that show one is connected to
        connectionOne -> next = vertices[indexOne].connections;
        //this connection is now connected to the other vertices at that index
        vertices[indexOne].connections = connectionOne;
        //this new connection will be the start of showOnes connections

        //creating an edge from showTwo -> showOne
        Edge* connectionTwo = new Edge;
        connectionTwo -> connectedShow = showOne;
        //show one stored as a show that show two is connected to
        connectionTwo -> next = vertices[indexTwo].connections;
        //this connection is now connected to the other vertices at that index
        vertices[indexTwo].connections = connectionTwo;
        //this new connection will be the start of showTwos connections
    }
}

Graph::Edge * Graph::findEdge(string title) {
    int index = findVertex(title); //find index of the show
    if (index == -1) { //if not found, then show is not there
        return nullptr;
    }
    //else, return connections that the edge is at
    return vertices[index].connections;
}

bool Graph::findShow(string title) {
    if (findVertex(title) == -1) {
        //if -1, show is not there
        return false;
    }
    return true;
}

void Graph::clear() {
    for (int i = 0; i < showCount; i++) {
        Edge* current = vertices[i].connections;
        //accessing a node in the graph (tv show) and then accessing its connections
        while (current != nullptr) {
            //looping thru the connections to clear them
            Edge* temp = current -> next;
            delete current;
            current = temp;
        }
        vertices[i].connections = nullptr;
    }
    //deleting the array
    delete [] vertices;
    //reseting
    vertices = nullptr;
    showCount = 0;
}
