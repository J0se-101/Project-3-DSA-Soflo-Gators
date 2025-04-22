#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "TVShow.h"
#include <cctype>
#include <algorithm>
#include <unordered_set>
#include <random>
#include <iomanip>
#include <chrono>

using namespace std;

unordered_map<string, TVShow> TVShow::TVShowsMap;
HashMaps TVShow::CustomMap;

//default constructor
TVShow::TVShow () {
    name = "";
    genres = "";
    creators = "";
    networks = "";
    vote_count = 0;
    vote_average = 0.0;
}

//constructor
TVShow::TVShow (std::string name, std::string genres, std::string creators, string networks, int vote_count, float vote_average) {
    this->name = name;
    this->genres = genres;
    this->creators = creators;
    this->networks = networks;
    this->vote_count = vote_count;
    this->vote_average = vote_average;
}

//prints information
void TVShow::print() {
    //prints out the information
    cout << "\nTV SHOW INFORMATION"<<endl;
    cout << "-------------------" << endl;
    cout << "TITLE: " << name << std::endl;
    cout <<"GENRES: " << genres << endl;
    cout<< "SHOW NETWORKS: " << networks << endl;
    cout << fixed<< setprecision(1);
    cout<< "RATING: " << vote_average << "/10"<< endl;
    cout<< "CREATED BY: " << creators << std::endl;
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

//Not used anymore, we used this before we had the hash map
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

//accessing wanted columns, not all the ones in csv
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
    ifstream file(csvFile, ios::binary);
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

    //only builds our master map once if it wasn't built previously
    if(TVShowsMap.empty()) {
        cout << "loading shows ...\n" << endl;
        //reading each row from csv file
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
            while (row.size() > colCreatedBy + 1) {
                string followingCreator = trim(row[colCreatedBy + 1]);
                if (followingCreator.size() > 2 && !number(followingCreator)) {
                    creators += ", " + followingCreator; //add that creator
                }
                row.erase(row.begin() + colCreatedBy + 1);
            }

            //making lowercase and tv show instance
            string lowerCaseTitle = title;
            transform(lowerCaseTitle.begin(), lowerCaseTitle.end(), lowerCaseTitle.begin(), ::tolower);
            TVShow myShow(title, genres, creators, networks, 0, 0.0);
            //adding tv show to the hash map
            TVShowsMap[lowerCaseTitle] = myShow;
        }
    }
    //using .find function of a hash map to find a match for the inputted tv show!
    if(TVShowsMap.find(trimmedSearch)!=TVShowsMap.end()){
      TVShowsMap[trimmedSearch].print();
    } else {
        cout << "Sorry! No titles match :( Please try again.\n";
    }
}

//if there are multiple genres/networks, this will seperate them
vector<string> TVShow::splitstring(string s, char delimiter) {
    vector<string> splitStrings;
    stringstream ss(s);
    string singleGenre;

    while (getline(ss, singleGenre, delimiter)) {
        splitStrings.push_back(trim(singleGenre));
    }
    return splitStrings;
}

unordered_map<string, vector<string>> TVShow::genreGraphBuilder(unordered_map<string, vector<string>> &genreMap) {
    unordered_map<string, vector<string>> genreGraph;
    //graph
    //looping through genres
    for (auto genreIter = genreMap.begin(); genreIter != genreMap.end(); genreIter++) {
        //value is shows, key is genre
        vector<string> shows = genreIter->second;
        //connecting the shows that share same genres
        for (int show1 = 0; show1 < shows.size(); show1++) {
            string showOne = shows[show1];
            transform(showOne.begin(), showOne.end(), showOne.begin(), ::tolower);
            for (int show2 = 0; show2 < shows.size(); show2++) {
                //this is to prevent shows from linking to themselves
                if (show1 != show2) {
                    //if the show isn't the same, then add an edge!
                    string showTwo = shows[show2];
                    transform(showTwo.begin(), showTwo.end(), showTwo.begin(), ::tolower);
                    genreGraph[showOne].push_back(showTwo);
                    genreGraph[showTwo].push_back(showOne);
                }
            }
        }
    }
    return genreGraph;
}

unordered_map<string, vector<string>> TVShow::networkGraphBuilder(unordered_map<string, vector<string>> &networkMap) {
    unordered_map<string, vector<string>> networkGraph;
    //looping through networks
    for (auto networkIter = networkMap.begin(); networkIter != networkMap.end(); networkIter++) {
        vector<string> shows = networkIter->second;
        //value is shows, key is network
        //connecting the shows that share same network
        for (int show1 = 0; show1 < shows.size(); show1++) {
            string showOne = shows[show1];
            transform(showOne.begin(), showOne.end(), showOne.begin(), ::tolower);
            for (int show2 = 0; show2 < shows.size(); show2++) {
                //don't want a show linking to itself
                if (show1 != show2) {
                    //if the show isn't the same, then add an edge!
                    string showTwo = shows[show2];
                    transform(showTwo.begin(), showTwo.end(), showTwo.begin(), ::tolower);
                    networkGraph[showOne].push_back(showTwo);
                }
            }
        }
    }
    return networkGraph;
}


unordered_map<string, vector<string>> TVShow::populateGenres(unordered_map<string, TVShow> TVShowsMap) {
    unordered_map<string, vector<string>> similarGenres;
    //key is the genre, vector is the vector of show titles
    for (auto iter = TVShowsMap.begin(); iter != TVShowsMap.end(); iter++) {
        string title = (iter->first);
        TVShow show = iter->second;

        string genres = show.genres;
        vector <string> splitGenres = splitstring(genres, ',');
        //splitting the genres based on the delimiter ','
        for (int i = 0; i < splitGenres.size(); i++) {
            string genre = splitGenres[i];
            //based on genre, add titles with same genre
            similarGenres[genre].push_back(title);
        }
    }
    return similarGenres;
}

unordered_map<string, vector<string>> TVShow::populateNetworks(unordered_map<string, TVShow> TVShowsMap) {
    unordered_map<string, vector<string>> similarNetworks;
    //key is the network, vector is the show titles
    for (auto iter = TVShowsMap.begin(); iter != TVShowsMap.end(); iter++) {
        string title =iter->first;
        TVShow show = iter->second;

        string networks = show.networks;
        vector <string> splitNetworks = splitstring(networks, ',');
        //splitting the networks based on the delimiter ','
        for (int i = 0; i < splitNetworks.size(); i++) {
            string network = splitNetworks[i];
            //based on network, add titles with same network
            similarNetworks[network].push_back(title);
        }
    }
    return similarNetworks;
}

//custom hash map recommend functions: used for quick lookup for finding the tv show object by title
void TVShow::recommendByGenreHash(const string& inputTitle,const unordered_map<string, vector<string>>& genreMap) {
    //Normalize lookup key
    string key = trim(inputTitle);
    bool printed = false;
    transform(key.begin(), key.end(), key.begin(), ::tolower);

    //Ensuring that the show exists
    TVShow* show = CustomMap.getShow(key);
    if (!show) {
        cout << "No titles match :(\n";
        return;
    }

    //Splits its genres into individual categories
    vector<string> categories = splitstring(show->genres, ',');
    unordered_set<string> recs;

    //For each genre bucket, we collect other shows
    for (auto& genre : categories) {
        auto itBucket = genreMap.find(genre);
        if (itBucket == genreMap.end()) continue;
        for (auto& other : itBucket->second) {
            if (other != key) recs.insert(other);
        }
    }

    //Prints recommendations
    if (recs.empty()) {
        cout << "No recommendations available.\n";
    }
    //Citation: Geeks for Geeks
    else {
        vector<string> finalRecs(recs.begin(),recs.end());
        random_device randomValue;
        mt19937 g(randomValue());
        shuffle(finalRecs.begin(),finalRecs.end(),g);
        int count = 0;
        for (auto& title : finalRecs) {
            //limits to 10 recommendations outputted, makes show object to check rating
            //only provides shows with >=500 votes and >= 6.0 avg. rating
            TVShow* rec = CustomMap.getShow(title);
            if(rec && rec->vote_count>=500 && rec->vote_average>=6.0){
                if(count++==10) break;
                cout << count << ". " << rec->name << "\n";
                printed = true;
            }
        }
    }
    if (!printed){
        cout << "No recommendations available." << endl;
    }
}

void TVShow::recommendByNetworkHash(const string& inputTitle,const unordered_map<string, vector<string>>& networkMap) {
    //Normalize lookup key
    string key = trim(inputTitle);
    bool printed = false;
    transform(key.begin(), key.end(), key.begin(), ::tolower);

    //Ensuring that the show exists
    TVShow* show = CustomMap.getShow(key);
    if (!show) {
        cout << "No titles match :(\n";
        return;
    }

    //Splits its genres into individual categories
    vector<string> categories = splitstring(show->networks, ',');
    unordered_set<string> recs;

    //For each genre bucket, we collect other shows
    for (auto& genre : categories) {
        auto itBucket = networkMap.find(genre);
        if (itBucket == networkMap.end()) continue;
        for (auto& other : itBucket->second) {
            if (other != key) recs.insert(other);
        }
    }

    //Prints recommendations
    if (recs.empty()) {
        cout << "No recommendations available.\n";
    }
    //Citation: Geeks for Geeks
    else {
        vector<string> finalRecs(recs.begin(),recs.end());
        random_device randomValue;
        mt19937 g(randomValue());
        shuffle(finalRecs.begin(),finalRecs.end(),g);
        int count = 0;
        for (auto& title : finalRecs) {
            //limits to 10 recommendations outputted, makes show object to check rating
            //only provides shows with >=500 votes and >= 6.0 avg. rating
            TVShow* rec = CustomMap.getShow(title);
            if(rec && rec->vote_count>=500 && rec->vote_average>=6.0){
                if(count++==10) break;
                cout << count << ". " << rec->name << "\n";
                printed = true;
            }
        }
    }
    if (!printed){
        cout << "No recommendations available." << endl;
    }
}

void TVShow::recommendByGenreGraph(const string& inputTitle,const Graph& g) {
    // Normalize lookup key
    string key = trim(inputTitle);
    transform(key.begin(), key.end(), key.begin(), ::tolower);

    // Ensure the show exists in the graph
    if (!g.hasShow(key)) {
        cout << "No titles match :(\n";
        return;
    }

    // Ask the graph for all neighbors (other shows sharing any genre)
    auto recs = g.getNeighbors(key);

    // Randomize
    random_device rd;
    mt19937 gen(rd());
    shuffle(recs.begin(), recs.end(), gen);

    // Print up to 10, filtering by your vote criteria
    int count = 0;
    for (auto& title : recs) {
        TVShow* rec = CustomMap.getShow(title);
        if (rec && rec->vote_count >= 500 && rec->vote_average >= 6.0) {
            cout << ++count << ". " << rec->name << "\n";
            if (count == 10) break;
        }
    }
    if (count == 0) {
        cout << "No recommendations available.\n";
    }
}

void TVShow::recommendByNetworkGraph(
    const string& inputTitle,
    const Graph& g
) {
    // Normalize lookup key
    string key = trim(inputTitle);
    transform(key.begin(), key.end(), key.begin(), ::tolower);

    // Ensure the show exists in the graph
    if (!g.hasShow(key)) {
        cout << "No titles match :(\n";
        return;
    }

    // Ask the graph for all neighbors (other shows sharing any network)
    auto recs = g.getNeighbors(key);

    // Randomize
    random_device rd;
    mt19937 gen(rd());
    shuffle(recs.begin(), recs.end(), gen);

    // Print up to 10, filtering by your vote criteria
    int count = 0;
    for (auto& title : recs) {
        TVShow* rec = CustomMap.getShow(title);
        if (rec && rec->vote_count >= 500 && rec->vote_average >= 6.0) {
            cout << ++count << ". " << rec->name << "\n";
            if (count == 10) break;
        }
    }
    if (count == 0) {
        cout << "No recommendations available.\n";
    }
}

void TVShow::loadAllShows(const string& csvFile) {
    ifstream file(csvFile, ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file " << csvFile << endl;
        return;
    }
    //reads header row to find column indices
    vector<string> header = readRow(file);
    int colName = -1, colGenres = -1, colCreatedBy = -1, colNetworks = -1, colVoteCount=-1, colVoteAverage=-1;
    for (int i = 0; i < header.size(); ++i) {
        string h = header[i];
        transform(h.begin(), h.end(), h.begin(), ::tolower);
        if (h == "name")        colName    = i;
        else if (h == "genres") colGenres  = i;
        else if (h == "created_by")  colCreatedBy = i;
        else if (h == "networks")    colNetworks  = i;
        else if (h == "vote_count")    colVoteCount  = i;
        else if (h == "vote_average")    colVoteAverage  = i;
    }
    if (colName<0 || colGenres<0 || colCreatedBy<0 || colNetworks<0 || colVoteCount<0||colVoteAverage <0) {
        cerr << "CSV header missing required columns\n";
        return;
    }

    //reads each data row
    while (true) {
        vector<string> row = readRow(file);
        if (row.empty()) break;


        int requiredCols = max({colName,colGenres,colCreatedBy,colNetworks,colVoteCount,colVoteAverage});
        if(row.size()<=requiredCols){
            continue;
        }

        // trim & extract
        auto t = trim(row[colName]);
        auto g = trim(row[colGenres]);
        auto c = trim(row[colCreatedBy]);
        auto n = trim(row[colNetworks]);
        int vc = stoi(trim(row[colVoteCount]));
        float va = stod(trim(row[colVoteAverage]));
        //used for debugging, keep until finally done in case we run into issues
        /*int vc=0;
        float va=0.0;
        string voteCountStr = trim(row[colVoteCount]);
        string voteAvgStr = trim(row[colVoteAverage]);
        if(!voteCountStr.empty()){
            vc=stoi(voteCountStr);
        }
        if(!voteAvgStr.empty()){
            va=stof(voteAvgStr);
        }*/

        //consume multi‑part creators if any
        while (row.size() > colCreatedBy+1) {
            if(colCreatedBy+1>=row.size()){
                break;
            }
            string extra = trim(row[colCreatedBy+1]);
            if (extra.size()>2 && !number(extra)) {
                c += ", " + extra;
                row.erase(row.begin() + colCreatedBy+1);
            } else break;
        }

        //insert into the map by lowercase title
        string key = t;

        transform(key.begin(), key.end(), key.begin(), ::tolower);
        TVShowsMap.emplace(key, TVShow(t, g, c, n, vc, va));
        CustomMap.insert(key, new TVShow(t, g, c, n, vc, va));
    }

    file.close();
    cout << "Loaded " << TVShowsMap.size() << " shows <3.\n" << endl;
}

int main() {
    //Load and parse the CSV once
    TVShow::loadAllShows("../src/TMDB_tv_dataset_v3.csv");

    //Build both graphs once at startup
    TVShow helper;
    auto genreMap    = helper.populateGenres(TVShow::TVShowsMap);
    auto networkMap  = helper.populateNetworks(TVShow::TVShowsMap);

    //Build graph‐based buckets
    int totalShows = TVShow::TVShowsMap.size();
    Graph genreGraph(totalShows), networkGraph(totalShows);

    // Add every show as a vertex
    for (auto &entry : TVShow::TVShowsMap) {
        const string &titleKey = entry.first;
        genreGraph.addShow(titleKey);
        networkGraph.addShow(titleKey);
    }

    //Bucket entries for genres
    for (auto &b : genreMap) {
        for (auto &titleKey : b.second) {
            genreGraph.addBucketEntry(b.first, titleKey);
        }
    }
    //Bucket entries for networks
    for (auto &b : networkMap) {
        for (auto &titleKey : b.second) {
            networkGraph.addBucketEntry(b.first, titleKey);
        }
    }

    // Menu loop

    cout << "DSA Project 3: Soflo Gators!\n\n"
         << "Type '1' to search for a TV show.\n"
         << "Type '2' to receive a recommendation based on a genre.\n"
         << "Type '3' to receive a recommendation based on network.\n"
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
            cout <<"Execution Time: " << duration.count()<< " microseconds!"<< endl;
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
            cout <<"Execution Time: " << duration.count()<< " microseconds!"<< endl;
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
            cout <<"Hash Map Execution Time: " << duration.count()<< " microseconds!"<< endl;
            cout << "\n";
        }
        else if (choice == "5") {
            cout << "Enter a show title to receive a recommendation based on network using HashMap: ";
            string title;
            getline(cin, title);
            helper.recommendByNetworkHash(title, networkMap);
            cout << "\n";
        }
        else {
            cout << "Invalid input.\n\n";
        }
    }
    return 0;
}
