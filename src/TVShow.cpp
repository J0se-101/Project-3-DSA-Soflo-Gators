#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;



int main(){
    ifstream file("TMDB_tv_dataset_v3");
    string line;
    getline(file,line);

    while(getline(file,line)){
        stringstream ss(line);
        string field;
        vector<string> fields;

        while(getline(ss,field,',')){
           fields.push_back(field);
        }
    }

    return 0;
};