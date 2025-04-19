#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "TVShow.h"
#include <limits>
#include <cctype>
#include <algorithm>

using namespace std;


//constructor
TVShow::TVShow (std::string name, std::string genres, std::string creators, string networks) {
    this->name = name;
    this->genres = genres;
    this->creators = creators;
    this->networks = networks;
}

//prints information
void TVShow::print() {
    //prints out the information
    std::cout << "TV SHOW INFORMATION"<<endl;
    cout << "-------------------" << endl;
    std::cout << "TITLE: " << name << std::endl;
    std::cout <<"GENRES: " << genres << endl;
    std::cout<< "CREATED BY: " << creators << std::endl;
    cout<< "SHOW NETWORKS: " << networks << endl;
    cout << "\n";
}

//Old function to check for a match (didn't do exact match so if you typed "game", game of thrones appeared)
//Maybe can use for the genre matchings!
bool TVShow::checkifMatch(std::string userSearch) {
    //searching for the name of show
    //lower case for sensitivity purposes
    //citation: geeksforgeeks
    transform(userSearch.begin(), userSearch.end(), userSearch.begin(), ::tolower);
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    if (name.find(userSearch) != std::string::npos) {
        return true;
    }
    return false;
}

bool TVShow::checkExactMatch(string trimmedSearch, bool& showFound) {
    //converting title to lowercase so case won't matter for comparison
    string lowerCaseTitle = name; //name from class
    transform(lowerCaseTitle.begin(),lowerCaseTitle.end(),lowerCaseTitle.begin(),::tolower);
    //does an exact match with lowerCaseTitle and the user input so ONLY full titles provide a match
    //TVShow show(userInput, genres, creators);
    if(lowerCaseTitle == trimmedSearch){
        print();
        showFound = true;
    }
    return showFound;
}

/* function purpose: so user can input their search multiple ways without
 a whitespace changing the result */
string TVShow::trim (const string& s) {
    const string whitespace = " \t\r\n";
    //these are characters that can be considered a whitespace

    //trying to find the first whitespace character
    int first = s.find_first_not_of(whitespace); //citation: cplusplus
    if (first == string::npos) { //citation: geeksforgeeks
        //case: only whitespace, so return an empty string
        return "";
    }
    //position of the last nonwhitespace char
    int last = s.find_last_not_of(whitespace); //citation: cplusplus

    //string that contains no whitespace
    //citation: cplusplus
    return s.substr(first, last - first + 1);
}

//checks if string is number, this was for a trailing number that kept appearing in output
//not happening anymore but...
bool TVShow::number(const string& s) {
    if (s.empty()) {
        return false; //empty -> no digit
    }

    for (unsigned int i = 0; i < s.length(); i++) {
        if (!isdigit(s[i])) {
            return false; //if we can't convert to digit, not a number
        }
    }
    return true;
}

//reads a row from the file
vector <string> TVShow::readRow(istream& input) {
    vector <string> columns;
    string category;
    bool quotes = false;
    char c;

    while (input.get(c)) {
        //handling quotes
        if (c == '"') {
            if (quotes && input.peek() == '"') {
                category += '"';
                input.get();
            }
            else {
                quotes = !quotes;
            }
        }

        //handling commas
        else if (c == ',' && !quotes) {
            columns.push_back(category);
            category.clear();
        }
        else if (c == '\n' && !quotes) { //line ended
            columns.push_back(category);
            break;
        }
        else {
            category += c; //add character to column
        }
    }
    return columns; //columns from the row that was read
}

//gets column information
string TVShow::column(vector<string> row, int colNum) {
    if (colNum >= 0 && colNum < row.size()) {
        if (!row[colNum].empty()) {
            return row[colNum]; //if it is not empty & valid
        }
    }
    return "No information available.";
}

//accessing wanted columns, not all of the ones in csv
vector<string> TVShow::filterCol (vector<string> row, vector<int> index) {
    vector<string> columns;
    for (int i = 0; i < index.size(); i++) {
        if (i >= 0 && i < row.size()) {
            columns.push_back(row[i]);
        }
        else {
            columns.push_back("no info available");
        }
    }
    return columns;
}

//finding show, printing show name, genre, and who it was created by
void TVShow::findShow(string csvFile, string userinput) {
    ifstream file(csvFile);
    if (!file.is_open()) {
        cerr << "Error opening file " << csvFile << endl;
        return;
    }

    vector <string> header = readRow(file);

    //assigning -1 bc we haven't found them yet
    int colName = -1;
    int colGenres = -1;
    int colCreatedBy = -1;
    int colNetworks = -1;

    //assigning indices based on the csv column names
    for (int i = 0; i < header.size(); i++) {
        transform(header[i].begin(), header[i].end(), header[i].begin(), ::tolower);
        //citation: geeksforgeeks
        if (header[i]== "name") {
            colName = i;
        }
        if (header[i] == "genres") {
            colGenres = i;
        }
        if (header[i] == "created_by") {
            colCreatedBy = i;
        }
        if (header[i] == "networks") {
            colNetworks = i;
        }
    }

    if (colName <0 || colGenres < 0 || colCreatedBy < 0 || colNetworks < 0) {
        cerr << "error in findShow";
        return;
    }

    //user input trimmed and made all lowercase
    string trimmedSearch = trim(userinput);
    transform(trimmedSearch.begin(), trimmedSearch.end(), trimmedSearch.begin(), ::tolower);
    //citation: geeksforgeeks

    bool showFound = false;
    while (true) {
        vector<string> row = readRow(file);
        if (row.empty()) {
            break;
        }

        //trimming the categories
        string title = trim(row[colName]);
        string genres = trim(row[colGenres]);
        string creators = trim(row[colCreatedBy]);
        string networks = trim(row[colNetworks]);

        //this while statement is dealing with multiple creators and commas
        while (row.size() > colCreatedBy+1) {
            string followingCreator = trim(row[colCreatedBy+1]);
            if (followingCreator.size() > 2 && !number(followingCreator)) {
                creators += ", " + followingCreator; //add that creator
            }
            row.erase(row.begin()+colCreatedBy+1);
        }
        //tv show instance
        TVShow myShow(title,genres,creators,networks);
        //calls function to check for an exact match of that tv show title in our data set
        myShow.checkExactMatch(trimmedSearch, showFound);
    }

    if (!showFound) {
        cout << "Sorry! No titles match :( Please try again.";
    }
}

int main() {
    cout << "Search for show title: " << endl;
    string search;
    getline(cin, search);

    TVShow userShow(" ", " ", " "," ");
    userShow.findShow("../src/TMDB_tv_dataset_v3.csv", search);
    return 0;
}