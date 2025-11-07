#include "PlaylistManager.h"

PlaylistManager::PlaylistManager(const std::string& path) {
    load(path);
    if (playlists.find(ROOT) == playlists.end()) {
        playlists[ROOT] = {};
    }

    if (!playlists.empty()) {
        for (auto& [name, _] : playlists) {
            if (name != ROOT) {
                activePlaylist = name;
                break;
            }
        }
    }

    if (activePlaylist.empty()) {
        activePlaylist = "Default";
        playlists[activePlaylist]["front"] = { "", "END" };
        playlists[activePlaylist]["END"] = { "", "" };
    }
}

void PlaylistManager::load(const std::string& path) {
    playlists.clear();
    activePlaylist = "";

    std::ifstream in(path);
    if (!in.is_open()) return;

    std::string line, currentPlaylist;

    while (std::getline(in, line)) {
        if (line.empty()) continue;

        if (line.rfind("## Playlist:", 0) == 0) {
            currentPlaylist = line.substr(std::string("## Playlist:").length());
            currentPlaylist.erase(0, currentPlaylist.find_first_not_of(" \t"));

            playlists[currentPlaylist]["front"] = { "", "END" };
            playlists[currentPlaylist]["END"] = { "", "" };
            continue;
        }

        std::istringstream iss(line);
        std::string key, URL, val;
        if (!currentPlaylist.empty() &&
            std::getline(iss, key, '|') &&
            std::getline(iss, URL, '|') &&
            std::getline(iss, val, '|'))
        {
            if (key == "END") continue;
            if (val.empty()) val = "END";
            playlists[currentPlaylist][key] = { URL, val };
        }
    }

    in.close();

    if (!playlists.empty()) {
        for (auto& [name, _] : playlists) {
            if (name != ROOT) {
                activePlaylist = name;
                break;
            }
        }
    }

    if (activePlaylist.empty()) {
        activePlaylist = "Default";
        playlists[activePlaylist]["front"] = { "", "END" };
        playlists[activePlaylist]["END"] = { "", "" };
    }
}

void PlaylistManager::save(const std::string& path) {
    std::ofstream out(path);
    if (!out.is_open()) return;

    for (auto& [playlistName, pl] : playlists) {
        out << "## Playlist: " << playlistName << '\n';
        for (auto& [key, element] : pl) {
            out << key << '|' << element.first << '|' << element.second << '\n';
        }
        out << '\n';
    }

    out.close();
}

void PlaylistManager::add(const std::string& song, const std::string& URL) {
    if (activePlaylist.empty()) {
        activePlaylist = "Default";
        playlists[activePlaylist];
    }

    auto& playlist = playlists[activePlaylist];

    if (playlist.empty()) {
        playlist["front"] = { "", "END" };
        playlist["END"] = { "", "" };
    }

    std::string last = "front";
    while (playlist.count(last) && playlist[last].second != "END") {
        last = playlist[last].second;
        if (playlist.find(last) == playlist.end()) {
            last = "front";
            break;
        }
    }

    playlist[last].second = song;
    playlist[song] = { URL, "END" };
}

void PlaylistManager::remove(const std::string& song) {
    if (activePlaylist.empty() || playlists.find(activePlaylist) == playlists.end()) return;

    auto& playlist = playlists[activePlaylist];
    if (playlist.find("front") == playlist.end()) return;

    std::string parent = "front";
    std::string child = playlist[parent].second;

    while (child != "END" && child != song) {
        if (playlist.find(child) == playlist.end()) return;
        parent = child;
        child = playlist[child].second;
    }

    if (child != song) return;

    playlist[parent].second = playlist[child].second;
    playlist.erase(child);
}

std::string PlaylistManager::getNext( const std::string& song) {
    if (playlists.find(activePlaylist) == playlists.end()) return "END";
    if (playlists[activePlaylist].find(song) == playlists[activePlaylist].end()) return "END";
    return playlists[activePlaylist][song].second;
}

std::string PlaylistManager::getUrl(const std::string& playlistName, const std::string& song) {
    if (playlists.find(playlistName) == playlists.end()) return "";
    if (playlists[playlistName].find(song) == playlists[playlistName].end()) return "";
    return playlists[playlistName][song].first;
}

std::vector<std::pair<std::string, std::string>> PlaylistManager::getSongs(const std::string& playlistName) {
    std::vector<std::pair<std::string, std::string>> ret;
    if (playlists.find(playlistName) == playlists.end()) return ret;

    const auto& playlist = playlists.at(playlistName);
    if (playlist.find("front") == playlist.end()) return ret;

    std::string current = playlist.at("front").second;

    while (current != "END") {
        auto it = playlist.find(current);
        if (it == playlist.end()) break;
        ret.push_back({ current, it->second.first });
        current = it->second.second;
    }

    return ret;
}

std::vector<std::string> PlaylistManager::getPlaylistNames() const {
    std::vector<std::string> names;
    for (const auto& [name, _] : playlists) {
        if (name != ROOT)
            names.push_back(name);
    }
    return names;
}

void PlaylistManager::setActivePlaylist(const std::string& name) {
    if (playlists.find(name) != playlists.end())
        activePlaylist = name;
}

std::string PlaylistManager::getActivePlaylist() const {
    return activePlaylist;
}

void PlaylistManager::createNewPlaylist(const std::string& name)
{
    if (playlists.find(name) == playlists.end())
    {
        playlists[name] = { {"front", {"", "END"}} };
    }
}