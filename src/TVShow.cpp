#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;



int main(){

    ifstream file("../src/TMDB_tv_dataset_v3.csv");

    if(!file.is_open()){
        cerr<< "Error: file couldn't be opened.\n";
        return 1;
    }

    string line;
    getline(file,line);

    while(getline(file,line)){
        stringstream ss(line);
        string field;
        vector<string> fields;

        while(getline(ss,field,',')){
           fields.push_back(field);
        }

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