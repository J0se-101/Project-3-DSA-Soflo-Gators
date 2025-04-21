//
// Created by mikayla cohen on 4/21/25.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <string>
using namespace std;


class Graph {
private:
    struct Edge {
        string connectedShow;
        Edge* next;
    };

    struct Vertex {
        string title;
        Edge* connections;
    };

    Vertex* vertices;
    int maxShows;
    int showCount;

    int findVertex(string title);

    public:
    Graph();
    ~Graph();
    Graph(int maxShows);
    void addShow(string title);
    //links two tv shows
    void addConnection(string title, string connection);
    //get edges for the show
    Edge* findEdge(string title);
    bool findShow(string title);
    void clear();
};



#endif //GRAPH_H
