#include "LibraryPage.h"
#include <fstream> // For saving files

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
		}},
		{"Light", {
		// BACKGROUNDS & BARS
		{"background_top", juce::Colour::fromRGB(0xF8, 0xF8, 0xF8)},
		{"background_bottom", juce::Colour::fromRGB(0xEE, 0xEE, 0xEE)},
		{"text_color", juce::Colours::black},
		{"list_bg", juce::Colour::fromRGBA(0xFF, 0xFF, 0xFF, 0x60)},
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
		g.drawText(JUCE_T("\xe2\x99\xab"), thumbArea, juce::Justification::centred); // Music note
	}

	g.setColour(rowIsSelected ? juce::Colours::white : rowTextColor);
	g.setFont(height * 0.45f);

	// Layout with Favorite Icon
	auto favoriteArea = bounds.removeFromRight(height); // Square area for heart
	auto durationWidth = (int)(width * 0.15);
	auto thumbnailWidth = height;
	auto durationArea = bounds.removeFromRight(durationWidth);
	// Adjust title width calculation to account for thumbnail, duration, and favorite
	auto titleArea = bounds.removeFromLeft((int)((width - thumbnailWidth - durationWidth - favoriteArea.getWidth()) * 0.65));
	auto artistArea = bounds;

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
		g.drawText(JUCE_T("\xe2\x99\xa5"), favoriteArea, juce::Justification::centred); // Filled heart
	}
	else
	{
		g.setColour(rowIsSelected ? juce::Colours::white.withAlpha(0.7f) : rowTextColor.withAlpha(0.7f));
		g.drawText(JUCE_T("\xe2\x99\xa1"), favoriteArea, juce::Justification::centred); // Empty heart
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
	else
	{

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
		g.drawText(JUCE_T("\xe2\x96\xa4"), thumbArea, juce::Justification::centred); // Playlist icon
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
		g.drawText(JUCE_T("\xe2\x99\xa5"), favoriteArea, juce::Justification::centred); // Filled heart
	}
	else
	{
		g.setColour(rowIsSelected ? juce::Colours::white.withAlpha(0.7f) : rowTextColor.withAlpha(0.7f));
		g.drawText(JUCE_T("\xe2\x99\xa1"), favoriteArea, juce::Justification::centred); // Empty heart
	}

	g.setColour(rowTextColor.withAlpha(0.2f));
	g.fillRect(originalBounds.withTop(height - 1));
}

void PlaylistListModel::listBoxItemClicked(int row, const juce::MouseEvent& e)
{
	// Get the bounds of the row. We know the row height is 60.
	auto rowBounds = juce::Rectangle<int>(0, 0, e.eventComponent->getWidth(), 60);
	// The favorite area is a square on the right side, with height 60
	auto favoriteArea = rowBounds.removeFromRight(60);

	if (favoriteArea.contains(e.getPosition()))
	{
		// Click was on the heart, notify the main LibraryPage
		// ownerPage.togglePlaylistFavoriteStatus(row); // This function is NOT in the new header.
	}
	else
	{
		// Click was elsewhere
	}
}

//==============================================================================
// LIBRARY PAGE
//==============================================================================

LibraryPage::LibraryPage(const std::string& themeColor, const std::string& language, const juce::String& songFile, const juce::String& playlistFile)
	: currentTheme(themeColor), currentLanguage(language), songFile(songFile), playlistFile(playlistFile)
{
	// Load data from files
	loadSongDataFromFile();
	loadPlaylistDataFromFile();

	// Setup Song List
	songsHeader.setFont(juce::Font(24.0f, juce::Font::bold));
	songsHeader.setJustificationType(juce::Justification::centredLeft);
	addAndMakeVisible(songsHeader);

	// Setup Column Headers
	auto setupHeaderLabel = [&](juce::Label& label, const juce::String& text)
		{
			label.setText(text, juce::dontSendNotification);
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
	repaint();
}

//==============================================================================
// FAVORITE TOGGLING
//==============================================================================

void LibraryPage::toggleFavoriteStatus(int rowNumber)
{
	if (rowNumber < 0 || rowNumber >= songs.size())
		return;

	saveSongDataToFile();

	songs.getReference(rowNumber).isFavorite = !songs[rowNumber].isFavorite;

	songListBox.repaintRow(rowNumber);

	saveSongDataToFile();
	removeSongfromFavorite();
	loadSongDataFromFile();
	songListBox.repaint();
}

void LibraryPage::saveSongDataToFile()
{
	std::ofstream file(songFile.toStdString());
	if (!file.is_open())
	{
		return;
	}

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

		file << parts.joinIntoString("|").toStdString() << "\n";
	}

	file.close();
}

void LibraryPage::removeSongfromFavorite()
{
	juce::Array<SongData> currentFavorites;
	juce::File favoritesFile("FavoritSong.txt");

	if (favoritesFile.existsAsFile())
	{
		juce::String fileContent = favoritesFile.loadFileAsString();
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
				SongData loadedSong;
				loadedSong.title = parts[0].trim();
				loadedSong.artist = parts[1].trim();

				juce::String source = parts[2].trim();
				if (source.startsWithIgnoreCase("http://") || source.startsWithIgnoreCase("https://"))
					loadedSong.url = source;
				else
					loadedSong.filePath = juce::File(source);

				if (parts.size() >= 4)
				{
					juce::String thumbSource = parts[3].trim();
					if (thumbSource.startsWithIgnoreCase("http://") || thumbSource.startsWithIgnoreCase("https://"))
						loadedSong.thumbnailUrl = thumbSource;
					else
						loadedSong.thumbnailFile = juce::File(thumbSource);
				}
				if (parts.size() >= 5)
					loadedSong.duration = parts[4].trim();

				if (loadedSong.title.isNotEmpty())
				{
					currentFavorites.add(loadedSong);
				}
			}
		}
	}

	for (const auto& song : songs)
	{
		bool alreadyInFavorites = false;
		int indexInFavorites = -1;
		for (int i = 0; i < currentFavorites.size(); ++i)
		{
			if (currentFavorites[i].title == song.title)
			{
				alreadyInFavorites = true;
				indexInFavorites = i;
				break;
			}
		}

		if (song.isFavorite)
		{
			if (!alreadyInFavorites)
			{
				currentFavorites.add(song);
			}
		}
		else
		{
			if (alreadyInFavorites)
			{
				currentFavorites.remove(indexInFavorites);
			}
		}
	}

	std::ofstream file(favoritesFile.getFullPathName().toStdString());
	if (!file.is_open())
	{
		return;
	}

	for (const auto& song : currentFavorites)
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

		file << parts.joinIntoString("|").toStdString() << "\n";
	}

	file.close();
}


//==============================================================================
// FILE LOADING IMPLEMENTATION
//==============================================================================

// Reads song data from List.txt (Title | Artist | SourcePath/URL | ThumbnailSource | Duration | isFavorite)
void LibraryPage::loadSongDataFromFile()
{
	songs.clear();
	juce::File libraryFile(this->songFile);

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
	juce::File playlistFile(this->playlistFile);

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
