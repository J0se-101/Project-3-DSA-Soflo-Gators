#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <vector>
using namespace std;


class Graph {

private:

    struct Vertex {
        string title;
    };

    struct Bucket {
        string key;
        vector<int> shows;  // indices of shows in this bucket
    };

    vector<Vertex> vertices;
    vector<Bucket> buckets;
    int maxShows;

    // Find the index of a vertex by title, or -1 if not found
    int findVertex(const string& title) const;

    void clear();

    public:
    ~Graph();
    explicit Graph(int maxShows);

    // Add a vertex for a show title (no duplicates)
    void addShow(const string& title);

    void addBucketEntry(const string& featureKey, const string& title);


    // Check if a show exists in the graph
    bool hasShow(const string& title) const;

    vector<string> getNeighbors(const string& title) const;

};


#endif //GRAPH_H
