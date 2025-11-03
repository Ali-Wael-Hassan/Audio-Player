#pragma once
#include "PlayerAudio.h"
#include <map>
///    على الكلام Bold الكلاس ده بيغير الحجم و بيعمل برده 
class CustomButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
	juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override
	{
		return juce::Font(20.0f, juce::Font::bold);
	}
};

class PlayerGUI :
	public juce::Component,
	public juce::Button::Listener,	//لما حد يدوس أزرار
	public juce::Slider::Listener, //لما حد يحرك السلايدرز
	public PlayerAudioSignal, // ياخد إشارات من الصوت
	public juce::AsyncUpdater,// يعمل تحديثات
	public juce::Timer, //  عشان يعمل حاجات كل فترة زي تحديث الوقت
	// waveform و كمان مهم فى 
	public juce::ChangeListener// تغيرات
{
private:
	// Custom LookAndFeel
	CustomButtonLookAndFeel customLookAndFeel;

	// Audio
	//pointer البيشغل الكود
	PlayerAudio* control;

	// GUI Controls - Buttons
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
  juce::TextButton markerButton{ "Flag Marker A/B:Not Set" };
	juce::TextButton settingsButton;


	std::vector<juce::TextButton*> buttons = {
		&loadButton, &restartButton, &stopButton, &playButton,
		&muteButton, &go_to_startButton, &go_to_endButton,
		&speedButton, &repeatButton ,&forwardButton ,&backwardButton ,&settingsButton, &markerButton
	};

	// Playlist Panel Controls //زوات حاجات بس فى حاجات منهم مش شغاله 
	juce::TextButton addToPlaylistButton{ "Add to Playlist" }; //شغال
	juce::TextButton removeFromPlaylistButton{ "Remove Selected" };//شغال
	juce::TextButton savePlaylistButton{ "Save Playlist" };// مش شغاله
	juce::TextButton loadPlaylistButton{ "Load Playlist" };//مش شغاله
	juce::Label playlistTitleLabel;

	// Custom ListBox for Playlist // بتاع علي 
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

		int getNumRows() override
		{
			return (int)playlistItems.size();
		}

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
			{
				owner.playTrackFromPlaylist(row);
			}
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

	private:
		PlayerGUI& owner;
		std::vector<std::pair<std::string, std::string>> playlistItems;
		int currentPlayingIndex = -1;
	};

	std::unique_ptr<PlaylistListBox> playlistBox;

	std::string currentPlaylistKey;
	int currentPlaylistIndex = -1;
	bool showPlaylistPanel = false;

	// Custom Circular Slider for Volume
	class CircularVolumeSlider : public juce::Slider //للتحكم في مستوى الصوت
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

				if (t <= currentValue)
				{
					g.setColour(juce::Colours::white);
				}
				else
				{
					g.setColour(juce::Colours::black.withAlpha(0.6f));
				}

				g.fillEllipse(x - dotRadius, y - dotRadius, dotRadius * 2, dotRadius * 2);
			}
		}
	};

	// Sliders
	CircularVolumeSlider volumeSlider;
	juce::Slider speedSlider;
	juce::Label speedLabel;

	// Position Slider (للتحكم في مكان الأغنية)
	juce::Slider positionSlider;

	bool isUserDraggingPosition = false;
	double lastVal = 0.5;
	bool muted = false;
	bool stoped = true;

	// Labels
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

	void initializeControls();
	void refreshPlaylist();
	void playTrackFromPlaylist(int index);

	// Waveform components
	juce::AudioFormatManager defaultFormatManager;
	juce::AudioThumbnailCache thumbnailCache{ 5 };
	juce::AudioThumbnail thumbnail;
	//الاشكال و المستطيلات 
	juce::Rectangle<int> waveformArea;
	juce::Rectangle<int> controlButtonsArea;
	juce::Rectangle<int> playlistPanelArea;
	juce::Rectangle<int> headerBoxArea;

    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void refreshPlaylistDisplay();
    void updateSelectedRow();
    void loadNextTrack();

public:
	PlayerGUI();
	PlayerGUI(PlayerAudio& control);
	~PlayerGUI()
	{
		// Clean up LookAndFeel (مفيش nav buttons دلوقتي)
	}

	void paint(juce::Graphics& g) override; //بترسم كل حاجة على الشاشة
	void resized() override;//الجحم و التوزيعه 
	void buttonClicked(juce::Button* button) override;// بتتنفذ لما حد يدوس على أي زرار
	void sliderValueChanged(juce::Slider* slider) override;// سلايدر
	void sliderDragStarted(juce::Slider* slider) override;// سلايدر
	void sliderDragEnded(juce::Slider* slider) override;// سلايدر
	void playBackFinished() override;//بتتنفذ لما الأغنية تخلص
	void timerCallback() override;// بتتنفذ كل فترة معينة (زي تحديث وقت الأغنية كل ثانية

	// --- ADDED: Missing Function Declarations ---
	void handleAsyncUpdate() override;
	void changeListenerCallback(juce::ChangeBroadcaster* source) override;//بتتنفذ لما يحصل تغيير في الصوت (زي لما يتحمل ملف جديد)
	// تحريك ال wave 
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseDrag(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;

	juce::Slider& getVolume();
	juce::Slider& getPosition();

};