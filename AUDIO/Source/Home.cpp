#include "Home.h"

//==============================================================================
// STATIC MEMBER INITIALIZATION
//==============================================================================

// Theme lookup
const std::map<std::string, Home::ThemeColors> Home::ThemeColorMap = {
	{"Dark", {
		{"background_top", juce::Colour::fromRGB(0x03, 0x00, 0x42)},
		{"background_bottom", juce::Colour::fromRGB(0x23, 0x23, 0x3d)},
		{"topbar_top", juce::Colour::fromRGB(0x60, 0x5e, 0x5e).withAlpha(0.55f)},
		{"topbar_bottom", juce::Colour::fromRGB(0x30, 0x00, 0x5d)},
		{"leftbar_top", juce::Colour::fromRGB(0x33, 0x05, 0x49).withAlpha(0.55f)},
		{"leftbar_bottom", juce::Colour::fromRGB(0x21, 0x00, 0x32)},
		{"logo_top", juce::Colour::fromRGB(0x4E, 0x09, 0x84).withAlpha(0.55f)},
		{"logo_bottom", juce::Colour::fromRGB(0x08, 0x00, 0x29)},
		{"text_color", juce::Colours::white},
		{"dimmer_color", juce::Colour::fromRGBA(0x00, 0x00, 0x00, 0xA0)},

		{"search_text", juce::Colours::white},

		{"search_bg", juce::Colour::fromRGB(0x1A, 0x1A, 0x1A)},
		{"button_bg_normal", juce::Colour::fromRGB(0x45, 0x45, 0x45)},
		{"button_bg_hover", juce::Colour::fromRGB(0x60, 0x60, 0x60)},

		{"button_bg_active", juce::Colour::fromRGB(0x00, 0x78, 0xD4)}
	}},
	{"Light", {
		{"background_top", juce::Colour::fromRGB(0xF8, 0xF8, 0xF8)},
		{"background_bottom", juce::Colour::fromRGB(0xEE, 0xEE, 0xEE)},
		{"topbar_top", juce::Colour::fromRGB(0xE0, 0xE0, 0xE0).withAlpha(0.7f)},
		{"topbar_bottom", juce::Colour::fromRGB(0xC0, 0xC0, 0xC0)},
		{"leftbar_top", juce::Colour::fromRGB(0xF0, 0xF0, 0xF0).withAlpha(0.7f)},
		{"leftbar_bottom", juce::Colour::fromRGB(0xD8, 0xD8, 0xD8)},
		{"logo_top", juce::Colour::fromRGB(0x00, 0x78, 0xD4).withAlpha(0.7f)},
		{"logo_bottom", juce::Colour::fromRGB(0xE0, 0xE0, 0xE0)},
		{"text_color", juce::Colours::black},
		{"dimmer_color", juce::Colour::fromRGBA(0xFF, 0xFF, 0xFF, 0xC0)},

		{"search_text", juce::Colours::black},
		{"search_bg", juce::Colours::white},

		{"button_bg_normal", juce::Colour::fromRGB(0xF0, 0xF0, 0xF0)},

		{"button_bg_hover", juce::Colour::fromRGB(0xE0, 0xE0, 0xE0)},

		{"button_bg_active", juce::Colour::fromRGB(0x00, 0x78, 0xD4)}
	}}
};

#define AR_SGL juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd8\xb3\xd8\xac\xd9\x84")
#define AR_AD juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd8\xa7\xd8\xb9\xd8\xaf\xd8\xa7\xd8\xaf\xd8\xa7\xd8\xaa")
#define AR_AGANI juce::String::fromUTF8("\xd8\xac\xd9\x85\xd9\x8a\xd8\xb9 \xd8\xa7\xd9\x84\xd8\xa3\xd8\xba\xd8\xa7\xd9\x86\xd9\x8a")
#define AR_BA juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd8\xa8\xd8\xad\xd8\xab")
#define AR_MKTBA juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd9\x85\xd9\x83\xd8\xaa\xd8\xa8\xd8\xa9")
#define AR_MF juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd9\x85\xd9\x81\xd8\xb6\xd9\x84\xd8\xa9")
#define AR_QAEMA juce::String::fromUTF8("\xd9\x82\xd8\xa7\xd8\xa6\xd9\x85\xd8\xaa\xd9\x8a")
#define AR_TD juce::String::fromUTF8("\xd8\xaa\xd8\xb9\xd8\xaf\xd9\x8a\xd9\x84")
#define AR_NORES juce::String::fromUTF8("\xd9\x84\xd8\xa7 \xd8\xaa\xd9\x88\xd8\xac\xd8\xaf \xd9\x86\xd8\xaa\xd8\xa7\xd8\xa6\xd8\xac \xd9\x84\xd9\x84\xd8\xa8\xd8\xad\xd8\xab \xd8\xb9\xd9\x86")
#define AR_CLOSE juce::String::fromUTF8("\xd8\xa7\xd8\xba\xd9\x84\xd8\xa7\xd9\x82")
#define AR_MTHR juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd9\x85\xd8\xb8\xd9\x87\xd8\xb1:")
#define AR_LGHA juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd9\x84\xd8\xba\xd8\xa9")
#define AR_DARK juce::String::fromUTF8("\xd8\xaf\xd8\xa7\xd9\x83\xd9\x86")
#define AR_LIGHT juce::String::fromUTF8("\xd8\xb3\xd8\xa7\xd8\xb7\xd8\xb9")
#define AR_ARABIC juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd8\xb9\xd8\xb1\xd8\xa8\xd9\x8a\xd8\xa9")
#define AR_FAV juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd9\x85\xd9\x81\xd8\xb6\xd9\x84\xd8\xa9")
#define AR_HOME juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd9\x82\xd8\xa7\xd8\xa6\xd9\x85\xd8\xa9 \xd8\xa7\xd9\x84\xd8\xb1\xd8\xa6\xd9\x8a\xd8\xb3\xd9\x8a\xd8\xa9")
#define AR_PLAYLIST juce::String::fromUTF8("\xd9\x82\xd8\xa7\xd8\xa6\xd9\x85\xd8\xa9 \xd8\xa7\xd9\x84\xd8\xaa\xd8\xb4\xd8\xba\xd9\x8a\xd9\x84")
#define AR_MYLIST juce::String::fromUTF8("\xd9\x82\xd8\xa7\xd8\xa6\xd9\x85\xd8\xaa\xd9\x8a")
#define AR_RECENT juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd9\x85\xd9\x81\xd8\xaa\xd9\x88\xd8\xad \xd9\x85\xd8\xa4\xd8\xae\xd8\xb1\xd8\xa7")

const std::map<juce::String, Home::Languages> Home::LanguageMap = {
	{ "English", {
		{AR_SGL,              "Logs"},
		{AR_FAV,			  "Favorite"},
		{AR_HOME,			  "Home"},
		{AR_AD,               "Settings"},
		{AR_AGANI,            "All Songs"},
		{AR_BA,               "Search"},
		{AR_MKTBA,            "Library"},
		{AR_MF,               "Favorites"},
		{AR_QAEMA,            "My List"},
		{AR_TD,               "Edit"},
		{AR_NORES,            "No results found for"},
		{AR_CLOSE,            "Close"},
		{AR_MTHR,             "Theme:"},
		{AR_LGHA,             "Language"},
		{AR_DARK,             "Dark"},
		{AR_LIGHT,            "Light" },
		{AR_ARABIC,           "Arabic"},
		{AR_PLAYLIST,		  "Playlist"},
		{AR_MYLIST,			  "Mylist"},
		{AR_RECENT,			  "Recent"}
	}},
	{ "Arabic", {
		{"Logs",                AR_SGL},
		{"Favorite",			AR_FAV},
		{"Home",				AR_HOME},
		{"Settings",            AR_AD},
		{"All Songs",           AR_AGANI},
		{"Search",              AR_BA},
		{"Library",             AR_MKTBA},
		{"Favorites",           AR_MF},
		{"My List",             AR_QAEMA},
		{"Edit",                AR_TD},
		{"No results found for", AR_NORES},
		{"Close",               AR_CLOSE},
		{"Theme:",              AR_MTHR},
		{"Language",            AR_LGHA},
		{"Dark",                AR_DARK},
		{"Light",               AR_LIGHT},
		{"Arabic",              AR_ARABIC},
		{"Playlist",              AR_PLAYLIST},
		{"Mylist",              AR_MYLIST},
		{"Recent",              AR_RECENT},
	}}
};


//==============================================================================
// RECENTS LIST MODEL IMPLEMENTATION
//==============================================================================

RecentsListModel::RecentsListModel(Home* h) : ownerHome(h)
{

}

void RecentsListModel::setResults(const juce::Array<LastSetion>& newResults)
{
	results = newResults;
	if (resultsListBox)
		resultsListBox->updateContent();
}

void RecentsListModel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
	if (rowNumber < 0 || rowNumber >= getNumRows())
		return;

	const auto& item = results.getUnchecked(rowNumber);

	juce::Colour foreground = rowIsSelected ? juce::Colours::black : rowTextColor;
	juce::Colour background = rowIsSelected ? juce::Colours::cyan.withAlpha(0.7f) : juce::Colours::transparentBlack;

	g.setColour(background);
	g.fillRect(0, 0, width, height);

	int imageSize = height - 8;
	juce::Rectangle<int> imageBounds(4, 4, imageSize, imageSize);

	juce::Rectangle<int> mainTextArea(imageBounds.getRight() + 8, 0, width - (imageBounds.getRight() + 12), height);
	juce::Rectangle<int> durationArea = mainTextArea.removeFromRight((int)(width * 0.15));
	juce::Rectangle<int> titleArtistArea = mainTextArea;

	if (item.thumbnailFile.existsAsFile())
	{
		juce::Image thumbnail = juce::ImageCache::getFromFile(item.thumbnailFile);
		if (thumbnail.isValid())
			g.drawImage(thumbnail, imageBounds.toFloat(), juce::RectanglePlacement::centred);
	}
	else
	{
		g.setColour(rowTextColor.withAlpha(0.2f));
		g.fillRect(imageBounds);
		g.setColour(rowTextColor);
		g.setFont(imageBounds.getHeight() * 0.6f);
		g.drawText(JUCE_T("\xe2\x99\xab"), imageBounds, juce::Justification::centred);
	}

	juce::String rowText = item.title + " - " + item.artist;
	g.setColour(foreground);
	g.setFont(juce::Font(14.0f));
	g.drawText(rowText, titleArtistArea, juce::Justification::centredLeft, true);

	g.setFont(juce::Font(13.0f));
	g.setColour(foreground.withAlpha(0.7f));
	g.drawText(item.duration, durationArea, juce::Justification::centredRight, true);
}

void RecentsListModel::listBoxItemClicked(int row, const juce::MouseEvent&)
{
	if (row < 0 || row >= results.size())
		return;

	const auto& item = results.getUnchecked(row);

	if (item.url.isNotEmpty())
		ownerHome->loadSong(item.url);
	else if (item.filePath.existsAsFile())
		ownerHome->loadSong(item.filePath.getFullPathName());

	ownerHome->getGUIF()->setAll(std::stof(item.lastVolume.toStdString()), std::stof(item.lastPostion.toStdString()), std::stoi(item.isLooping.toStdString()));
}

//==============================================================================
// SearchResultsModel
//==============================================================================

SearchResultsModel::SearchResultsModel(Home* home) : ownerHome(home)
{

}

void SearchResultsModel::setResults(const juce::Array<SongData>& newResults)
{
	results = newResults;
	if (resultsListBox)
		resultsListBox->updateContent();
}

void SearchResultsModel::setListBox(juce::ListBox* lb)
{
	resultsListBox = lb;
}

void SearchResultsModel::setRowTextColor(juce::Colour color)
{
	rowTextColor = color;
}

int SearchResultsModel::getNumRows()
{
	return results.size();
}

void SearchResultsModel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
	if (rowNumber < 0 || rowNumber >= getNumRows())
		return;

	const auto& song = results.getUnchecked(rowNumber);

	juce::Colour foreground = rowIsSelected ? juce::Colours::black : rowTextColor;
	juce::Colour background = rowIsSelected ? juce::Colours::cyan.withAlpha(0.7f) : juce::Colours::transparentBlack;

	g.setColour(background);
	g.fillRect(0, 0, width, height);

	int imageSize = height - 8;
	juce::Rectangle<int> imageBounds(4, 4, imageSize, imageSize);

	juce::Rectangle<int> mainTextArea(imageBounds.getRight() + 8, 0, width - (imageBounds.getRight() + 12), height);
	juce::Rectangle<int> durationArea = mainTextArea.removeFromRight((int)(width * 0.15));
	juce::Rectangle<int> titleArtistArea = mainTextArea;

	// Draw thumbnail
	g.setColour(rowTextColor.withAlpha(0.2f));
	g.fillRect(imageBounds);
	g.setColour(rowTextColor);
	g.setFont(imageBounds.getHeight() * 0.6f);
	g.drawText(juce::String::fromUTF8("\xe2\x99\xab"), imageBounds, juce::Justification::centred);

	// Draw text
	juce::String rowText = song.title + " - " + song.artist;
	g.setColour(foreground);
	g.setFont(juce::Font(14.0f));
	g.drawText(rowText, titleArtistArea, juce::Justification::centredLeft, true);

	g.setFont(juce::Font(13.0f));
	g.setColour(foreground.withAlpha(0.7f));
	g.drawText(song.duration, durationArea, juce::Justification::centredRight, true);
}

void SearchResultsModel::listBoxItemClicked(int row, const juce::MouseEvent&)
{
	if (row < 0 || row >= results.size())
		return;

	if (results[row].url.isNotEmpty())
		ownerHome->loadSong(results[row].url);
	else
		ownerHome->loadSong(results[row].filePath.getFullPathName());
}

//==============================================================================
// SearchResultsComponent
//==============================================================================

SearchResultsComponent::SearchResultsComponent(Home* home) : resultsModel(home)
{
	resultsListBox.setModel(&resultsModel);
	resultsListBox.setRowHeight(40);
	resultsListBox.setOutlineThickness(1);
	resultsModel.setListBox(&resultsListBox);

	statusLabel.setFont(juce::Font(16.0f, juce::Font::bold));
	resultTitleLabel.setFont(juce::Font(14.0f));
	resultTitleLabel.setJustificationType(juce::Justification::centredLeft);

	addAndMakeVisible(statusLabel);
	addAndMakeVisible(resultsListBox);
	addAndMakeVisible(resultTitleLabel);
}

void SearchResultsComponent::resized()
{
	auto bounds = getLocalBounds().reduced(10);
	statusLabel.setBounds(bounds.removeFromTop(20));
	resultTitleLabel.setBounds(bounds.removeFromTop(20));
	resultsListBox.setBounds(bounds);
}

void SearchResultsComponent::displayResultsArray(const juce::Array<SongData>& newResults, const juce::String& searchTerm)
{
	resultsModel.setResults(newResults);

	if (newResults.size() > 0)
	{
		juce::String status = "Found " + juce::String(newResults.size()) + " matches for \"" + searchTerm + "\"";
		statusLabel.setText(status, juce::dontSendNotification);

		resultTitleLabel.setText("Top Result: " + newResults.getFirst().title + " - " + newResults.getFirst().artist,
			juce::dontSendNotification);
	}
	else
	{
		statusLabel.setText("No results found for \"" + searchTerm + "\".", juce::dontSendNotification);
		resultTitleLabel.setText("", juce::dontSendNotification);
	}

	repaint();
}

//==============================================================================
// CONSTRUCTOR / DESTRUCTOR
//==============================================================================

Home::Home(const std::string& themeColor, const std::string& language, PlayerGUI* GUI1, PlayerAudio* Audio1, PlayerGUI* GUI2, PlayerAudio* Audio2)
	: currentTheme(themeColor), currentLanguage(language), GUI1(GUI1), Audio1(Audio1), GUI2(GUI2), Audio2(Audio2), searchResultsList(this)
{
	juce::Font safeFont("Arial Unicode MS", 25.0f, juce::Font::bold);
	// Labels
	for (auto& [msg, textLabel] : labels) {
		textLabel->setText(msg, juce::dontSendNotification);
		textLabel->setFont(safeFont);
		textLabel->setJustificationType(juce::Justification::centred);
		textLabel->setColour(juce::Label::textColourId, juce::Colours::white);
		addAndMakeVisible(textLabel);
	}
	juce::Font safeFont2("Arial Unicode MS", 50.0f, juce::Font::bold);
	// Custom Text
	playList.setFont(safeFont2);
	recent.setFont(safeFont2);
	recent.setJustificationType(juce::Justification::centredLeft);

	// Visibilty of buttons
	addAndMakeVisible(tempSuggest);
	recentsListBox.setModel(&recentsModel);
	recentsListBox.setRowHeight(40);
	recentsModel.setListBox(&recentsListBox);
	addAndMakeVisible(recentsListBox);
	recentsListBox.setColour(juce::ListBox::backgroundColourId, juce::Colours::transparentBlack);
	recentsListBox.setColour(juce::ListBox::outlineColourId, juce::Colours::transparentBlack);

	// Top Bar: Settings, Logs
	juce::Image settings = juce::ImageCache::getFromMemory(BinaryData::settings_png, BinaryData::settings_pngSize);
	settingsButton.setImages(true, true, true, settings, 1.0f, {}, settings, 0.7f, {}, settings, 0.3f, {});
	addAndMakeVisible(settingsButton);
	settingsButton.addListener(this);

	juce::Image logs = juce::ImageCache::getFromMemory(BinaryData::logs_png, BinaryData::logs_pngSize);
	logsButton.setImages(true, true, true, logs, 1.0f, {}, logs, 0.7f, {}, logs, 0.3f, {});
	addAndMakeVisible(logsButton);
	logsButton.addListener(this);

	// Left Bar: Home, Library, ...etc
	juce::Image home = juce::ImageCache::getFromMemory(BinaryData::homen_png, BinaryData::homen_pngSize);
	homeButton.setImages(true, true, true, home, 1.0f, {}, home, 0.7f, {}, home, 0.3f, {});
	addAndMakeVisible(homeButton);
	homeButton.addListener(this);
	homeButton.setAlpha(0.3f);

	juce::Image library = juce::ImageCache::getFromMemory(BinaryData::book_png, BinaryData::book_pngSize);
	libraryButton.setImages(true, true, true, library, 1.0f, {}, library, 0.7f, {}, library, 0.3f, {});
	addAndMakeVisible(libraryButton);
	libraryButton.addListener(this);

	juce::Image heart = juce::ImageCache::getFromMemory(BinaryData::heart_png, BinaryData::heart_pngSize);
	favoriteButton.setImages(true, true, true, heart, 1.0f, {}, heart, 0.7f, {}, heart, 0.3f, {});
	addAndMakeVisible(favoriteButton);
	favoriteButton.addListener(this);

	juce::Image edit = juce::ImageCache::getFromMemory(BinaryData::edit_png, BinaryData::edit_pngSize);
	editButton.setImages(true, true, true, edit, 1.0f, {}, edit, 0.7f, {}, edit, 0.3f, {});
	addAndMakeVisible(editButton);
	editButton.addListener(this);

	// Search Bar Setup
	searchBar.setMultiLine(false);
	searchBar.setTextToShowWhenEmpty("Search", juce::Colours::grey);
	searchBar.setReturnKeyStartsNewLine(false);
	searchBar.addListener(this);
	addAndMakeVisible(searchBar);

	// Overlays and Data
	settingsOverlay = std::make_unique<SettingsPage>(Audio1,"Dark");
	settingsOverlay->addListener(this);
	settingsOverlay->getCloseButton().addListener(this);
	addAndMakeVisible(*settingsOverlay);
	settingsOverlay->setVisible(false);
	isSettingsVisible = false;

	// Search setup
	addAndMakeVisible(searchResultsList);
	searchResultsList.setVisible(false);

	// Other Pages
	libraryPage = std::make_unique<LibraryPage>(currentTheme, currentLanguage, "List.txt", "PlayList.txt");
	addAndMakeVisible(*libraryPage);
	libraryPage->setVisible(false);

	favoritePage = std::make_unique<LibraryPage>(currentTheme, currentLanguage, "FavoritSong.txt", "FavoritPlaylist.txt");
	addAndMakeVisible(*favoritePage);
	favoritePage->setVisible(false);


	addAndMakeVisible(*GUI1);
	GUI1->setVisible(false);
	addAndMakeVisible(*GUI2);
	GUI2->setVisible(false);

	// Listeners
	libraryPage->setListener(this);
	favoritePage->setListener(this);


	// Load Data
	juce::File exeFolder = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();

	juce::File f(exeFolder.getChildFile("List.txt"));
	loadSongsFromFile(f);
	loadRecentsFromFile();
	recentsModel.setResults(recentSongs);
	themeSettingChanged("Dark");

	GUI1->setPlayerListener(this);
	GUI2->setPlayerListener(this);

	setSize(500, 250);
}

//==============================================================================
// CORE JUCE OVERRIDES
//==============================================================================

// Painting to screen
void Home::paint(juce::Graphics& g) {
	const std::string& themeKey = (Home::ThemeColorMap.count(currentTheme) > 0) ? currentTheme : "Dark";
	const auto& themeMap = Home::ThemeColorMap.at(themeKey);

	paintBackgroundGradients(g, themeMap);
	paintModalDimmer(g, themeMap);
}

// Dynamic Resizing
void Home::resized() {
	auto bounds = getLocalBounds();

	if (GUI1 && GUI1->isVisible()) {
		GUI1->setBounds(bounds);
	}

	if (!(GUI1 && GUI1->isVisible()))
	{
		layoutTopBar(bounds);
		layoutLeftBar(bounds);

		layoutPageArea(bounds);
	}

	layoutSettingsOverlay();

	settingsOverlay->toFront(true);

	if (searchResultsList.isVisible() && !(GUI1 && GUI1->isVisible()))
	{
		juce::Rectangle<int> barBounds = searchBar.getBounds();

		const int overlayWidth = barBounds.getWidth();
		const int overlayHeight = juce::jmin(400, (int)(getHeight() * 0.7));

		searchResultsList.setBounds(
			barBounds.getX(),
			barBounds.getBottom() + 2,
			overlayWidth,
			overlayHeight
		);

		searchResultsList.toFront(true);
	}
}

//==============================================================================
// LOGIC METHODS
//==============================================================================


// Button Clicking
void Home::buttonClicked(juce::Button* button) {
	auto resetButtonSelection = [&]() {
		homeButton.setAlpha(1.0f);
		libraryButton.setAlpha(1.0f);
		favoriteButton.setAlpha(1.0f);
		editButton.setAlpha(1.0f);
		};
	
	if (button == &settingsButton) {
		isSettingsVisible = !isSettingsVisible;
		resized();
		repaint();
	}
	else if (button == &settingsOverlay->getCloseButton())
	{
		if (isSettingsVisible)
		{
			isSettingsVisible = !isSettingsVisible;
			resized();
			repaint();
		}
	}
	else if (button == &homeButton)
	{
		if (tempSuggest.isVisible()) {
			resetButtonSelection();
			homeButton.setAlpha(0.3f);
			return;
		}

		if (libraryPage) libraryPage->setVisible(false);

		if (favoritePage) favoritePage->setVisible(false);


		if (GUI1) {
			GUI1->setVisible(false);
			GUI1->reset();
		}

		searchResultsList.setVisible(false);

		tempSuggest.setVisible(true);
		recent.setVisible(true);
		recentsListBox.setVisible(true);

		resetButtonSelection();
		homeButton.setAlpha(0.3f);

		resized();
		repaint();
	}
	else if (button == &libraryButton)
	{
		if (libraryPage && libraryPage->isVisible()) {
			resetButtonSelection();
			libraryButton.setAlpha(0.3f);
			return;
		}
		
		if (libraryPage) 
		{
			libraryPage->setVisible(true);
		}

		if (favoritePage) favoritePage->setVisible(false);

		libraryPage->loadPlaylistDataFromFile();
		libraryPage->loadSongDataFromFile();


		if (GUI1) {
			GUI1->setVisible(false);
			GUI1->reset();
		}

		tempSuggest.setVisible(false);
		recent.setVisible(false);
		recentsListBox.setVisible(false);

		resetButtonSelection();
		libraryButton.setAlpha(0.3f);

		resized();
		repaint();
	}
	else if (button == &favoriteButton)
	{
		if (favoritePage && favoritePage->isVisible()) {
			resetButtonSelection();
			favoriteButton.setAlpha(0.3f);
			return;
		}

		if (libraryPage) libraryPage->setVisible(false);

		if (favoritePage)
		{
			favoritePage->setVisible(true);
		}

		if (GUI1) {
			GUI1->setVisible(false);
			GUI1->reset();
		}

		favoritePage->loadPlaylistDataFromFile();
		favoritePage->loadSongDataFromFile();

		tempSuggest.setVisible(false);
		recent.setVisible(false);
		recentsListBox.setVisible(false);

		resetButtonSelection();
		favoriteButton.setAlpha(0.3f);

		resized();
		repaint();
	}
	else if (button == &editButton) {
		if (libraryPage) libraryPage->setVisible(false);

		if (favoritePage) favoritePage->setVisible(false);

		if (GUI1) {
			GUI1->reset();
			GUI1->setVisible(true);
		}


		tempSuggest.setVisible(false);
		recent.setVisible(false);
		recentsListBox.setVisible(false);

		resized();
		repaint();
	}
}

//==============================================================================
// THEME / PAINT HELPER
//==============================================================================

// Paint Helper
void Home::paintBackgroundGradients(juce::Graphics& g, const std::map<std::string, juce::Colour>& themeMap)
{
	juce::Colour backGround1 = themeMap.at("background_top");
	juce::Colour backGround2 = themeMap.at("background_bottom");
	juce::ColourGradient gradient(backGround1, 0, 0, backGround2, (float)getWidth(), (float)getHeight(), false);
	g.setGradientFill(gradient);
	g.fillRect(getLocalBounds());

	if (topBarArea.getWidth() > 0) {
		juce::Colour topBarColor1 = themeMap.at("topbar_top");
		juce::Colour topBarColor2 = themeMap.at("topbar_bottom");
		juce::ColourGradient gradient2(topBarColor1, topBarArea.getTopLeft().toFloat(), topBarColor2, topBarArea.getBottomRight().toFloat(), false);
		g.setGradientFill(gradient2);
		g.fillRect(topBarArea);
	}

	if (leftBarArea.getHeight() > 0) {
		juce::Colour leftBarColor1 = themeMap.at("leftbar_top");
		juce::Colour leftBarColor2 = themeMap.at("leftbar_bottom");
		juce::ColourGradient gradient2(leftBarColor1, leftBarArea.getTopLeft().toFloat(), leftBarColor2, leftBarArea.getTopRight().toFloat(), false);
		g.setGradientFill(gradient2);
		g.fillRect(leftBarArea);
	}

	if (logoArea.getHeight() > 0) {
		juce::Colour logoColor1 = themeMap.at("logo_top");
		juce::Colour logoColor2 = themeMap.at("logo_bottom");
		juce::ColourGradient gradient2(logoColor1, logoArea.getTopLeft().toFloat(), logoColor2, logoArea.getBottomRight().toFloat(), false);
		g.setGradientFill(gradient2);
		g.fillRect(logoArea);
	}
}
// Paint Helper for settings overlay
void Home::paintModalDimmer(juce::Graphics& g, const std::map<std::string, juce::Colour>& themeMap)
{
	if (isSettingsVisible) {
		juce::Colour settingsColour = themeMap.at("dimmer_color");
		g.setColour(settingsColour);
		g.fillRect(backgroundDimmerArea);
	}
}

// Listeners
void Home::loadSong(juce::String source) {
	GUI1->loadSong(source);

	if (libraryPage) libraryPage->setVisible(false);
	if (favoritePage) favoritePage->setVisible(false);
	if (GUI1) GUI1->setVisible(true);
	searchResultsList.setVisible(false);

	tempSuggest.setVisible(false);
	recent.setVisible(false);
	recentsListBox.setVisible(false);

	resized();
	repaint();
}

void Home::loadPlaylist(juce::String name) {
	GUI1->loadPlaylist(name);

	if (libraryPage) libraryPage->setVisible(false);
	if (favoritePage) favoritePage->setVisible(false);
	if (GUI1) GUI1->setVisible(true);
	searchResultsList.setVisible(false);

	tempSuggest.setVisible(false);
	recent.setVisible(false);
	recentsListBox.setVisible(false);

	resized();
	repaint();
}

// Theme Update
void Home::themeSettingChanged(const juce::String& newThemeName)
{
	currentTheme = newThemeName.toStdString();

	const std::string& themeKey = (Home::ThemeColorMap.count(currentTheme) > 0)
		? currentTheme
		: "Dark";

	const auto& themeMap = Home::ThemeColorMap.at(themeKey);

	juce::Colour newTextColor = themeMap.at("text_color");
	juce::Colour searchTextColor = themeMap.at("search_text");
	juce::Colour searchBgColor = themeMap.at("search_bg");
	juce::Colour buttonNormalColor = themeMap.at("button_bg_normal");
	juce::Colour buttonHoverColor = themeMap.at("button_bg_hover");
	juce::Colour buttonActiveColor = themeMap.at("button_bg_active");

	recentsModel.setRowTextColor(newTextColor);
	recentsListBox.setColour(juce::ListBox::backgroundColourId, searchBgColor);
	recentsListBox.repaint();

	for (auto& [msg, textLabel] : labels) {
		textLabel->setColour(juce::Label::textColourId, newTextColor);
	}
	playList.setColour(juce::Label::textColourId, newTextColor);
	recent.setColour(juce::Label::textColourId, newTextColor);

	searchBar.setColour(juce::TextEditor::textColourId, searchTextColor);
	searchBar.setColour(juce::TextEditor::backgroundColourId, searchBgColor);

	searchResultsList.getListModel().setRowTextColor(newTextColor);
	searchResultsList.getListBox().setColour(juce::ListBox::backgroundColourId, searchBgColor);
	searchResultsList.getStatusLabel().setColour(juce::Label::textColourId, newTextColor);
	searchResultsList.getResultTitleLabel().setColour(juce::Label::textColourId, newTextColor);

	auto updateTextButton = [&](juce::TextButton& button) {
		button.setColour(juce::TextButton::buttonColourId, buttonNormalColor);
		button.setColour(juce::TextButton::textColourOffId, newTextColor);
		button.setColour(juce::TextButton::buttonOnColourId, buttonHoverColor);
		};

	updateTextButton(tempSuggest);

	auto updateImageButton = [&](juce::ImageButton& button) {
		button.setColour(juce::TextButton::buttonColourId, buttonNormalColor);
		button.setColour(juce::TextButton::buttonOnColourId, buttonHoverColor);
		};

	updateImageButton(homeButton);
	updateImageButton(libraryButton);
	updateImageButton(favoriteButton);
	updateImageButton(editButton);
	updateImageButton(settingsButton);
	updateImageButton(logsButton);

	if (libraryPage)
		libraryPage->themeSettingChanged(newThemeName);

	if (favoritePage)
		favoritePage->themeSettingChanged(newThemeName);

	if (GUI1)
		favoritePage->themeSettingChanged(newThemeName);

	if (GUI2)
		favoritePage->themeSettingChanged(newThemeName);


	repaint();
	resized();
}

void Home::languageSettingChanged(const juce::String& newLanguageName)
{
	currentLanguage = newLanguageName.toStdString();

	const std::string& langKey = (Home::LanguageMap.count(currentLanguage) > 0)
		? currentLanguage
		: "English";

	const auto& targetLangMap = Home::LanguageMap.at(langKey);


	auto getTranslatedText = [&](const juce::String& sourceText) -> juce::String {

		if (targetLangMap.count(sourceText.toStdString()))
			return targetLangMap.at(sourceText.toStdString());

		const auto& englishSourceMap = Home::LanguageMap.at("English");
		if (englishSourceMap.count(sourceText.toStdString()))
		{
			return englishSourceMap.at(sourceText.toStdString());
		}

		if (sourceText == "Search" && langKey == "Arabic")
			return AR_BA;

		return sourceText;
		};


	for (auto& [englishKey, textLabel] : labels)
	{
		if (textLabel)
			textLabel->setText(getTranslatedText(englishKey), juce::dontSendNotification);
	}

	searchBar.setTextToShowWhenEmpty(getTranslatedText("Search"), juce::Colours::grey);


	if (libraryPage)
		libraryPage->languageSettingChanged(newLanguageName);

	if (favoritePage)
		favoritePage->languageSettingChanged(newLanguageName);

	if (GUI1)
		favoritePage->languageSettingChanged(newLanguageName);

	if (GUI2)
		favoritePage->languageSettingChanged(newLanguageName);


	repaint();
	resized();
}

//==============================================================================
// LAYOUT METHODS
//==============================================================================

// Top Bar
void Home::layoutTopBar(juce::Rectangle<int>& bounds)
{
	int tempWidth = (int)(getWidth() * 0.1);
	auto topRectangle = bounds.removeFromTop((int)(getHeight() * 0.13));
	logoArea = topRectangle.removeFromLeft(tempWidth);
	topBarArea = topRectangle;

	auto righArea = topRectangle.removeFromRight((int)(topRectangle.getWidth() * 0.2)).reduced(5);
	auto buttonWidth1 = righArea.getWidth() / 2;

	auto settingsArea = righArea.removeFromRight(buttonWidth1);
	settingsButton.setBounds(settingsArea.removeFromTop((int)(settingsArea.getHeight() * 0.7)));
	settingsText.setBounds(settingsArea);

	auto logsArea = righArea.removeFromRight(buttonWidth1);
	logsButton.setBounds(logsArea.removeFromTop((int)(logsArea.getHeight() * 0.7)));
	logsText.setBounds(logsArea);

	searchBar.setBounds(topRectangle.reduced(300, (int)(topRectangle.getHeight() * 0.35)));
}

// Left Bar
void Home::layoutLeftBar(juce::Rectangle<int>& bounds)
{
	int tempWidth = (int)(getWidth() * 0.1);
	auto leftRectangle = bounds.removeFromLeft(tempWidth);
	leftBarArea = leftRectangle;
	auto buttonColumn = leftBarArea;
	auto buttonHeight2 = buttonColumn.getHeight() / 6;

	auto homeArea = buttonColumn.removeFromTop(buttonHeight2);
	homeButton.setBounds(homeArea.removeFromLeft((int)(tempWidth * 0.6)).reduced(10));
	homeText.setBounds(homeArea);

	auto libraryArea = buttonColumn.removeFromTop(buttonHeight2);
	libraryButton.setBounds(libraryArea.removeFromLeft((int)(tempWidth * 0.6)).reduced(10));
	libraryText.setBounds(libraryArea);

	playList.setBounds(buttonColumn.removeFromTop(buttonHeight2));

	auto favoritArea = buttonColumn.removeFromTop(buttonHeight2);
	favoriteButton.setBounds(favoritArea.removeFromLeft((int)(tempWidth * 0.6)).reduced(10));
	favoriteText.setBounds(favoritArea);

	auto editArea = buttonColumn.removeFromTop(buttonHeight2);
	editButton.setBounds(editArea.removeFromLeft((int)(tempWidth * 0.6)).reduced(10));
	editText.setBounds(editArea);
}

// Page Content
void Home::layoutPageArea(juce::Rectangle<int>& bounds)
{
	if (libraryPage && libraryPage->isVisible())
	{
		libraryPage->setBounds(bounds);
		return;
	}

	if (favoritePage && favoritePage->isVisible())
	{
		favoritePage->setBounds(bounds);
		return;
	}

	auto suggest = bounds.removeFromTop((int)(bounds.getHeight() * 0.5));
	tempSuggest.setBounds(suggest);

	recent.setBounds(bounds.removeFromTop((int)(bounds.getHeight() * 0.2)));

	recentsListBox.setBounds(bounds);
}

// Settings Layer
void Home::layoutSettingsOverlay()
{
	backgroundDimmerArea = getLocalBounds();

	if (isSettingsVisible) {
		int overlayWidth = (int)(getWidth() * 0.6);
		int overlayHeight = (int)(getHeight() * 0.7);

		settingsOverlay->setBounds(
			(getWidth() - overlayWidth) / 2,
			(getHeight() - overlayHeight) / 2,
			overlayWidth,
			overlayHeight
		);
		settingsOverlay->setVisible(true);
	}
	else {
		settingsOverlay->setBounds(0, 0, 0, 0);
		settingsOverlay->setVisible(false);
	}
}

//==============================================================================
// SEARCH / DATA LOADING
//==============================================================================

// When the text change
void Home::textEditorTextChanged(juce::TextEditor& editor)
{
	if (&editor == &searchBar)
	{
		juce::String searchTerm = searchBar.getText().trim();
		bool searching = searchTerm.isNotEmpty();

		searchResultsList.setVisible(searching);

		if (searching)
		{
			juce::Array<SongData> results = findSongsByPartialTitle(searchTerm);
			searchResultsList.displayResultsArray(results, searchTerm);
		}

		resized();
		repaint();

		if (searching)
		{
			searchBar.grabKeyboardFocus();
		}
	}
}

// Loading from file
void Home::loadSongsFromFile(const juce::File& libraryFile)
{
	songMap.clear();

	if (libraryFile.existsAsFile())
	{
		juce::String fileContent = libraryFile.loadFileAsString();
		juce::String currentLine;

		while (fileContent.isNotEmpty())
		{
			int newlinePos = fileContent.indexOfChar('\n');

			if (newlinePos < 0) {
				currentLine = fileContent.trim();
				fileContent = {};
			}
			else {
				currentLine = fileContent.substring(0, newlinePos).trim();
				fileContent = fileContent.substring(newlinePos + 1);
			}

			if (currentLine.trim().isEmpty()) continue;

			juce::StringArray parts;
			parts.addTokens(currentLine, "|", "");


			if (parts.size() >= 3)
			{
				juce::String title = parts[0].trim();
				juce::String artist = parts[1].trim();
				juce::String audioSource = parts[2].trim();

				if (title.isNotEmpty())
				{
					SongData newSong;
					newSong.title = title;
					newSong.artist = artist;

					if (audioSource.startsWithIgnoreCase("http://") || audioSource.startsWithIgnoreCase("https://"))
					{
						newSong.url = audioSource;
					}
					else
					{
						newSong.filePath = juce::File(audioSource);
					}

					if (parts.size() >= 4)
					{
						juce::String thumbSource = parts[3].trim();
						if (thumbSource.startsWithIgnoreCase("http://") || thumbSource.startsWithIgnoreCase("https://"))
						{
							newSong.thumbnailUrl = thumbSource;
						}
						else
						{
							newSong.thumbnailFile = juce::File(thumbSource);
						}
					}

					if (parts.size() >= 5)
					{
						newSong.duration = parts[4].trim();
					}

					if (parts.size() >= 6)
					{
						newSong.isFavorite = (parts[5].trim().compareIgnoreCase("true") == 0);
					}

					songMap.insert({ newSong.title.toStdString(), newSong });
				}
			}
		}
	}
}

// Getting Results
juce::Array<SongData> Home::findSongsByPartialTitle(const juce::String& targetTerm)
{
	juce::Array<SongData> results;
	juce::String lowerSearchTerm = targetTerm.toLowerCase();

	for (auto const& [key, song] : songMap)
	{
		juce::String searchableText = song.title.toLowerCase() + " " + song.artist.toLowerCase();

		if (searchableText.contains(lowerSearchTerm))
		{
			results.add(song);
		}
	}
	return results;
}

void Home::loadRecentsFromFile()
{
	juce::File exeFolder = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();

	juce::File recentsFile(exeFolder.getChildFile("recent.txt"));
	recentSongs.clear();

	if (recentsFile.existsAsFile())
	{
		juce::String fileContent = recentsFile.loadFileAsString();
		juce::StringArray lines;
		lines.addTokens(fileContent, "\n", "");

		for (const auto& line : lines)
		{
			juce::String currentLine = line.trim();
			if (currentLine.isEmpty()) continue;

			juce::StringArray parts;
			parts.addTokens(currentLine, "|", "");

			if (parts.size() >= 11)
			{
				LastSetion item;
				item.title = parts[0];
				item.artist = parts[1];
				item.filePath = juce::File(parts[2]);
				item.url = parts[3];
				item.thumbnailFile = juce::File(parts[4]);
				item.thumbnailUrl = parts[5];
				item.duration = parts[6];
				item.lastVolume = parts[7];
				item.lastPostion = parts[8];
				item.isLooping = parts[9];
				item.activePlaylistName = parts[10];

				recentSongs.add(item);
			}
		}
	}
}

#undef AR_SGL
#undef AR_AD
#undef AR_AGANI
#undef AR_BA
#undef AR_MKTBA
#undef AR_MF
#undef AR_QAEMA
#undef AR_TD
#undef AR_NORES
#undef AR_CLOSE
#undef AR_MTHR
#undef AR_LGHA
#undef AR_DARK
#undef AR_LIGHT
#undef AR_ARABIC