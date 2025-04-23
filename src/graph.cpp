#include "graph.h"
#include <algorithm>

Graph::Graph(const vector<string> &titles) {
    { vertices.reserve(titles.size());
        for (const string title : titles) {
            vertices.push_back({title});
        }
    }
}


int Graph::findVertex(const string& title) const {
    int left = 0;
    int right = int(vertices.size() - 1);

    while (left <= right) {
        int mid = left + (right - left) / 2;
        string midpointTitle = vertices[mid].title;

        if (midpointTitle == title) {
            return mid;
        }

        if (midpointTitle < title) {
            left = mid + 1;
        }

        else {
            right = mid - 1;
        }
    }
    return -1;
}


Graph::~Graph() {
    clear();
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


void Graph::buildBuckets(vector<pair<string, vector<string>>> &graph) {
    //clear buckets and reserve space
    buckets.clear();
    //citation: reserve() --> cplusplus
    buckets.reserve(graph.size());
    for (size_t i = 0; i < graph.size(); ++i) {
        string titleName = graph[i].first;
        vector<string> showTitles = graph[i].second;

        //make a new bucket
        Bucket bucketbuilding;
        bucketbuilding.key = titleName;

        //making sure that theres enough room to push back
        bucketbuilding.shows.reserve(showTitles.size());
        for (int j = 0; j < showTitles.size(); ++j) {
            const string showTitle = showTitles[j];
            //find index of the jth title
            int index = findVertex(showTitle);
            //if found, record in bucket
            if (index >= 0) {
                bucketbuilding.shows.push_back(index);
            }
        }
        //add bucket to graph
        buckets.push_back(move(bucketbuilding));
    }
}


