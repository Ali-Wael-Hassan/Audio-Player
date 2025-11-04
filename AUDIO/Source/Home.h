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

//==============================================================================
// Required Data Structures
//==============================================================================

// Required for the ListBox to function correctly
class SearchResultsModel : public juce::ListBoxModel
{
public:
	SearchResultsModel() = default;

	// Gets new results then update the ListBox
	void setResults(const juce::Array<SongData>& newResults)
	{
		results = newResults;
		if (resultsListBox)
			resultsListBox->updateContent();
	}

	// Allowing communication with other classes(Objects)
	void setListBox(juce::ListBox* lb) { resultsListBox = lb; }

	int getNumRows() override
	{
		return results.size();
	}

	void setRowTextColor(juce::Colour color) {
		rowTextColor = color;
	}

	// Painting the select boxes elemnts (Box1, Box2, ...)
	void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override
	{
		if (rowNumber < 0 || rowNumber >= getNumRows()) // Safer check
			return;

		const auto& song = results.getUnchecked(rowNumber);

		juce::Colour foreground = rowIsSelected ? juce::Colours::black : rowTextColor;
		juce::Colour background = rowIsSelected ? juce::Colours::cyan.withAlpha(0.7f) : juce::Colours::transparentBlack;

		g.setColour(background);
		g.fillRect(0, 0, width, height);

		int imageSize = height - 8;
		juce::Rectangle<int> imageBounds(4, 4, imageSize, imageSize);

		// Area for all text
		juce::Rectangle<int> mainTextArea(imageBounds.getRight() + 8, 0, width - (imageBounds.getRight() + 12), height);

		// Split text area for duration
		juce::Rectangle<int> durationArea = mainTextArea.removeFromRight((int)(width * 0.15)); // 15% for duration
		juce::Rectangle<int> titleArtistArea = mainTextArea; // The rest for title/artist


		// Draw Thumbnail
		if (song.thumbnailFile.existsAsFile())
		{
			juce::Image thumbnail = juce::ImageCache::getFromFile(song.thumbnailFile);
			if (thumbnail.isValid())
			{
				g.drawImage(thumbnail, imageBounds.toFloat(), juce::RectanglePlacement::centred);
			}
		}
		else
		{
			// Placeholder
			g.setColour(rowTextColor.withAlpha(0.2f));
			g.fillRect(imageBounds);
			g.setColour(rowTextColor);
			g.setFont(imageBounds.getHeight() * 0.6f);
			g.drawText(JUCE_T("\xe2\x99\xab"), imageBounds, juce::Justification::centred);
		}

		// Draw Title & Artist
		juce::String rowText = song.title + " - " + song.artist;
		g.setColour(foreground);
		g.setFont(juce::Font(14.0f));
		g.drawText(rowText, titleArtistArea, juce::Justification::centredLeft, true);

		// Draw Duration
		g.setFont(juce::Font(13.0f));
		g.setColour(foreground.withAlpha(0.7f));
		g.drawText(song.duration, durationArea, juce::Justification::centredRight, true);
	}

private:
	// Results array for search
	juce::Array<SongData> results;
	// UI ListBox for visualizing the data
	juce::ListBox* resultsListBox = nullptr;
	// for theme
	juce::Colour rowTextColor = juce::Colours::white;
};


//==============================================================================
// Search Results View
//==============================================================================

class SearchResultsComponent : public juce::Component
{
public:
	// Setting the Model of the listBox(Previous), Border and Listener for Model
	SearchResultsComponent()
	{
		resultsListBox.setModel(&resultsModel);
		resultsListBox.setRowHeight(40);
		resultsListBox.setOutlineThickness(1);
		resultsModel.setListBox(&resultsListBox);

		// Make status labels visible
		statusLabel.setFont(juce::Font(16.0f, juce::Font::bold));
		resultTitleLabel.setFont(juce::Font(14.0f));
		resultTitleLabel.setJustificationType(juce::Justification::centredLeft);

		addAndMakeVisible(statusLabel);
		addAndMakeVisible(resultsListBox);
		addAndMakeVisible(resultTitleLabel);
	}

	// Dynamic resizing to get in multiple windows size
	void resized() override
	{
		auto bounds = getLocalBounds().reduced(10);
		// Headers
		statusLabel.setBounds(bounds.removeFromTop(20));
		resultTitleLabel.setBounds(bounds.removeFromTop(20));
		// Results details
		resultsListBox.setBounds(bounds);
	}

	// Displaying the BoxList(Array)
	void displayResultsArray(const juce::Array<SongData>& newResults, const juce::String& searchTerm)
	{
		// Update results
		resultsModel.setResults(newResults);

		// Checking if there is something in our data with this name
		if (newResults.size() > 0)
		{
			// Format: Found <Size> matches for "<Search Term>"
			juce::String status =
				"Found " + juce::String(newResults.size()) + " matches for \"" + searchTerm + "\"";

			// Updating Label
			statusLabel.setText(status, juce::dontSendNotification);

			// Show the top result details below the status
			resultTitleLabel.setText("Top Result: " + newResults.getFirst().title + " - " + newResults.getFirst().artist, juce::dontSendNotification);
		}
		else
		{
			// If nothing Found
			statusLabel.setText("No results found for \"" + searchTerm + "\".", juce::dontSendNotification);
			resultTitleLabel.setText("", juce::dontSendNotification);
		}
		// Refresh
		repaint();
	}

	// To apply Theme
	juce::ListBox& getListBox() { return resultsListBox; }
	SearchResultsModel& getListModel() { return resultsModel; }
	juce::Label& getStatusLabel() { return statusLabel; }
	juce::Label& getResultTitleLabel() { return resultTitleLabel; }


private:
	// TopBar
	juce::Label statusLabel;
	juce::Label resultTitleLabel;

	// ListBox that shows details
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
	public LibraryPage::Listener
{
private:
	// Theme Colors shortcut
	using ThemeColors = std::map<std::string, juce::Colour>;
	// static map to live across the application (later will use .json file or .xml)
	static const std::map<std::string, ThemeColors> ThemeColorMap;

	// Home Page Layer
	juce::TextButton tempSuggest{ "Suggest" };
	juce::TextButton tempAudio1{ "Audio" };
	juce::TextButton tempAudio2{ "Audio" };
	juce::TextButton tempAudio3{ "Audio" };
	juce::TextButton tempAudio4{ "Audio" };
	juce::TextButton tempAudio5{ "Audio" };

	// Labels
	juce::Label playList{ "Playlist" };
	juce::Label recent{ "Recent" };

	// Top Bar
	juce::TextEditor searchBar;
	juce::ImageButton logsButton;
	juce::ImageButton settingsButton;

	// Labels
	juce::Label settingsText;
	juce::Label logsText;

	// Left Bar
	juce::ImageButton homeButton;
	juce::ImageButton libraryButton;
	juce::ImageButton favoriteButton;
	juce::ImageButton listButton;
	juce::ImageButton editButton;

	// Labels
	juce::Label homeText;
	juce::Label searchText;
	juce::Label libraryText;
	juce::Label favoriteText;
	juce::Label listText;
	juce::Label editText;

	// For easier Making Visible and changes
	std::vector<std::pair<juce::String, juce::Label*>> labels = {
		{"Home", &homeText}, {"Search", &searchText}, {"Library", &libraryText},
		{"Favorite", &favoriteText}, {"Mylist", &listText}, {"Edit", &editText},
		{"Settings", &settingsText}, {"Logs", &logsText}, {"Playlist", &playList}, {"Recent", &recent}
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
	std::unique_ptr<LibraryPage> listPage;
	PlayerGUI* GUI;
	PlayerAudio* Audio;

	// HELPER METHODS
	// Layers
	void layoutTopBar(juce::Rectangle<int>& bounds);
	void layoutLeftBar(juce::Rectangle<int>& bounds);
	void layoutPageArea(juce::Rectangle<int>& bounds);
	void layoutSettingsOverlay();

	// Painting
	void paintBackgroundGradients(juce::Graphics& g, const std::map<std::string, juce::Colour>& themeMap);
	void paintModalDimmer(juce::Graphics& g, const std::map<std::string, juce::Colour>& themeMap);

	void loadSong(juce::String source) override;

public:
	// Constructor
	Home(const std::string& themeColor, const std::string& language, PlayerGUI* GUI, PlayerAudio* Audio);
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
};