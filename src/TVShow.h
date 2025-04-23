//
// Created by josed on 4/16/2025.
//
#ifndef TVSHOW_H
#define TVSHOW_H
#include <string>
#include <vector>
#include <unordered_map>
#include "hashmaps.h"
#include "graph.h"
using namespace std;
#include <iostream>

class TVShow {
public:
    string name;
    string genres;
    string creators;
    string networks;
    int vote_count;
    float vote_average;

    static unordered_map<string, TVShow>  TVShowsMap; //our hash table
    //new hash table
    static HashMaps CustomMap;

    //default constructor
    TVShow();
    //constructor
    TVShow(std::string name, std::string genres, std::string creators, string networks, int vote_count, float vote_average);
    //prints out the information
    void print();

    //trimming whitespaces in user input
    static std::string trim (const std::string& s);
    //this function was to hard code the 2750 out, but it is not working...
    static bool number(const std::string& s);
    //reads the row
    static std::vector <std::string> readRow(std::istream& input);
    //finding show
    void findShow(std::string csvFile, std::string userinput);

    unordered_map<string, vector<string>> populateGenres(unordered_map<string, TVShow> TVShowsMap);
    unordered_map<string, vector<string>> populateNetworks(unordered_map<string, TVShow> TVShowsMap);

    //helper function for networks and genres
    static vector<string> splitstring(string s, char delimiter);

    //recommending based on genre or network (graph)
    void recommendByGenreGraph(const string& inputTitle, const Graph& g);
    void recommendByNetworkGraph(const string& inputTitle, const Graph& g);

    //load every show into the static map once
    static void loadAllShows(const string& csvFile);

    //recommending based on genre or network (Hash-map)
    void recommendByNetworkHash(const string& inputTitle,const unordered_map<string, vector<string>>& networkMap);
    void recommendByGenreHash(const string& inputTitle,const unordered_map<string, vector<string>>& genreMap);

private:
    unordered_map<string, vector<string>> graph; //graph
    unordered_map<string, vector<string>> genreMap; //genres
    unordered_map<string, vector<string>> networkMap; //networks
};



#endif //TVSHOW_H
