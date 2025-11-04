#pragma once

// JUCE Header
#include <JuceHeader.h>

// map for lookup
#include <map>
#include <string>

class LibraryPage;

struct SongData
{
	juce::String title;
	juce::String artist;
	juce::File filePath;
	juce::String url;
	juce::File thumbnailFile;
	juce::String thumbnailUrl;
	juce::String duration;
	bool isFavorite = false;

	bool isValid() const { return title.isNotEmpty(); }
};

struct PlaylistData
{
	juce::String name;
	int songCount;
	juce::File thumbnailFile;
	juce::String thumbnailUrl;
	bool isFavorite = false;
};

// Model for displaying songs
class SongListModel : public juce::ListBoxModel
{
public:
	SongListModel(LibraryPage& p, const juce::Array<SongData>& songs);
	int getNumRows() override;
	void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;

	void listBoxItemClicked(int row, const juce::MouseEvent& e) override;

private:
	LibraryPage& ownerPage;
	const juce::Array<SongData>& songList;
	juce::Colour rowTextColor = juce::Colours::white; // for theme

public:
	void setRowTextColor(juce::Colour colour) { rowTextColor = colour; }
};

// Model for displaying playlists
class PlaylistListModel : public juce::ListBoxModel
{
public:
	PlaylistListModel(LibraryPage& p, const juce::Array<PlaylistData>& playlists);
	int getNumRows() override;
	void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;

	void listBoxItemClicked(int row, const juce::MouseEvent& e) override;

private:
	LibraryPage& ownerPage;
	const juce::Array<PlaylistData>& playlistList;
	juce::Colour rowTextColor = juce::Colours::white; // for theme

public:
	void setRowTextColor(juce::Colour colour) { rowTextColor = colour; }
};

class LibraryPage : public juce::Component
{
public:
	class Listener {
		public:
			~Listener() = default;
			virtual void loadSong(juce::String source) = 0;
	};

	using ThemeColors = std::map<std::string, juce::Colour>;
	static const std::map<std::string, ThemeColors> ThemeColorMap;

	LibraryPage(const std::string& themeColor, const std::string& language, const juce::String& songFile, const juce::String& playlistFile);
	~LibraryPage() override = default;

	void paint(juce::Graphics& g) override;
	void resized() override;

	void themeSettingChanged(const juce::String& newThemeName);
	void languageSettingChanged(const juce::String& newLanguageName);

	void toggleFavoriteStatus(int row);

	void setListener(Listener* l) { listen = l; }

	void ClickedSong(juce::String source) { listen->loadSong(source); }

	void loadSongDataFromFile();
	void loadPlaylistDataFromFile();
	void saveSongDataToFile();
	void removeSongfromFavorite();

private:
	Listener* listen;
	std::string currentTheme;
	std::string currentLanguage;

	juce::String songFile;
	juce::String playlistFile;

	juce::Array<SongData> songs;
	juce::Array<PlaylistData> playlists;

	juce::Label songsHeader{ {}, "All Songs" };
	juce::ListBox songListBox;
	std::unique_ptr<SongListModel> songModel;

	juce::Label titleHeader;
	juce::Label artistHeader;
	juce::Label durationHeader;

	juce::Label playlistsHeader{ {}, "Playlists" };
	juce::ListBox playlistListBox;
	std::unique_ptr<PlaylistListModel> playlistModel;

	void paintBackground(juce::Graphics& g, const ThemeColors& themeMap);
};