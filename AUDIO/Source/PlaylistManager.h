#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>

class PlaylistManager {
private:
    std::map<std::string, std::map<std::string, std::pair<std::string, std::string>>> playlists;
    const std::string ROOT = "front";

    std::string activePlaylist;

public:
    PlaylistManager(const std::string& path);

    void load(const std::string& path);
    void save(const std::string& path);

    void add(const std::string& song, const std::string& URL);
    void remove(const std::string& song);

    std::string getNext(const std::string& song);
    std::string getUrl(const std::string& playlistName, const std::string& song);

    std::vector<std::pair<std::string, std::string>> getSongs(const std::string& playlistName);

    std::vector<std::string> getPlaylistNames() const;

    void setActivePlaylist(const std::string& name);
    std::string getActivePlaylist() const;

    void createNewPlaylist(const std::string& name);
};
