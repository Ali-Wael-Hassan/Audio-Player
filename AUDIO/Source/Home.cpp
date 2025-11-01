#include "Home.h"

const std::map<std::string, Home::ThemeColors> Home::ThemeColorMap = {
	{"Dark", {
		// Main Background
		{"background_top", juce::Colour::fromRGB(0x03, 0x00, 0x42)},
		{"background_bottom", juce::Colour::fromRGB(0x23, 0x23, 0x3d)},
		// Top Bar
		{"topbar_top", juce::Colour::fromRGB(0x60, 0x5e, 0x5e).withAlpha(0.55f)},
		{"topbar_bottom", juce::Colour::fromRGB(0x30, 0x00, 0x5d)},
		// Left Bar
		{"leftbar_top", juce::Colour::fromRGB(0x33, 0x05, 0x49).withAlpha(0.55f)},
		{"leftbar_bottom", juce::Colour::fromRGB(0x21, 0x00, 0x32)},
		// Logo Area
		{"logo_top", juce::Colour::fromRGB(0x4E, 0x09, 0x84).withAlpha(0.55f)},
		{"logo_bottom", juce::Colour::fromRGB(0x08, 0x00, 0x29)},
		// Settings Dimmer & Text
		{"text_color", juce::Colours::white},
		{"dimmer_color", juce::Colour::fromRGBA(0x00, 0x00, 0x00, 0xA0)}
	}},
	{"Light", {
		// Main Background
		{"background_top", juce::Colour::fromRGB(0xF8, 0xF8, 0xF8)},
		{"background_bottom", juce::Colour::fromRGB(0xEE, 0xEE, 0xEE)},
		// Top Bar
		{"topbar_top", juce::Colour::fromRGB(0xE0, 0xE0, 0xE0).withAlpha(0.7f)},
		{"topbar_bottom", juce::Colour::fromRGB(0xC0, 0xC0, 0xC0)},
		// Left Bar
		{"leftbar_top", juce::Colour::fromRGB(0xF0, 0xF0, 0xF0).withAlpha(0.7f)},
		{"leftbar_bottom", juce::Colour::fromRGB(0xD8, 0xD8, 0xD8)},
		// Logo Area
		{"logo_top", juce::Colour::fromRGB(0x00, 0x78, 0xD4).withAlpha(0.7f)},
		{"logo_bottom", juce::Colour::fromRGB(0xE0, 0xE0, 0xE0)},
		// Settings Dimmer & Text
		{"text_color", juce::Colours::black},
		{"dimmer_color", juce::Colour::fromRGBA(0xFF, 0xFF, 0xFF, 0xC0)}
	}}
};

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

void Home::layoutLeftBar(juce::Rectangle<int>& bounds)
{
	int tempWidth = (int)(getWidth() * 0.1);
	auto leftRectangle = bounds.removeFromLeft(tempWidth);

	leftBarArea = leftRectangle;
	auto buttonColumn = leftBarArea;
	auto buttonHeight2 = buttonColumn.getHeight() / 7;

	auto homeArea = buttonColumn.removeFromTop(buttonHeight2);
	homeButton.setBounds(homeArea.removeFromLeft((int)(tempWidth * 0.6)).reduced(10));
	homeText.setBounds(homeArea);

	auto searchArea = buttonColumn.removeFromTop(buttonHeight2);
	searchButton.setBounds(searchArea.removeFromLeft((int)(tempWidth * 0.6)).reduced(10));
	searchText.setBounds(searchArea);

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

void Home::layoutPageArea(juce::Rectangle<int>& bounds)
{
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

void Home::layoutSettingsOverlay()
{
	backgroundDimmerArea = getLocalBounds();

	if (isSettingsVisible)
	{
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
	else
	{
		settingsOverlay->setBounds(0, 0, 0, 0);
		settingsOverlay->setVisible(false);
	}
}

void Home::paintBackgroundGradients(juce::Graphics& g, const std::map<string, juce::Colour>& themeMap)
{
	juce::Colour backGround1 = themeMap.at("background_top");
	juce::Colour backGround2 = themeMap.at("background_bottom");
	juce::ColourGradient gradient(
		backGround1, 0, 0,
		backGround2, (float)getWidth(), (float)getHeight(),
		false
	);
	g.setGradientFill(gradient);
	g.fillRect(getLocalBounds());

	if (topBarArea.getWidth() > 0)
	{
		juce::Colour topBarColor1 = themeMap.at("topbar_top");
		juce::Colour topBarColor2 = themeMap.at("topbar_bottom");

		juce::ColourGradient gradient2(
			topBarColor1,
			topBarArea.getTopLeft().toFloat(),
			topBarColor2,
			topBarArea.getBottomRight().toFloat(),
			false
		);

		g.setGradientFill(gradient2);
		g.fillRect(topBarArea);
	}

	if (leftBarArea.getHeight() > 0)
	{
		juce::Colour leftBarColor1 = themeMap.at("leftbar_top");
		juce::Colour leftBarColor2 = themeMap.at("leftbar_bottom");

		juce::ColourGradient gradient2(
			leftBarColor1,
			leftBarArea.getTopLeft().toFloat(),
			leftBarColor2,
			leftBarArea.getTopRight().toFloat(),
			false
		);

		g.setGradientFill(gradient2);
		g.fillRect(leftBarArea);
	}

	if (logoArea.getHeight() > 0)
	{
		juce::Colour logoColor1 = themeMap.at("logo_top");
		juce::Colour logoColor2 = themeMap.at("logo_bottom");

		juce::ColourGradient gradient2(
			logoColor1,
			logoArea.getTopLeft().toFloat(),
			logoColor2,
			logoArea.getBottomRight().toFloat(),
			false
		);

		g.setGradientFill(gradient2);
		g.fillRect(logoArea);
	}
}

void Home::paintModalDimmer(juce::Graphics& g, const std::map<string, juce::Colour>& themeMap)
{
	if (isSettingsVisible)
	{
		juce::Colour settingsColour = themeMap.at("dimmer_color");
		g.setColour(settingsColour);
		g.fillRect(backgroundDimmerArea);
	}
}

void Home::themeSettingChanged(const juce::String& newThemeName)
{
	currentTheme = newThemeName.toStdString();

	const std::string& themeKey = (Home::ThemeColorMap.count(currentTheme) > 0)
		? currentTheme
		: "Dark";

	juce::Colour newTextColor = Home::ThemeColorMap.at(themeKey).at("text_color");

	for (auto& [msg, textLabel] : labels) {
		textLabel->setColour(juce::Label::textColourId, newTextColor);
	}

	playList.setColour(juce::Label::textColourId, newTextColor);
	recent.setColour(juce::Label::textColourId, newTextColor);

	repaint();
	resized();
}

void Home::languageSettingChanged(const juce::String& newLanguageName)
{
	currentLanguage = newLanguageName.toStdString();
	repaint();
	resized();
}

Home::Home(const string& themeColor = "Dark", const string& language = "English") : currentTheme(themeColor), currentLanguage(language) {

	// Labels
	for (auto& [msg, textLabel] : labels) {
		textLabel->setText(msg, juce::dontSendNotification);
		textLabel->setFont(juce::Font(25.0f, juce::Font::bold));
		textLabel->setJustificationType(juce::Justification::centred);
		textLabel->setColour(juce::Label::textColourId, juce::Colours::white);
		addAndMakeVisible(textLabel);
	}

	playList.setFont(juce::Font(50.0f, juce::Font::bold));
	recent.setFont(juce::Font(50.0f, juce::Font::bold));
	recent.setJustificationType(juce::Justification::centredLeft);

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

	// Top Bar
	juce::Image settings = juce::ImageCache::getFromMemory(
		BinaryData::settings_png,
		BinaryData::settings_pngSize
	);

	settingsButton.setImages(
		true,
		true,
		true,
		settings, 1.0f, {},
		settings, 0.7f, {},
		settings, 0.3f, {}
	);

	addAndMakeVisible(settingsButton);
	settingsButton.addListener(this);

	juce::Image logs = juce::ImageCache::getFromMemory(
		BinaryData::logs_png,
		BinaryData::logs_pngSize
	);

	logsButton.setImages(
		true,
		true,
		true,
		logs, 1.0f, {},
		logs, 0.7f, {},
		logs, 0.3f, {}
	);
	addAndMakeVisible(logsButton);
	logsButton.addListener(this);

	// Left Bar

	juce::Image home = juce::ImageCache::getFromMemory(
		BinaryData::homen_png,
		BinaryData::homen_pngSize
	);

	homeButton.setImages(
		true,
		true,
		true,
		home, 0.3f, {},
		home, 0.3f, {},
		home, 0.3f, {}
	);
	addAndMakeVisible(homeButton);
	homeButton.addListener(this);

	juce::Image search = juce::ImageCache::getFromMemory(
		BinaryData::search_png,
		BinaryData::search_pngSize
	);

	searchButton.setImages(
		true,
		true,
		true,
		search, 1.0f, {},
		search, 0.7f, {},
		search, 0.3f, {}
	);
	addAndMakeVisible(searchButton);
	searchButton.addListener(this);

	juce::Image library = juce::ImageCache::getFromMemory(
		BinaryData::book_png,
		BinaryData::book_pngSize
	);

	libraryButton.setImages(
		true,
		true,
		true,
		library, 1.0f, {},
		library, 0.7f, {},
		library, 0.3f, {}
	);
	addAndMakeVisible(libraryButton);
	libraryButton.addListener(this);

	juce::Image heart = juce::ImageCache::getFromMemory(
		BinaryData::heart_png,
		BinaryData::heart_pngSize
	);

	favoriteButton.setImages(
		true,
		true,
		true,
		heart, 1.0f, {},
		heart, 0.7f, {},
		heart, 0.3f, {}
	);
	addAndMakeVisible(favoriteButton);
	favoriteButton.addListener(this);

	juce::Image list = juce::ImageCache::getFromMemory(
		BinaryData::list_png,
		BinaryData::list_pngSize
	);

	listButton.setImages(
		true,
		true,
		true,
		list, 1.0f, {},
		list, 0.7f, {},
		list, 0.3f, {}
	);
	addAndMakeVisible(listButton);
	listButton.addListener(this);

	juce::Image edit = juce::ImageCache::getFromMemory(
		BinaryData::edit_png,
		BinaryData::edit_pngSize
	);

	editButton.setImages(
		true,
		true,
		true,
		edit, 1.0f, {},
		edit, 0.7f, {},
		edit, 0.3f, {}
	);
	addAndMakeVisible(editButton);
	editButton.addListener(this);

	searchBar.setMultiLine(false);
	searchBar.setTextToShowWhenEmpty("Search your music library...", juce::Colours::grey);
	searchBar.setReturnKeyStartsNewLine(false);
	addAndMakeVisible(searchBar);

	settingsOverlay = std::make_unique<SettingsPage>("Dark");
	settingsOverlay->addListener(this);
	settingsOverlay->getCloseButton().addListener(this);
	addAndMakeVisible(*settingsOverlay);
	settingsOverlay->setVisible(false);
	isSettingsVisible = false;

	setSize(500, 250);
}

void Home::paint(juce::Graphics& g) {
	const std::string& themeKey = (Home::ThemeColorMap.count(currentTheme) > 0)
		? currentTheme
		: "Dark";
	const auto& themeMap = Home::ThemeColorMap.at(themeKey);

	paintBackgroundGradients(g, themeMap);

	paintModalDimmer(g, themeMap);

}

void Home::resized() {
	auto bounds = getLocalBounds();

	layoutTopBar(bounds);
	layoutLeftBar(bounds);

	layoutPageArea(bounds);

	layoutSettingsOverlay();
}

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
}