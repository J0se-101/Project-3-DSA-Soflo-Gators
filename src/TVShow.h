//
// Created by josed on 4/16/2025.
//
#ifndef TVSHOW_H
#define TVSHOW_H
#include <string>
#include <vector>
using namespace std;

class TVShow {
public:
    std::string name;
    std::string genres;
    std:: string creators;
    string networks;

    //constructor
    TVShow(std::string name, std::string genres, std::string creators, string networks);
    //prints out the information
    void print();
    //searching for the name of show
    bool checkifMatch (std::string userSearch);
    //trimming whitespaces in user input
    std::string trim (const std::string& s);
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
};



#endif //TVSHOW_H
