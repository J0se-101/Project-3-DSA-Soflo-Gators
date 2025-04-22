#include "graph.h"
#include <algorithm>

Graph::Graph(int maxShows)
    : maxShows(maxShows)
{
    vertices.reserve(maxShows);
}

int Graph::findVertex(const string& title) const {
    for (int i = 0; i < vertices.size(); i++) {
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

void Graph::addShow(const string& title) {
    if (vertices.size() >= maxShows) return;
    if (findVertex(title) == -1) {
        // create a new vertex with no neighbors
        vertices.push_back({title});
    }
}

void Graph::addBucketEntry(const string& featureKey, const string& title) {
    int index = findVertex(title);
    if (index < 0) return;
    // find or create bucket
    for (auto &bucket : buckets) {
        if (bucket.key == featureKey) {
            bucket.shows.push_back(index);
            return;
        }
    }
    // new bucket
    buckets.push_back({featureKey, {index}});
}

bool Graph::hasShow(const string& title) const {
    return findVertex(title) != -1;
}

vector<string> Graph::getNeighbors(const string& title) const {
    vector<string> result;
    int idx = findVertex(title);
    if (idx < 0) return result;

    vector<int> recs;
    // for each bucket containing idx, collect peers
    for (auto &bucket : buckets) {
        // check if this show is in bucket
        if (std::find(bucket.shows.begin(), bucket.shows.end(), idx)
            != bucket.shows.end())
        {
            // add all other shows from this bucket
            for (int peer : bucket.shows) {
                if (peer != idx &&
                    find(recs.begin(), recs.end(), peer) == recs.end())
                {
                    recs.push_back(peer);
                }
            }
        }
    }
    // translate indices to titles
    for (int ver_in : recs) {
        result.push_back(vertices[ver_in].title);
    }
    return result;
}

void Graph::clear() {
    vertices.clear();
    buckets.clear();
}


