#pragma once

// JUCE headers
#include <JuceHeader.h>

// C++ headers
#include <vector>
#include <map>
#include <unordered_map>
#include <optional>
#include <string>

// Settings Header
#include "SettingsPage.h" 
#include "LibraryPage.h"
#include "PlayerGUI.h"


class Home;

struct LastSetion {
	juce::String lastVolume;
	juce::String lastPostion;
	juce::String isLooping;
	juce::String activePlaylistName;
	juce::String title;
	juce::String artist;
	juce::File filePath;
	juce::String url;
	juce::File thumbnailFile;
	juce::String thumbnailUrl;
	juce::String duration;
	std::vector<Marker> marker;
};
//==============================================================================
// Required Data Structures
//==============================================================================

class RecentsListModel : public juce::ListBoxModel
{
public:
	RecentsListModel(Home* h);

	void setResults(const juce::Array<LastSetion>& newResults);

	void setListBox(juce::ListBox* lb) { resultsListBox = lb; }
	void setRowTextColor(juce::Colour color) { rowTextColor = color; }

	int getNumRows() override { return results.size(); }

	void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;

	void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override;

private:
	Home* ownerHome;
	juce::Array<LastSetion> results;
	juce::ListBox* resultsListBox = nullptr;
	juce::Colour rowTextColor = juce::Colours::white;
};

//==============================================================================
// Search Results Component
//==============================================================================
class SearchResultsModel : public juce::ListBoxModel
{
public:
	SearchResultsModel(Home* home);

	void setResults(const juce::Array<SongData>& newResults);
	void setListBox(juce::ListBox* lb);
	void setRowTextColor(juce::Colour color);

	int getNumRows() override;
	void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
	void listBoxItemDoubleClicked(int row, const juce::MouseEvent& e) override;

private:
	Home* ownerHome;
	juce::Array<SongData> results;
	juce::ListBox* resultsListBox = nullptr;
	juce::Colour rowTextColor = juce::Colours::white;
};

//==============================================================================
// Search Results View
//==============================================================================
class SearchResultsComponent : public juce::Component
{
public:
	SearchResultsComponent(Home* home);

	void resized() override;

	void displayResultsArray(const juce::Array<SongData>& newResults, const juce::String& searchTerm);

	// Accessors
	juce::ListBox& getListBox() { return resultsListBox; }
	SearchResultsModel& getListModel() { return resultsModel; }
	juce::Label& getStatusLabel() { return statusLabel; }
	juce::Label& getResultTitleLabel() { return resultTitleLabel; }

private:
	juce::Label statusLabel;
	juce::Label resultTitleLabel;

	juce::ListBox resultsListBox;
	SearchResultsModel resultsModel;
};

//==============================================================================
// Main Application
//==============================================================================

// Use std::unordered_map for O(1) key lookup (Assuming there is no collision, It will be updated to double hash later)
using SongLibraryMap = std::unordered_map<std::string, SongData>;

// Main Class with required Inheritance
class Home : public juce::Component,
	public juce::Button::Listener,
	public SettingsPage::Listener,
	public juce::TextEditor::Listener,
	public LibraryPage::Listener,
	public PlayerGUI::Listener
{
private:
	// Theme Colors shortcut
	using ThemeColors = std::map<std::string, juce::Colour>;
	using Languages = std::map<juce::String, juce::String>;
	// static map to live across the application (later will use .json file or .xml)
	static const std::map<std::string, ThemeColors> ThemeColorMap;
	static const std::map<juce::String, Languages> LanguageMap;

	// Home Page Layer
	juce::ImageButton tempSuggest{ "Suggest" };
	juce::Label welcomeLabel;

	RecentsListModel recentsModel{ this };
	juce::ListBox recentsListBox;
	juce::Array<LastSetion> recentSongs;

	// Labels
	juce::Label playList{ "Playlist" };
	juce::Label recent{ "Recent" };

	// Top Bar
	juce::TextEditor searchBar;
	juce::ImageButton logsButton;
	juce::ImageButton settingsButton;
	juce::Image logoImage;

	// Labels
	juce::Label settingsText;
	juce::Label logsText;

	// Left Bar
	juce::ImageButton homeButton;
	juce::ImageButton libraryButton;
	juce::ImageButton favoriteButton;
	juce::ImageButton editButton;

	// Labels
	juce::Label homeText;
	juce::Label searchText;
	juce::Label libraryText;
	juce::Label favoriteText;
	juce::Label editText;

	// For easier Making Visible and changes
	std::vector<std::pair<juce::String, juce::Label*>> labels = {
		{"Home", &homeText},
		{"Search", &searchText},
		{"Library", &libraryText},
		{"Favorite", &favoriteText},
		{"Edit", &editText},
		{"Settings", &settingsText},
		{"Logs", &logsText},
		{"Playlist", &playList},
		{"Recent", &recent}
	};

	// Setting state
	std::unique_ptr<SettingsPage> settingsOverlay;
	bool isSettingsVisible = false;
	juce::Rectangle<int> backgroundDimmerArea;

	// Theme prefrences
	std::string currentTheme;
	std::string currentLanguage;

	// Search Data
	SongLibraryMap songMap;
	SearchResultsComponent searchResultsList;

	// Layout
	juce::Rectangle<int> topBarArea;
	juce::Rectangle<int> logoArea;
	juce::Rectangle<int> leftBarArea;

	// Library Page
	std::unique_ptr<LibraryPage> libraryPage;
	std::unique_ptr<LibraryPage> favoritePage;
	PlayerGUI* GUI1;
	PlayerAudio* Audio1;
	PlayerGUI* GUI2;
	PlayerAudio* Audio2;

	// HELPER METHODS
	// Layers
	void layoutTopBar(juce::Rectangle<int>& bounds);
	void layoutLeftBar(juce::Rectangle<int>& bounds);
	void layoutPageArea(juce::Rectangle<int>& bounds);
	void layoutSettingsOverlay();

	// Painting
	void paintBackgroundGradients(juce::Graphics& g, const std::map<std::string, juce::Colour>& themeMap);
	void paintModalDimmer(juce::Graphics& g, const std::map<std::string, juce::Colour>& themeMap);

	void returnToHomePage() override {
		loadRecentsFromFile();
		recentsModel.setResults(recentSongs);
		GUI1->reset();
		GUI1->setVisible(false);
		GUI2->reset();
		GUI2->setVisible(false);
		resized();
		repaint();
		buttonClicked(&homeButton);
	}

	void twoGUI() override {
		if (GUI2->isVisible()) {
			GUI2->reset();
			GUI2->setVisible(false);
			GUI1->setBounds(getLocalBounds());
		}
		else {
			GUI2->setVisible(true);
			auto bounds = getLocalBounds();
			GUI1->setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
			GUI2->setBounds(bounds);
		}
	}

	void openSettings() override{
		isSettingsVisible = !isSettingsVisible;
		resized();
		repaint();
	}


public:
	void loadSong(juce::String source) override;
	void loadPlaylist(juce::String name) override;
	// Constructor
	Home(const std::string& themeColor, const std::string& language, PlayerGUI* GUI1, PlayerAudio* Audio1, PlayerGUI* GUI2, PlayerAudio* Audio2);
	~Home() override = default;

	// UI visuals
	void paint(juce::Graphics& g) override;
	void resized() override;

	// Logic Methods
	void buttonClicked(juce::Button*) override;
	void themeSettingChanged(const juce::String& newThemeName) override;
	void languageSettingChanged(const juce::String& newLanguageName) override;
	void textEditorTextChanged(juce::TextEditor& editor) override;

	void loadSongsFromFile(const juce::File& libraryFile);
	juce::Array<SongData> findSongsByPartialTitle(const juce::String& targetTerm);
	void loadRecentsFromFile();
	PlayerGUI* getGUIF() { return GUI1; }
};