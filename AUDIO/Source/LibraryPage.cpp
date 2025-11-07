#include "LibraryPage.h"
#include <fstream>
#include <sstream>

//==============================================================================
// STATIC MEMBER INITIALIZATION
//==============================================================================

const std::map<std::string, LibraryPage::ThemeColors> LibraryPage::ThemeColorMap = {
{"Dark", {
		// BACKGROUNDS & BARS
		{"background_top", juce::Colour::fromRGB(0x03, 0x00, 0x42)},
		{"background_bottom", juce::Colour::fromRGB(0x23, 0x23, 0x3d)},
		{"text_color", juce::Colours::white},
		{"list_bg", juce::Colour::fromRGBA(0x00, 0x00, 0x00, 0x20)},
		{"topbar_top", juce::Colour::fromRGB(0x60, 0x5e, 0x5e).withAlpha(0.55f)},
		{"topbar_bottom", juce::Colour::fromRGB(0x30, 0x00, 0x5d)},
		{"leftbar_top", juce::Colour::fromRGB(0x33, 0x05, 0x49).withAlpha(0.55f)},
		{"leftbar_bottom", juce::Colour::fromRGB(0x21, 0x00, 0x32)},
		{"logo_top", juce::Colour::fromRGB(0x4E, 0x09, 0x84).withAlpha(0.55f)},
		{"logo_bottom", juce::Colour::fromRGB(0x08, 0x00, 0x29)},
		{"search_text", juce::Colours::white},
		{"search_bg", juce::Colour::fromRGB(0x1A, 0x1A, 0x1A)},
		{"button_bg_normal", juce::Colour::fromRGB(0x45, 0x45, 0x45)},
		{"button_bg_hover", juce::Colour::fromRGB(0x60, 0x60, 0x60)},
		{"button_bg_active", juce::Colour::fromRGB(0x00, 0x78, 0xD4)},
		}},
		{"Light", {
		// BACKGROUNDS & BARS
		{"background_top", juce::Colour::fromRGB(0xF8, 0xF8, 0xF8)},
		{"background_bottom", juce::Colour::fromRGB(0xEE, 0xEE, 0xEE)},
		{"text_color", juce::Colours::black},
		{"list_bg", juce::Colour::fromRGBA(0xFF, 0xFF, 0xFF, 0x60)},
		{"topbar_top", juce::Colour::fromRGB(0xE0, 0xE0, 0xE0).withAlpha(0.7f)},
		{"topbar_bottom", juce::Colour::fromRGB(0xC0, 0xC0, 0xC0)},
		{"leftbar_top", juce::Colour::fromRGB(0xF0, 0xF0, 0xF0).withAlpha(0.7f)},
		{"leftbar_bottom", juce::Colour::fromRGB(0xD8, 0xD8, 0xD8)},
		{"logo_top", juce::Colour::fromRGB(0x00, 0x78, 0xD4).withAlpha(0.7f)},
		{"logo_bottom", juce::Colour::fromRGB(0xE0, 0xE0, 0xE0)},
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

const std::map<juce::String, LibraryPage::LanguagesMap> LibraryPage::LanguageMap = {
	{ "English", {
		{AR_SGL, "Logs"},
		{AR_FAV,			 "Favorite"},
		{AR_HOME,			 "Home"},
		{AR_AD, "Settings"},
		{AR_AGANI, "All Songs"},
		{AR_BA, "Search"},
		{AR_MKTBA, "Library"},
		{AR_MF, "Favorites"},
		{AR_QAEMA, "My List"},
		{AR_TD, "Edit"},
		{AR_NORES, "No results found for"},
		{AR_CLOSE, "Close"},
		{AR_MTHR, "Theme:"},
		{AR_LGHA, "Language"},
		{AR_DARK, "Dark"},
		{AR_LIGHT, "Light" },
		{AR_ARABIC, "Arabic"},
		{AR_PLAYLIST, "Playlist"},
		{AR_MYLIST, "Mylist"},
		{AR_RECENT, "Recent"}
	}},
	{ "Arabic", {
		{"Logs", AR_SGL},
		{"Favorite", AR_FAV},
		{"Home", AR_HOME},
		{"Settings", AR_AD},
		{"All Songs", AR_AGANI},
		{"Search", AR_BA},
		{"Library", AR_MKTBA},
		{"Favorites", AR_MF},
		{"My List", AR_QAEMA},
		{"Edit", AR_TD},
		{"No results found for", AR_NORES},
		{"Close", AR_CLOSE},
		{"Theme:", AR_MTHR},
		{"Language", AR_LGHA},
		{"Dark", AR_DARK},
		{"Light", AR_LIGHT},
		{"Arabic", AR_ARABIC},
		{"Playlist", AR_PLAYLIST},
		{"Mylist", AR_MYLIST},
		{"Recent", AR_RECENT},
	}}
};

//==============================================================================
// SONG LIST MODEL
//==============================================================================

SongListModel::SongListModel(LibraryPage& p, const juce::Array<SongData>& songs)
	: ownerPage(p), songList(songs)
{

}

int SongListModel::getNumRows()
{
	return songList.size();
}

void SongListModel::paintListBoxItem(int rowNumber, juce::Graphics& g,
	int width, int height, bool rowIsSelected)
{
	if (rowNumber < 0 || rowNumber >= songList.size())
		return;

	const auto& song = songList[rowNumber];

	if (rowIsSelected)
	{
		g.setColour(juce::Colour::fromRGB(0x00, 0x78, 0xD4));
		g.fillRect(0, 0, width, height);
	}

	juce::Rectangle<int> bounds(0, 0, width, height);
	auto originalBounds = bounds; // Keep a copy for the separator line

	// Draw Thumbnail
	auto thumbArea = bounds.removeFromLeft(height).reduced(4);
	bool hasThumb = song.thumbnailFile.existsAsFile();

	if (hasThumb)
	{
		juce::Image thumb = juce::ImageCache::getFromFile(song.thumbnailFile);
		if (thumb.isValid())
		{
			g.drawImageWithin(thumb,
				thumbArea.getX(), thumbArea.getY(),
				thumbArea.getWidth(), thumbArea.getHeight(),
				juce::RectanglePlacement::centred, false);
		}
		else
		{
			hasThumb = false;
		}
	}

	if (!hasThumb)
	{
		g.setColour(rowTextColor.withAlpha(0.2f));
		g.fillRect(thumbArea);
		g.setColour(rowTextColor);
		g.setFont(thumbArea.getHeight() * 0.6f);
		g.drawText(juce::String::fromUTF8("\xe2\x99\xab"), thumbArea, juce::Justification::centred);
	}

	g.setColour(rowIsSelected ? juce::Colours::white : rowTextColor);
	g.setFont(height * 0.45f);

	// Layout with Favorite Icon
	auto favoriteArea = bounds.removeFromRight(height); // Square area for heart
	auto durationWidth = (int)(width * 0.15);
	auto durationArea = bounds.removeFromRight(durationWidth);

	int remainingWidth = bounds.getWidth();
	auto titleArea = bounds.removeFromLeft((int)(remainingWidth * 0.55)); // Title 55% of remaining
	auto artistArea = bounds; // Artist gets the rest

	// Draw Title
	g.drawText(song.title, titleArea.reduced(5, 0), juce::Justification::centredLeft, true);

	// Draw Artist
	g.drawText(song.artist, artistArea.reduced(5, 0), juce::Justification::centredLeft, true);

	// Draw Duration
	g.drawText(song.duration, durationArea.reduced(5, 0), juce::Justification::centredRight, true);

	// Draw Favorite Icon
	g.setFont(height * 0.5f);
	if (song.isFavorite)
	{
		g.setColour(juce::Colours::red);
		g.drawText(juce::String::fromUTF8("\xE2\x99\xA5"), favoriteArea, juce::Justification::centred);
	}
	else
	{
		g.setColour(rowIsSelected ? juce::Colours::white.withAlpha(0.7f) : rowTextColor.withAlpha(0.7f));
		g.drawText(juce::String::fromUTF8("\xE2\x99\xA1"), favoriteArea, juce::Justification::centred);
	}


	g.setColour(rowTextColor.withAlpha(0.2f));
	g.fillRect(originalBounds.withTop(height - 1));
}

void SongListModel::listBoxItemClicked(int row, const juce::MouseEvent& e)
{

	auto rowBounds = juce::Rectangle<int>(0, 0, e.eventComponent->getWidth(), 40);
	auto favoriteArea = rowBounds.removeFromRight(40);

	if (favoriteArea.contains(e.getPosition()))
	{
		ownerPage.toggleFavoriteStatus(row);
	}
}

void SongListModel::listBoxItemDoubleClicked(int row, const juce::MouseEvent& e)
{

	auto rowBounds = juce::Rectangle<int>(0, 0, e.eventComponent->getWidth(), 40);
	auto favoriteArea = rowBounds.removeFromRight(40);

	if (favoriteArea.contains(e.getPosition()))
	{

	}
	else
	{
		if (row >= 0 && row < songList.size())
		{
			if (songList[row].url.isNotEmpty()) ownerPage.ClickedSong(songList[row].url);
			else ownerPage.ClickedSong(songList[row].filePath.getFullPathName());
		}
	}
}


//==============================================================================
// PLAYLIST LIST MODEL
//==============================================================================

PlaylistListModel::PlaylistListModel(LibraryPage& p, const juce::Array<PlaylistData>& playlists)
	: ownerPage(p), playlistList(playlists)
{

}

int PlaylistListModel::getNumRows()
{
	return playlistList.size();
}

void PlaylistListModel::paintListBoxItem(int rowNumber, juce::Graphics& g,
	int width, int height, bool rowIsSelected)
{
	if (rowNumber < 0 || rowNumber >= playlistList.size())
		return;

	const auto& playlist = playlistList[rowNumber];

	if (rowIsSelected)
	{
		g.setColour(juce::Colour::fromRGB(0x00, 0x78, 0xD4));
		g.fillRect(0, 0, width, height);
	}

	juce::Rectangle<int> bounds(0, 0, width, height);
	auto originalBounds = bounds; // Keep a copy for the separator line

	// Draw Thumbnail
	auto thumbArea = bounds.removeFromLeft(height).reduced(5);
	bool hasThumb = playlist.thumbnailFile.existsAsFile();

	if (hasThumb)
	{
		juce::Image thumb = juce::ImageCache::getFromFile(playlist.thumbnailFile);
		if (thumb.isValid())
		{
			g.drawImageWithin(thumb,
				thumbArea.getX(), thumbArea.getY(),
				thumbArea.getWidth(), thumbArea.getHeight(),
				juce::RectanglePlacement::centred, false);
		}
		else
		{
			hasThumb = false;
		}
	}

	if (!hasThumb)
	{
		g.setColour(rowTextColor.withAlpha(0.2f));
		g.fillRect(thumbArea);
		g.setColour(rowTextColor);
		g.setFont(thumbArea.getHeight() * 0.6f);
		g.drawText(juce::String::fromUTF8("\xe2\x96\xa4"), thumbArea, juce::Justification::centred);
	}

	// Draw Favorite Icon
	auto favoriteArea = bounds.removeFromRight(height);
	g.setColour(rowIsSelected ? juce::Colours::white : rowTextColor);

	auto textArea = bounds.reduced(5, 0);
	auto nameArea = textArea.removeFromTop(textArea.getHeight() / 2);
	auto countArea = textArea;

	g.setFont(height * 0.35f);
	g.drawText(playlist.name, nameArea, juce::Justification::centredLeft, true);

	g.setFont(height * 0.3f);
	g.setColour(rowIsSelected ? juce::Colours::white.withAlpha(0.7f) : rowTextColor.withAlpha(0.7f));
	juce::String countText = juce::String(playlist.songCount) + " songs";
	g.drawText(countText, countArea, juce::Justification::centredLeft, true);

	// Draw Favorite Icon
	g.setFont(height * 0.5f);
	if (playlist.isFavorite)
	{
		g.setColour(juce::Colours::red);
		g.drawText(juce::String::fromUTF8("\xE2\x99\xA5"), favoriteArea, juce::Justification::centred);
	}
	else
	{
		g.setColour(rowIsSelected ? juce::Colours::white.withAlpha(0.7f) : rowTextColor.withAlpha(0.7f));
		g.drawText(juce::String::fromUTF8("\xE2\x99\xA1"), favoriteArea, juce::Justification::centred);
	}

	g.setColour(rowTextColor.withAlpha(0.2f));
	g.fillRect(originalBounds.withTop(height - 1));
}

void PlaylistListModel::listBoxItemClicked(int row, const juce::MouseEvent& e)
{
	auto rowBounds = juce::Rectangle<int>(0, 0, e.eventComponent->getWidth(), 60);
	auto favoriteArea = rowBounds.removeFromRight(60);

	if (favoriteArea.contains(e.getPosition()))
	{
		ownerPage.togglePlaylistFavoriteStatus(row);
	}
}

void PlaylistListModel::listBoxItemDoubleClicked(int row, const juce::MouseEvent& e)
{
	auto rowBounds = juce::Rectangle<int>(0, 0, e.eventComponent->getWidth(), 60);
	auto favoriteArea = rowBounds.removeFromRight(60);

	if (favoriteArea.contains(e.getPosition()))
	{
		
	}
	else
	{
		ownerPage.ClickedPlaylist(playlistList[row].name);
	}
}

//==============================================================================
// LIBRARY PAGE
//==============================================================================

juce::String LibraryPage::getTranslatedText(const juce::String& englishKey)
{
	const std::string& currentLangKey = (LanguageMap.count(currentLanguage) > 0) ? currentLanguage : "English";

	const auto& targetMap = LanguageMap.at(currentLangKey);

	if (targetMap.count(englishKey.toStdString()))
		return targetMap.at(englishKey.toStdString());

	const auto& englishMap = LanguageMap.at("English");
	if (englishMap.count(englishKey.toStdString()))
		return englishMap.at(englishKey.toStdString());

	return englishKey;
}


LibraryPage::LibraryPage(const std::string& themeColor, const std::string& language, const juce::String& songFile, const juce::String& playlistFile)
	: currentTheme(themeColor), currentLanguage(language), songFile(songFile), playlistFile(playlistFile)
{
	// Load data from files
	if (!this->songFile.isEmpty())
		loadSongDataFromFile();
	if (!this->playlistFile.isEmpty())
		loadPlaylistDataFromFile();

	// Setup Song List
	songsHeader.setText(getTranslatedText("All Songs"), juce::dontSendNotification); // Set initial text
	songsHeader.setFont(juce::Font(24.0f, juce::Font::bold));
	songsHeader.setJustificationType(juce::Justification::centredLeft);
	addAndMakeVisible(songsHeader);

	// Setup Column Headers
	auto setupHeaderLabel = [&](juce::Label& label, const juce::String& text)
		{
			label.setText(getTranslatedText(text), juce::dontSendNotification); // Set initial text
			label.setFont(juce::Font(14.0f, juce::Font::bold));
			label.setJustificationType(juce::Justification::centredLeft);
			addAndMakeVisible(label);
		};

	setupHeaderLabel(titleHeader, "Title");
	setupHeaderLabel(artistHeader, "Artist");
	setupHeaderLabel(durationHeader, "Duration");
	durationHeader.setJustificationType(juce::Justification::centredRight);

	// Initialise models with loaded data
	songModel = std::make_unique<SongListModel>(*this, songs); // Pass 'this'
	songListBox.setModel(songModel.get());
	songListBox.setRowHeight(40);
	songListBox.setOutlineThickness(1);
	addAndMakeVisible(songListBox);

	// Setup Playlist List
	playlistsHeader.setText(getTranslatedText("Playlists"), juce::dontSendNotification); // Set initial text
	playlistsHeader.setFont(juce::Font(24.0f, juce::Font::bold));
	playlistsHeader.setJustificationType(juce::Justification::centredLeft);
	addAndMakeVisible(playlistsHeader);

	// Initialise models with loaded data
	playlistModel = std::make_unique<PlaylistListModel>(*this, playlists); // Pass 'this'
	playlistListBox.setModel(playlistModel.get());
	playlistListBox.setRowHeight(60);
	playlistListBox.setOutlineThickness(1);
	addAndMakeVisible(playlistListBox);

	// Initial theme setup
	themeSettingChanged(juce::String(currentTheme));

	setSize(800, 600);
}

void LibraryPage::paint(juce::Graphics& g)
{
	const std::string& themeKey = ((ThemeColorMap.count(currentTheme) > 0) ? currentTheme : "Dark");
	const auto& themeMap = ThemeColorMap.at(themeKey);
	paintBackground(g, themeMap);
}

void LibraryPage::paintBackground(juce::Graphics& g, const ThemeColors& themeMap)
{
	g.setColour(juce::Colours::transparentBlack);
	g.fillRect(getLocalBounds());
}

void LibraryPage::resized()
{
	auto area = getLocalBounds().reduced(20);

	// Songs Section (Top 60%)
	auto songArea = area.removeFromTop((int)(area.getHeight() * 0.6));

	// Song Header
	songsHeader.setBounds(songArea.removeFromTop(40).reduced(0, 5));

	// Column Headers
	auto headerRowArea = songArea.removeFromTop(20);
	headerRowArea.removeFromLeft(songListBox.getRowHeight()); // Skip thumbnail area
	headerRowArea.removeFromRight(songListBox.getRowHeight()); // Skip favorite icon area

	// Corrected layout calculation
	auto durationWidth = (int)(headerRowArea.getWidth() * 0.20); // Give duration 20%
	auto durationHeaderArea = headerRowArea.removeFromRight(durationWidth);
	auto titleHeaderArea = headerRowArea.removeFromLeft((int)(headerRowArea.getWidth() * 0.55)); // Give title 55%
	auto artistHeaderArea = headerRowArea; // Artist gets the rest

	titleHeader.setBounds(titleHeaderArea.reduced(5, 0));
	artistHeader.setBounds(artistHeaderArea.reduced(5, 0));
	durationHeader.setBounds(durationHeaderArea.reduced(5, 0));

	// Song List Box
	songListBox.setBounds(songArea);

	// small gap between sections
	area.removeFromTop(20);

	auto playlistArea = area;

	// Playlists Header
	playlistsHeader.setBounds(playlistArea.removeFromTop(40).reduced(0, 5));

	// Playlist List Box
	playlistListBox.setBounds(playlistArea);
}

// Theme Update
void LibraryPage::themeSettingChanged(const juce::String& newThemeName)
{
	currentTheme = newThemeName.toStdString();

	const std::string& themeKey = ((ThemeColorMap.count(currentTheme) > 0) ? currentTheme : "Dark");
	const auto& themeMap = ThemeColorMap.at(themeKey);

	juce::Colour textColor = themeMap.at("text_color");
	juce::Colour listBg = themeMap.at("list_bg");

	songsHeader.setColour(juce::Label::textColourId, textColor);
	playlistsHeader.setColour(juce::Label::textColourId, textColor);

	titleHeader.setColour(juce::Label::textColourId, textColor.withAlpha(0.7f));
	artistHeader.setColour(juce::Label::textColourId, textColor.withAlpha(0.7f));
	durationHeader.setColour(juce::Label::textColourId, textColor.withAlpha(0.7f));

	songListBox.setColour(juce::ListBox::backgroundColourId, listBg);
	playlistListBox.setColour(juce::ListBox::backgroundColourId, listBg);

	// NOTE: Setting ListBox outlines for theme consistency
	juce::Colour outlineColor = textColor.withAlpha(0.2f);
	songListBox.setColour(juce::ListBox::outlineColourId, outlineColor);
	playlistListBox.setColour(juce::ListBox::outlineColourId, outlineColor);


	if (songModel) songModel->setRowTextColor(textColor);

	if (playlistModel) playlistModel->setRowTextColor(textColor);

	songListBox.repaint();
	playlistListBox.repaint();

	repaint();
}

// Language Update
void LibraryPage::languageSettingChanged(const juce::String& newLanguageName)
{
	currentLanguage = newLanguageName.toStdString();

	const std::string& langKey = (LibraryPage::LanguageMap.count(currentLanguage) > 0)
		? currentLanguage
		: "English";

	const auto& targetLangMap = LibraryPage::LanguageMap.at(langKey);


	auto getTranslatedText = [&](const juce::String& sourceText) -> juce::String {

		if (targetLangMap.count(sourceText.toStdString()))
			return targetLangMap.at(sourceText.toStdString());

		const auto& englishSourceMap = LibraryPage::LanguageMap.at("English");
		if (englishSourceMap.count(sourceText.toStdString()))
		{
			return englishSourceMap.at(sourceText.toStdString());
		}

		if (sourceText == "Search" && langKey == "Arabic")
			return AR_BA;

		return sourceText;
		};

	songsHeader.setText(getTranslatedText("All Songs"), juce::dontSendNotification);
	playlistsHeader.setText(getTranslatedText("Playlists"), juce::dontSendNotification);
	titleHeader.setText(getTranslatedText("Title"), juce::dontSendNotification);
	artistHeader.setText(getTranslatedText("Artist"), juce::dontSendNotification);
	durationHeader.setText(getTranslatedText("Duration"), juce::dontSendNotification);

	songListBox.repaint();
	playlistListBox.repaint();

	repaint();
}

//==============================================================================
// FAVORITE TOGGLING
//==============================================================================

void LibraryPage::toggleFavoriteStatus(int rowNumber)
{
	if (rowNumber < 0 || rowNumber >= songs.size())
		return;

	songs.getReference(rowNumber).isFavorite = !songs[rowNumber].isFavorite;

	songListBox.repaintRow(rowNumber);

	saveSongDataToFile();

	syncFavoritesToFile();
	
	syncLibraryToFile();

	loadSongDataFromFile();

}

void LibraryPage::syncFavoritesToFile()
{
	juce::File exeFolder = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
	juce::File favoritesFile = exeFolder.getChildFile("FavoritSong.txt");

	std::ostringstream newFavoritesContent;

	for (const auto& song : songs)
	{
		if (song.isFavorite)
		{
			juce::StringArray parts;
			parts.add(song.title);
			parts.add(song.artist);

			if (song.url.isNotEmpty()) parts.add(song.url);
			else parts.add(song.filePath.getFullPathName());

			if (song.thumbnailUrl.isNotEmpty()) parts.add(song.thumbnailUrl);
			else parts.add(song.thumbnailFile.getFullPathName());

			parts.add(song.duration);
			parts.add("true");

			newFavoritesContent << parts.joinIntoString("|").toStdString() << "\n";
		}
	}
	favoritesFile.replaceWithText(newFavoritesContent.str());
}

void LibraryPage::syncLibraryToFile()
{
	juce::File exeFolder = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
	juce::File libraryFile = exeFolder.getChildFile("List.txt");
	juce::File favoritesFile = exeFolder.getChildFile("FavoritSong.txt");

	if (!libraryFile.existsAsFile() || !favoritesFile.existsAsFile())
		return;

	juce::String libraryContent = libraryFile.loadFileAsString();
	juce::StringArray libraryLines;
	libraryLines.addLines(libraryContent);

	std::set<juce::String> mainSongKeys;
	for (const auto& song : songs)
	{
		mainSongKeys.insert(song.title + "|" + song.artist);
	}

	juce::String favContent = favoritesFile.loadFileAsString();
	juce::StringArray favLines;
	favLines.addLines(favContent);

	for (int i = 0; i < libraryLines.size(); ++i)
	{
		juce::String line = libraryLines[i].trim();
		if (line.isEmpty()) continue;

		juce::StringArray parts;
		parts.addTokens(line, "|", "");

		if (parts.size() < 6) continue;

		juce::String key = parts[0] + "|" + parts[1];

		bool isInFav = false;
		for (const auto& favLine : favLines)
		{
			juce::StringArray favParts;
			favParts.addTokens(favLine, "|", "");
			if (favParts.size() >= 6 && (favParts[0] + "|" + favParts[1]) == key)
			{
				isInFav = true;
				break;
			}
		}

		if (mainSongKeys.find(key) == mainSongKeys.end() || !isInFav)
		{
			parts.set(5, "false");
			libraryLines.set(i, parts.joinIntoString("|"));
		}
	}

	libraryFile.replaceWithText(libraryLines.joinIntoString("\n"));
}

void LibraryPage::togglePlaylistFavoriteStatus(int rowNumber)
{
	if (rowNumber < 0 || rowNumber >= playlists.size())
		return;

	playlists.getReference(rowNumber).isFavorite = !playlists[rowNumber].isFavorite;

	playlistListBox.repaintRow(rowNumber);

	savePlaylistDataToFile();

	syncFavoritePlaylistsToFile();

	syncLibraryPlaylistToFile();

	loadPlaylistDataFromFile();
}

void LibraryPage::syncFavoritePlaylistsToFile()
{
	juce::File exeFolder = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
	juce::File favoritesFile = exeFolder.getChildFile("FavoritPlaylist.txt");

	std::ostringstream newFavoritesContent;

	for (const auto& playlist : playlists)
	{
		if (playlist.isFavorite)
		{
			juce::StringArray parts;
			parts.add(playlist.name);
			parts.add(juce::String(playlist.songCount));

			if (playlist.thumbnailUrl.isNotEmpty()) parts.add(playlist.thumbnailUrl);
			else parts.add(playlist.thumbnailFile.getFullPathName());

			parts.add("true");

			newFavoritesContent << parts.joinIntoString("|").toStdString() << "\n";
		}
	}
	favoritesFile.replaceWithText(newFavoritesContent.str());
}

void LibraryPage::syncLibraryPlaylistToFile()
{
	juce::File exeFolder = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
	juce::File libraryFile = exeFolder.getChildFile("PLayList.txt");
	juce::File favoritesFile = exeFolder.getChildFile("FavoritPlaylist.txt");

	if (!libraryFile.existsAsFile() || !favoritesFile.existsAsFile())
		return;

	juce::String libraryContent = libraryFile.loadFileAsString();
	juce::StringArray libraryLines;
	libraryLines.addLines(libraryContent);

	juce::String favContent = favoritesFile.loadFileAsString();
	juce::StringArray favLines;
	favLines.addLines(favContent);

	for (int i = 0; i < libraryLines.size(); ++i)
	{
		juce::String line = libraryLines[i].trim();
		if (line.isEmpty()) continue;

		juce::StringArray parts;
		parts.addTokens(line, "|", "");
		if (parts.size() < 4) continue;

		juce::String playlistName = parts[0];
		bool isInFav = false;

		for (const auto& favLine : favLines)
		{
			juce::StringArray favParts;
			favParts.addTokens(favLine, "|", "");
			if (favParts.size() >= 4 && favParts[0] == playlistName)
			{
				isInFav = true;
				break;
			}
		}

		parts.set(3, isInFav ? "true" : "false");
		libraryLines.set(i, parts.joinIntoString("|"));
	}

	libraryFile.replaceWithText(libraryLines.joinIntoString("\n"));
}

void LibraryPage::saveSongDataToFile()
{
	juce::File exeFolder = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
	juce::File file = exeFolder.getChildFile(songFile);

	std::ofstream out(file.getFullPathName().toStdString());
	if (!out.is_open()) return;

	for (const auto& song : songs)
	{
		juce::StringArray parts;
		parts.add(song.title);
		parts.add(song.artist);

		if (song.url.isNotEmpty())
			parts.add(song.url);
		else
			parts.add(song.filePath.getFullPathName());

		if (song.thumbnailUrl.isNotEmpty())
			parts.add(song.thumbnailUrl);
		else
			parts.add(song.thumbnailFile.getFullPathName());

		parts.add(song.duration);
		parts.add(song.isFavorite ? "true" : "false");

		out << parts.joinIntoString("|").toStdString() << "\n";
	}
	out.close();
}

// Name|Songs|URL|Favourite
void LibraryPage::savePlaylistDataToFile()
{
	juce::File exeFolder = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
	juce::File file = exeFolder.getChildFile(playlistFile);

	std::ofstream out(file.getFullPathName().toStdString());
	if (!out.is_open()) return;

	for (const auto& playlist : playlists)
	{
		juce::StringArray parts;
		parts.add(playlist.name);
		parts.add(juce::String(playlist.songCount));

		if (playlist.thumbnailUrl.isNotEmpty())
			parts.add(playlist.thumbnailUrl);
		else
			parts.add(playlist.thumbnailFile.getFullPathName());

		parts.add(playlist.isFavorite ? "true" : "false");

		out << parts.joinIntoString("|").toStdString() << "\n";
	}
	out.close();
}

// Reads song data from List.txt (Title | Artist | SourcePath/URL | ThumbnailSource | Duration | isFavorite)
void LibraryPage::loadSongDataFromFile()
{
	if (this->songFile.isEmpty()) {
		DBG("Error: songFilePath is empty!");
		return;
	}
	songs.clear();
	juce::File exeFolder = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();

	juce::File libraryFile(exeFolder.getChildFile(this->songFile));

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
				juce::String source = parts[2].trim();

				if (title.isNotEmpty())
				{
					SongData newSong;
					newSong.title = title;
					newSong.artist = artist;

					if (source.startsWithIgnoreCase("http://") || source.startsWithIgnoreCase("https://"))
					{
						newSong.url = source;
					}
					else
					{
						newSong.filePath = juce::File(source);
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

					songs.add(newSong);
				}
			}
		}
	}

	if (songListBox.getModel()) songListBox.updateContent();
}

void LibraryPage::loadPlaylistDataFromFile()
{
	playlists.clear();
	juce::File exeFolder = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();

	juce::File playlistFile(exeFolder.getChildFile(this->playlistFile));

	if (playlistFile.existsAsFile())
	{
		juce::String fileContent = playlistFile.loadFileAsString();
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

			if (parts.size() >= 2)
			{
				juce::String name = parts[0].trim();
				int count = parts[1].trim().getIntValue();

				if (name.isNotEmpty())
				{
					PlaylistData newPlaylist;
					newPlaylist.name = name;
					newPlaylist.songCount = count;

					if (parts.size() >= 3)
					{
						juce::String thumbSource = parts[2].trim();
						if (thumbSource.startsWithIgnoreCase("http://") || thumbSource.startsWithIgnoreCase("https"))
						{
							newPlaylist.thumbnailUrl = thumbSource;
						}
						else
						{
							newPlaylist.thumbnailFile = juce::File(thumbSource);
						}
					}

					if (parts.size() >= 4)
					{
						newPlaylist.isFavorite = (parts[3].trim().compareIgnoreCase("true") == 0);
					}

					playlists.add(newPlaylist);
				}
			}
		}
	}
	if (playlistListBox.getModel()) playlistListBox.updateContent();
}