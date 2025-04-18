//
// Created by josed on 4/16/2025.
//
#ifndef TVSHOW_H
#define TVSHOW_H
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class TVShow {
public:
    std::string name;
    std::string genres;
    std:: string creators;
    string networks;

    //default constructor
    TVShow();
    //constructor
    TVShow(std::string name, std::string genres, std::string creators, string networks);
    //prints out the information
    void print();
    //searching for the name of show
    bool checkifMatch (std::string userSearch);
    //trimming whitespaces in user input
    static std::string trim (const std::string& s);
    //this function was to hard code the 2750 out but its not working...
    bool number(const std::string& s);
    //reads the row
    std::vector <std::string> readRow(std::istream& input);
    //gets column information
    std::string column(std::vector<std::string> row, int colNum);
    //there are extra columns in the database we don't want, this is to filter them out
    std::vector<std::string> filterCol (std::vector<std::string> row, std::vector<int> index);
    //finding show
    void findShow(std::string csvFile, std::string userinput);

    //finding exact show match based on title
    bool checkExactMatch(string trimmedSearch, bool& showFound);
    unordered_map<string, vector<string>> populateGenres(unordered_map<string, TVShow> TVShowsMap);
    unordered_map<string, vector<string>> populateNetworks(unordered_map<string, TVShow> TVShowsMap);

    //helper function for networks and genres
    vector<string> splitstring(string s, char delimiter);
    //building the graph
    unordered_map<string, vector<string>> graphBuilder(unordered_map<string, vector<string>>& genreMap, unordered_map<string, vector<string>>& networkMap);
private:
    unordered_map<string, vector<string>> graph; //graph
    unordered_map<string, TVShow>  TVShowsMap; //our hash table
    unordered_map<string, vector<string>> genreMap; //genres
    unordered_map<string, vector<string>> networkMap; //networks
};



#endif //TVSHOW_H
