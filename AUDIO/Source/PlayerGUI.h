#pragma once
#include "PlayerAudio.h"
#include <map>
#include <cstdlib>

class NewPlaylistPopup : public juce::Component
{
public:

	// Popup Menu for Getting Playlist name from User
	NewPlaylistPopup(std::function<void(const juce::String&)> callback)
		: onCreate(callback)
	{

		// Making Components visible
		addAndMakeVisible(nameEditor);
		addAndMakeVisible(createButton);

		// Setting text
		createButton.setButtonText("Create");
		// Using Lambda function to return text when clicking
		createButton.onClick = [this]()
			{
				if (onCreate)
					onCreate(nameEditor.getText());
				getParentComponent()->removeChildComponent(this);
			};
	}

	void resized() override
	{
		// Text Input
		nameEditor.setBounds(x, y, getWidth() - 20, 24);
		// Apply Button
		createButton.setBounds(x, y+30, getWidth() - 20, 24);
	}

private:
	// X, Y for setting the menu location
	int x, y;
	juce::TextEditor nameEditor;
	juce::TextButton createButton;
	// Funtion Object
	// std::function<parameters(return type)> name
	std::function<void(const juce::String&)> onCreate;
};

///    على الكلام Bold الكلاس ده بيغير الحجم و بيعمل برده 
class CustomButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
	juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override
	{
		// Segoe UI Symbol for better windows support to look native
		return juce::Font("Segoe UI Symbol", 24.0f, juce::Font::bold);
	}
};

class PlayerGUI :
	public juce::Component, // Making the user object
	public juce::Button::Listener,	//لما حد يدوس أزرار
	public juce::Slider::Listener, //لما حد يحرك السلايدرز
	public PlayerAudio::PlayerAudioSignal, // ياخد إشارات من الصوت
	public juce::Timer, //  عشان يعمل حاجات كل فترة زي تحديث الوقت
	// waveform و كمان مهم فى 
	public juce::ChangeListener// تغيرات
{
public:
	// ===============================================
	// ============== Listener =======================
	// ===============================================

	// Listener class to make this class communicate with others
	class Listener
	{
	public:
		virtual ~Listener() = default;
		// return to home page
		virtual void returnToHomePage() = 0;
		// Open multiple Players
		virtual void twoGUI() = 0;
		// Open Settings from home(central class) to communicate with all classes
		virtual void openSettings() = 0;
	};
	// Set listener method to send singals to Home
	void setPlayerListener(PlayerGUI::Listener* l) { listen = l; }

	// ===============================================
	// ============== Creation\Destruction ===========
	// ===============================================
	// Constructor accepts address of audio player (audio engine), url (path from The Internet), fileName (local path)
	PlayerGUI(PlayerAudio& control, juce::String url, juce::String fileName);
	// Destructor does nothing since there is no allocation
	~PlayerGUI()
	{

	}
	// Helper method for setup
	void initializeControls();

	// ===============================================
	// ============== Last Sessions Setup ============
	// ===============================================
	// Called by Home to load from library and last sessions
	void loadSong(juce::String source);
	void loadPlaylist(juce::String name);

	// ===============================================
	// ============== Screen Setup ===================
	// ===============================================
	// Painting To The Screen
	void paint(juce::Graphics& g) override;
	// Resizing Window (nearly responsive)
	void resized() override;


	// ===============================================
	// ============== GUI Buttons ====================
	// ===============================================
	// Connect the logic with GUI
	void buttonClicked(juce::Button* button) override;

	// ===============================================
	// ============== Sliders Methods ================
	// ===============================================
	// Updating values that has relation to the slider
	void sliderValueChanged(juce::Slider* slider) override;
	// Track when user start dragging
	void sliderDragStarted(juce::Slider* slider) override;
	// Track when user stop dragging
	void sliderDragEnded(juce::Slider* slider) override;

	// ===============================================
	// ============== Receiving Signals ==============
	// ===============================================
	// Manage When the Audio file ends (with the four loop states)
	void playBackFinished() override;
	// Method from PlayerAudio::Listener to receive signals from the audio engine
	void loadMetaData() override;
	void loadWave(juce::File file) override;

	// ===============================================
	// ============== Checkers =======================
	// ===============================================
	// Track the program state every x seconds
	void timerCallback() override;
	// Listen for general Signals that doesn't have specific Listener
	void changeListenerCallback(juce::ChangeBroadcaster* source) override;
	// Manage Wave Area Clicking
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseDrag(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;

	// ===============================================
	// ============== Setters\Getters ================
	// ===============================================
	// Getter For Saving Some Format in last sessions
	juce::Slider& getVolume();
	juce::Slider& getPosition();

	// ===============================================
	// ============== Refreshing and Helpers =========
	// ===============================================
	// Applied when returning home
	void reset();
	// Refresh GUI
	void refreshPlaylistSelector();
	void refreshPlaylist();
	// Play Specific song using its index
	void playTrackFromPlaylist(int index);
	// Save in some format for last sessions
	void save(const std::string& path);
	// load of last session
	void setAll(float lastVolume, float lastPosition, bool loop, std::vector<Marker> m);
	void PlayerGUI::updateMarkerLabel();

private:
	// ===============================================
	// ============== Marker Management ==============
	// ===============================================
	std::vector<Marker> markers;
	juce::ComboBox Markers;
	juce::String Labels;
	int markerA, markerB;
	juce::TextButton removeMarker;
	juce::TextButton setA;
	juce::TextButton setB;
	juce::TextButton StartLooping;
	juce::TextButton goToMarker;
	juce::Rectangle<int> MarkerPanelArea;
	juce::TextEditor labelEditor;

	// ===============================================
	// ============== Playlist Management ============
	// ===============================================
	juce::Rectangle<int> playlistPanelArea;
	juce::ComboBox playlistSelector;
	juce::Label playlistTitleLabel;

	class PlaylistListBox : public juce::ListBox, public juce::ListBoxModel
	{
	public:
		PlaylistListBox(PlayerGUI& owner) : owner(owner)
		{
			setModel(this);
			setRowHeight(40);
			setColour(juce::ListBox::backgroundColourId, juce::Colour::fromRGBA(80, 60, 120, 180));
			setColour(juce::ListBox::outlineColourId, juce::Colour::fromRGB(100, 80, 150));
		}

		int getNumRows() override { return (int)playlistItems.size(); }

		void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override
		{
			if (rowNumber < 0 || rowNumber >= playlistItems.size()) return;

			if (rowIsSelected)
				g.fillAll(juce::Colour::fromRGB(100, 80, 150));
			else if (rowNumber == currentPlayingIndex)
				g.fillAll(juce::Colour::fromRGB(120, 100, 180).withAlpha(0.5f));
			else
				g.fillAll(juce::Colour::fromRGB(60, 40, 100).withAlpha(0.3f));

			g.setColour(juce::Colours::white);
			g.setFont(14.0f);
			auto& item = playlistItems[rowNumber];
			juce::String displayText = juce::String(rowNumber + 1) + ". " + item.first;
			g.drawText(displayText, 10, 0, width - 20, height, juce::Justification::centredLeft);

			if (rowNumber == currentPlayingIndex)
			{
				g.setColour(juce::Colours::lightgreen);
				g.fillEllipse(width - 25, height / 2 - 5, 10, 10);
			}
		}

		void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override
		{
			if (row >= 0 && row < playlistItems.size())
				owner.playTrackFromPlaylist(row);
		}

		void updatePlaylist(const std::vector<std::pair<std::string, std::string>>& items)
		{
			playlistItems = items;
			updateContent();
			repaint();
		}

		void setCurrentPlayingIndex(int index)
		{
			currentPlayingIndex = index;
			repaint();
		}

		std::string getSelectedItemKey()
		{
			int selectedRow = getSelectedRow();
			if (selectedRow >= 0 && selectedRow < playlistItems.size())
				return playlistItems[selectedRow].first;
			return "";
		}

		void deselectAll()
		{
			currentPlayingIndex = -1;
			deselectAllRows();
			repaint();
		}

	private:
		PlayerGUI& owner;
		std::vector<std::pair<std::string, std::string>> playlistItems;
		int currentPlayingIndex = -1;
	};

	std::unique_ptr<PlaylistListBox> playlistBox;
	std::string currentPlaylistKey;
	int currentPlaylistIndex = -1;
	bool showPlaylistPanel = false;
	bool twoGUIs = false;

	// Playlist Buttons
	juce::TextButton addToPlaylistButton{ "Add to Playlist" };
	juce::TextButton removeFromPlaylistButton{ "Remove Selected" };
	juce::TextButton savePlaylistButton{ "Save Playlist" };
	juce::TextButton addNewPlaylistButton{ "add Playlist" };

	// ===============================================
	// ============== Custom Font ====================
	// ===============================================
	CustomButtonLookAndFeel customLookAndFeel;

	// ===============================================
	// ============== Engine & Listener ==============
	// ===============================================
	PlayerAudio* control;
	PlayerGUI::Listener* listen;

	// ===============================================
	// ============== Main Buttons ===================
	// ===============================================
	juce::TextButton loadButton{ "Load Files" };
	juce::TextButton restartButton;
	juce::TextButton stopButton;
	juce::TextButton playButton;
	juce::TextButton muteButton;
	juce::TextButton go_to_startButton{ "|<" };
	juce::TextButton go_to_endButton;
	juce::TextButton speedButton{ "Speed: 1.0x" };
	juce::TextButton repeatButton;
	juce::TextButton forwardButton;
	juce::TextButton backwardButton;
	juce::TextButton settingsButton;

	std::vector<juce::TextButton*> buttons = {
		&loadButton, &restartButton, &stopButton, &playButton,
		&muteButton, &go_to_startButton, &go_to_endButton,
		&speedButton, &repeatButton, &forwardButton, &backwardButton, &settingsButton
	};

	// ===============================================
	// ============== Sliders ========================
	// ===============================================
	class CircularVolumeSlider : public juce::Slider
	{
	public:
		CircularVolumeSlider()
		{
			setSliderStyle(juce::Slider::Rotary);
			setRotaryParameters(juce::MathConstants<float>::pi * 1.2f,
				juce::MathConstants<float>::pi * 2.8f,
				true);
			setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
			setInterceptsMouseClicks(true, false);
		}

		bool hitTest(int x, int y) override
		{
			auto centre = getLocalBounds().getCentre();
			auto dx = x - centre.x;
			auto dy = y - centre.y;
			auto distanceFromCentre = std::sqrt(dx * dx + dy * dy);
			auto radius = juce::jmin(getWidth(), getHeight()) / 2.0f;
			return distanceFromCentre > (radius * 0.4f) && distanceFromCentre < (radius * 0.9f);
		}

		void paint(juce::Graphics& g) override
		{
			auto bounds = getLocalBounds().toFloat();
			auto centre = bounds.getCentre();
			auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
			auto startAngle = juce::MathConstants<float>::pi * 1.2f;
			auto endAngle = juce::MathConstants<float>::pi * 2.8f;
			auto totalAngleRange = endAngle - startAngle;

			int numDots = 12;
			float dotRadius = 6.0f;
			float dotDistance = radius * 0.75f;
			float currentValue = (float)getValue();

			for (int i = 0; i < numDots; ++i)
			{
				float t = (float)i / (float)(numDots - 1);
				float angle = startAngle + (totalAngleRange * t);
				float x = centre.x + std::cos(angle) * dotDistance;
				float y = centre.y + std::sin(angle) * dotDistance;
				g.setColour(t <= currentValue ? juce::Colours::white : juce::Colours::black.withAlpha(0.6f));
				g.fillEllipse(x - dotRadius, y - dotRadius, dotRadius * 2, dotRadius * 2);
			}
		}
	};

	CircularVolumeSlider volumeSlider;
	juce::Slider speedSlider;
	juce::Label speedLabel;
	juce::Slider positionSlider;
	bool isUserDraggingPosition = false;
	double lastVal = 0.5;
	bool muted = false;
	bool stoped = true;

	// ===============================================
	// ============== Labels ========================
	// ===============================================
	juce::Label author;
	juce::Label songTitle;
	juce::Label durationHeader;
	juce::Label name;
	juce::Label title;
	juce::Label duration;

	std::vector<std::pair<juce::String, juce::Label*>> labels = {
		{"Author: ", &author},
		{"Title: ", &songTitle},
		{"Duration: ", &durationHeader},
		{"Unknown", &name},
		{"No Track Loaded", &title},
		{"0:00", &duration}
	};

	std::unique_ptr<juce::FileChooser> fileChooser;

	// ===============================================
	// ============== Waveform ======================
	// ===============================================
	juce::AudioThumbnail thumbnail;
	juce::Rectangle<int> waveformArea;
	juce::Rectangle<int> controlButtonsArea;
	juce::Rectangle<int> headerBoxArea;
};