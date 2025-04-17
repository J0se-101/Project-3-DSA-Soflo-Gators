#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "TVShow.h"

using namespace std;

std::vector<std::string> parseCSV(const std::string& line) {
    vector<string> result;
    string category;
    bool quoted = false;

    for (int i = 0; i < line.length(); i++) {
        if (line[i] == '"') {
            quoted = !quoted;
        }
        else if (line[i] == ',' && quoted) {
            result.push_back(category);
            category.clear();
        }
        else {
            category += line[i];
        }
    }

    result.push_back(category);
    return result;
}

int main(){

    ifstream file("../src/TMDB_tv_dataset_v3.csv");

    if(!file.is_open()){
        cerr<< "Error: file couldn't be opened.\n";
        return 1;
    }

    string line;
    getline(file,line);

    while(getline(file,line)){

        vector<string> fields = parseCSV(line);
        // stringstream ss(line);
        // string field;
        // vector<string> fields;
        //
        // while(getline(ss,field,',')){
        //    fields.push_back(field);
        // }
        //git hub loves us
        for (int i = 0; i < fields.size(); i++) {
            cout << i << ": " << fields[i] << endl;
        }

        cout << "--------------------\n";

        if(fields.size()>21){
            string title=fields[1];
            string genres = fields[20];
            string creators = fields[21];

            cout << "Title: " << title << "\n";
            cout << "Genres: " << genres << "\n";
            cout << "Created By: " << creators << "\n";
            cout << "-----------------------\n";
        }
    }
file.close();
    return 0;
};