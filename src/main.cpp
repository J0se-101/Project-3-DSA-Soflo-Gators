using namespace std;
#include "graph.h"
#include "hashmaps.h"
#include "TVShow.h"
#include <chrono>
#include <algorithm>

int main() {
    //Load and parse the CSV once
    TVShow::loadAllShows("../src/TMDB_tv_dataset_v3.csv");

    //Build both graphs once at startup
    TVShow helper;
    auto genreMap    = helper.populateGenres(TVShow::TVShowsMap);
    auto networkMap  = helper.populateNetworks(TVShow::TVShowsMap);

    //copying our genre and networks into vectors
    vector<pair<string, vector<string>>> genreList (genreMap.begin(), genreMap.end());
    vector <pair<string, vector<string>>> networkList (networkMap.begin(), networkMap.end());

    //gather the keys (titles) into a vector
    vector<string> titles;
    //reserve (citation: cplusplus) needed for optimization
    titles.reserve(TVShow::TVShowsMap.size());
    for (auto tvShow: TVShow::TVShowsMap) {
        titles.push_back(tvShow.first);
        //pushing back show title
    }

    //sort titles alphabetically
    sort(titles.begin(), titles.end());
    //create graph objects for genres and network
    Graph genreGraph(titles);
    Graph networkGraph(titles);

    //build buckets!
    genreGraph.buildBuckets(genreList);
    networkGraph.buildBuckets(networkList);

    // Menu loop

    cout << "DSA Project 3: Soflo Gators!\n\n"
         << "Type '1' to search for a TV show.\n"
         << "Type '2' to receive a recommendation based on a genre using Graph.\n"
         << "Type '3' to receive a recommendation based on network using Graph.\n"
         << "Type '4' to receive a recommendation based on genre using HashMap.\n"
         << "Type '5' to receive a recommendation based on network using HashMap.\n"
         << "Type 'exit' to quit.\n\n";

    string choice;

    while(true){

        cout << "Please input here: ";
        getline(cin, choice);
        choice = TVShow::trim(choice);
        transform(choice.begin(), choice.end(), choice.begin(), ::tolower);

        if(choice == "exit"){
            break;
        }
        else if (choice == "1") {
            cout << "Search for show title: ";
            string title;
            getline(cin, title);
            helper.findShow("../src/TMDB_tv_dataset_v3.csv", title);
            cout << "\n";
        }
        else if (choice == "2") {
            cout << "Enter a show title to receive a recommendation based on genre using Graph: ";
            string title;
            getline(cin, title);

            //timing code
            auto start = chrono::high_resolution_clock::now();
            helper.recommendByGenreGraph(title, genreGraph);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end-start);
            cout <<"Graph for Genres Execution Time: " << duration.count()<< " microseconds!"<< endl;
            cout << "\n";
            cout << "\n";
        }
        else if (choice == "3") {
            cout << "Enter a show title to receive a recommendation based on network using Graph: ";
            string title;
            getline(cin, title);

            //timing code
            auto start = chrono::high_resolution_clock::now();
            helper.recommendByNetworkGraph(title, networkGraph);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end-start);
            cout <<"Graph for Networks Execution Time: " << duration.count()<< " microseconds!"<< endl;
            cout << "\n";
            cout << "\n";
        }
        else if (choice == "4") {
            cout << "Enter a show title to receive a recommendation based on genre using HashMap: ";
            string title;
            getline(cin, title);

            //timing code, can replicate for other timers!
            auto start = chrono::high_resolution_clock::now();
            helper.recommendByGenreHash(title, genreMap);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end-start);
            cout <<"Hash Map for Genres Execution Time: " << duration.count()<< " microseconds!"<< endl;
            cout << "\n";
        }
        else if (choice == "5") {
            cout << "Enter a show title to receive a recommendation based on network using HashMap: ";
            string title;
            getline(cin, title);
            auto start = chrono::high_resolution_clock::now();
            helper.recommendByNetworkHash(title, networkMap);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end-start);
            cout <<"Hash Map for Networks Execution Time: " << duration.count()<< " microseconds!"<< endl;
            cout << "\n";
        }
        else {
            cout << "Invalid input.\n\n";
        }
    }
    return 0;
}
