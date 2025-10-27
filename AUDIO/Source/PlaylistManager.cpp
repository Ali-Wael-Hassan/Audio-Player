#include "PlaylistManager.h"

PlaylistManager::PlaylistManager(const string& path) {
    load(path);
    if (playlist.find(ROOT) == playlist.end()) {
        playlist[ROOT] = { "", "" };
    }
}

void PlaylistManager::load(const string& path) {
    playlist.clear();
    ifstream in(path);
    string key, val, URL;

    while (in >> key >> URL >> val) {
        playlist[key] = { URL, val };
    }
    in.close();
}

void PlaylistManager::save(const string& path) {
    ofstream out(path);

    for (auto& [key, element] : playlist) {
        out << key << ' ' << element.first << ' ' << element.second << '\n';
    }

    out.close();
}

void PlaylistManager::add(const string& key, const string& nextSong, const string& nextURL) {
    auto it = playlist.find(key);
    if (it != playlist.end()) {
        it->second.second = nextSong;
        playlist[nextSong] = { nextURL, "" };
    }
}

void PlaylistManager::remove(const string& key) {
    if (key == ROOT || playlist.size() <= 1) return;
    string parent = ROOT, child = playlist[parent].second;

    while (playlist[child].second != "" && child != key) {
        parent = child;
        child = playlist[child].second;
    }

    if (child != key) return;
    playlist[parent].second = playlist[child].second;
    playlist.erase(child);
}

string PlaylistManager::getNext(const string& key) {
    return playlist[key].second;
}

string PlaylistManager::getUrl(const string& key) {
    if (playlist.find(key) == playlist.end()) return "";
    return playlist[key].first;
}

vector<pair<string, string>> PlaylistManager::getSongs(const std::string& start_key) {

    vector<pair<string, string>> ret;

    std::string current_key;
    if (start_key == ROOT) {
        current_key = playlist[ROOT].second;
    }
    else {
        current_key = start_key;
    }

    while (!current_key.empty() && playlist.count(current_key)) {
        const auto& data = playlist[current_key];

        ret.push_back({ data.first, data.first });

        current_key = data.second;
    }

    return ret;
}