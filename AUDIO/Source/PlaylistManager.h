#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;

class PlaylistManager {
private:
    map<string, pair<string, string>> playlist;
    const std::string ROOT = "front";
public:
    PlaylistManager(const string& path);

    void load(const string& path);

    void save(const string& path);

    void add(const string& key, const string& nextSong, const string& nextURL);

    void remove(const string& key);

    string getNext(const string& key);

    string getUrl(const string& key);

    vector<pair<string, string>> getSongs(const std::string& start_key);

};
