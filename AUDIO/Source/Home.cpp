#include "Home.h"
// Assuming "LibraryPage.h" is included in "Home.h"

//==============================================================================
// STATIC MEMBER INITIALIZATION
//==============================================================================

// Theme lookup
const std::map<std::string, Home::ThemeColors> Home::ThemeColorMap = {
	{"Dark", {
		// BACKGROUNDS & BARS
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

		// Text Color
		{"search_text", juce::Colours::white},

		// Search Bar Background
		{"search_bg", juce::Colour::fromRGB(0x1A, 0x1A, 0x1A)},

		// Button Backgrounds (Normal State)
		{"button_bg_normal", juce::Colour::fromRGB(0x45, 0x45, 0x45)},

		// Hover
		{"button_bg_hover", juce::Colour::fromRGB(0x60, 0x60, 0x60)},

		// Active State
		{"button_bg_active", juce::Colour::fromRGB(0x00, 0x78, 0xD4)}
	}},
	{"Light", {
		// BACKGROUNDS & BARS
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

		// Text Color
		{"search_text", juce::Colours::black},

		// Search Bar Background
		{"search_bg", juce::Colours::white},

		// Button (Normal State)
		{"button_bg_normal", juce::Colour::fromRGB(0xF0, 0xF0, 0xF0)},

		// Hover State
		{"button_bg_hover", juce::Colour::fromRGB(0xE0, 0xE0, 0xE0)},

		// Active State
		{"button_bg_active", juce::Colour::fromRGB(0x00, 0x78, 0xD4)}
	}}
};

//==============================================================================
// CONSTRUCTOR / DESTRUCTOR
//==============================================================================

Home::Home(const std::string& themeColor, const std::string& language, PlayerGUI* GUI, PlayerAudio* Audio)
	: currentTheme(themeColor), currentLanguage(language), GUI(GUI), Audio(Audio)
{
	// Labels
	for (auto& [msg, textLabel] : labels) {
		textLabel->setText(msg, juce::dontSendNotification);
		textLabel->setFont(juce::Font(25.0f, juce::Font::bold));
		textLabel->setJustificationType(juce::Justification::centred);
		textLabel->setColour(juce::Label::textColourId, juce::Colours::white);
		addAndMakeVisible(textLabel);
	}
	// Custom Text
	playList.setFont(juce::Font(50.0f, juce::Font::bold));
	recent.setFont(juce::Font(50.0f, juce::Font::bold));
	recent.setJustificationType(juce::Justification::centredLeft);

	// Visibilty of buttons
	addAndMakeVisible(tempSuggest);
	tempSuggest.addListener(this);
	addAndMakeVisible(tempAudio1);
	tempAudio1.addListener(this);
	addAndMakeVisible(tempAudio2);
	tempAudio2.addListener(this);
	addAndMakeVisible(tempAudio3);
	tempAudio3.addListener(this);
	addAndMakeVisible(tempAudio4);
	tempAudio4.addListener(this);
	addAndMakeVisible(tempAudio5);
	tempAudio5.addListener(this);

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
	homeButton.setImages(true, true, true, home, 0.3f, {}, home, 0.3f, {}, home, 0.3f, {});
	addAndMakeVisible(homeButton);
	homeButton.addListener(this);

	juce::Image library = juce::ImageCache::getFromMemory(BinaryData::book_png, BinaryData::book_pngSize);
	libraryButton.setImages(true, true, true, library, 1.0f, {}, library, 0.7f, {}, library, 0.3f, {});
	addAndMakeVisible(libraryButton);
	libraryButton.addListener(this);

	juce::Image heart = juce::ImageCache::getFromMemory(BinaryData::heart_png, BinaryData::heart_pngSize);
	favoriteButton.setImages(true, true, true, heart, 1.0f, {}, heart, 0.7f, {}, heart, 0.3f, {});
	addAndMakeVisible(favoriteButton);
	favoriteButton.addListener(this);

	juce::Image list = juce::ImageCache::getFromMemory(BinaryData::list_png, BinaryData::list_pngSize);
	listButton.setImages(true, true, true, list, 1.0f, {}, list, 0.7f, {}, list, 0.3f, {});
	addAndMakeVisible(listButton);
	listButton.addListener(this);

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
	settingsOverlay = std::make_unique<SettingsPage>("Dark");
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

	listPage = std::make_unique<LibraryPage>(currentTheme, currentLanguage, "UserSongs.txt", "UserPlaylist.txt");
	addAndMakeVisible(*listPage);
	listPage->setVisible(false);


	addAndMakeVisible(*GUI);
	GUI->setVisible(false);


	// Load Data
	juce::File f("List.txt");
	loadSongsFromFile(f);

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

	if (GUI && GUI->isVisible()) {
		GUI->setBounds(bounds);
		return;
	}

	layoutTopBar(bounds);
	layoutLeftBar(bounds);

	layoutPageArea(bounds);

	layoutSettingsOverlay();

	settingsOverlay->toFront(true);

	if (searchResultsList.isVisible())
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
		if (libraryPage) libraryPage->setVisible(false);

		if (favoritePage) favoritePage->setVisible(false);

		if (listPage) listPage->setVisible(false);

		if (GUI) GUI->setVisible(false);

		searchResultsList.setVisible(false);

		tempSuggest.setVisible(true);
		recent.setVisible(true);
		tempAudio1.setVisible(true);
		tempAudio2.setVisible(true);
		tempAudio3.setVisible(true);
		tempAudio4.setVisible(true);
		tempAudio5.setVisible(true);

		resized();
		repaint();
	}
	else if (button == &libraryButton)
	{
		if (libraryPage) 
		{
			libraryPage->setVisible(true);
			libraryPage->loadSongDataFromFile();
			libraryPage->loadPlaylistDataFromFile();
		}

		if (favoritePage) favoritePage->setVisible(false);

		if (listPage) listPage->setVisible(false);

		if (GUI) GUI->setVisible(false);

		tempSuggest.setVisible(false);
		recent.setVisible(false);
		tempAudio1.setVisible(false);
		tempAudio2.setVisible(false);
		tempAudio3.setVisible(false);
		tempAudio4.setVisible(false);
		tempAudio5.setVisible(false);

		resized();
		repaint();
	}
	else if (button == &favoriteButton)
	{
		if (libraryPage) libraryPage->setVisible(false);

		if (favoritePage)
		{
			favoritePage->setVisible(true);
			favoritePage->loadSongDataFromFile();
			favoritePage->loadPlaylistDataFromFile();
		}

		if (listPage) listPage->setVisible(false);

		if (GUI) GUI->setVisible(false);

		tempSuggest.setVisible(false);
		recent.setVisible(false);
		tempAudio1.setVisible(false);
		tempAudio2.setVisible(false);
		tempAudio3.setVisible(false);
		tempAudio4.setVisible(false);
		tempAudio5.setVisible(false);

		resized();
		repaint();
	}
	else if (button == &listButton)
	{
		if (libraryPage) libraryPage->setVisible(false);

		if (favoritePage) favoritePage->setVisible(false);

		if (GUI) GUI->setVisible(false);

		if (listPage)
		{
			listPage->setVisible(true);
			listPage->loadSongDataFromFile();
			listPage->loadPlaylistDataFromFile();
		}

		tempSuggest.setVisible(false);
		recent.setVisible(false);
		tempAudio1.setVisible(false);
		tempAudio2.setVisible(false);
		tempAudio3.setVisible(false);
		tempAudio4.setVisible(false);
		tempAudio5.setVisible(false);

		resized();
		repaint();
	}
	else if (button == &editButton) {
		if (libraryPage) libraryPage->setVisible(false);

		if (favoritePage) favoritePage->setVisible(false);

		if (GUI)
		{
			GUI->setVisible(true);
		}

		if (listPage) listPage->setVisible(false);

		tempSuggest.setVisible(false);
		recent.setVisible(false);
		tempAudio1.setVisible(false);
		tempAudio2.setVisible(false);
		tempAudio3.setVisible(false);
		tempAudio4.setVisible(false);
		tempAudio5.setVisible(false);

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
	updateTextButton(tempAudio1);
	updateTextButton(tempAudio2);
	updateTextButton(tempAudio3);
	updateTextButton(tempAudio4);
	updateTextButton(tempAudio5);

	auto updateImageButton = [&](juce::ImageButton& button) {
		button.setColour(juce::TextButton::buttonColourId, buttonNormalColor);
		button.setColour(juce::TextButton::buttonOnColourId, buttonHoverColor);
		};

	updateImageButton(homeButton);
	updateImageButton(libraryButton);
	updateImageButton(favoriteButton);
	updateImageButton(listButton);
	updateImageButton(editButton);
	updateImageButton(settingsButton);
	updateImageButton(logsButton);

	if (libraryPage)
		libraryPage->themeSettingChanged(newThemeName);

	if (favoritePage)
		favoritePage->themeSettingChanged(newThemeName);

	if (listPage)
		listPage->themeSettingChanged(newThemeName);

	repaint();
	resized();
}

// Language Update
void Home::languageSettingChanged(const juce::String& newLanguageName)
{
	currentLanguage = newLanguageName.toStdString();
	if (libraryPage) libraryPage->languageSettingChanged(newLanguageName);
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

	auto listArea = buttonColumn.removeFromTop(buttonHeight2);
	listButton.setBounds(listArea.removeFromLeft((int)(tempWidth * 0.6)).reduced(10));
	listText.setBounds(listArea);

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

	if (listPage && listPage->isVisible())
	{
		listPage->setBounds(bounds);
		return;
	}

	auto suggest = bounds.removeFromTop((int)(bounds.getHeight() * 0.5));
	tempSuggest.setBounds(suggest);

	recent.setBounds(bounds.removeFromTop((int)(bounds.getHeight() * 0.3)));

	auto buttonWidth3 = bounds.getWidth() / 5;

	tempAudio5.setBounds(bounds.removeFromRight(buttonWidth3).reduced(10, 10));
	tempAudio4.setBounds(bounds.removeFromRight(buttonWidth3).reduced(10, 10));
	tempAudio3.setBounds(bounds.removeFromRight(buttonWidth3).reduced(10, 10));
	tempAudio2.setBounds(bounds.removeFromRight(buttonWidth3).reduced(10, 10));
	tempAudio1.setBounds(bounds.removeFromRight(buttonWidth3).reduced(10, 10));
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