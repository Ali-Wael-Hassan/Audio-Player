#include "PlayerGUI.h"
#include <algorithm>
#include <ctime>

void PlayerGUI::initializeControls()
{
	static CustomButtonLookAndFeel emojiLook;
	juce::LookAndFeel::setDefaultLookAndFeel(&emojiLook);

	// Button set up with modern circular style
	for (auto* btn : buttons)
	{
		addAndMakeVisible(btn);//يخلي الزرار يظهر على الشاشة
		btn->addListener(this); //هيستدعي الزرار 
		//بيحدد الألوان
		btn->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGBA(80, 100, 150, 180));
		btn->setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGBA(100, 120, 200, 220));
		btn->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
	}

	// Control buttons with emojis - larger size
	restartButton.setButtonText(juce::String::fromUTF8("\xE2\x86\xBB"));
	stopButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xB9"));
	playButton.setButtonText(juce::String::fromUTF8("\xE2\x96\xB6"));
	muteButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x8A"));
	go_to_endButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xAD"));
	go_to_startButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xAE"));
	repeatButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x84"));
	forwardButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xA9"));
	backwardButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xAA"));

	// نطبق نفس الإعدادات الأساسية عليهم
	std::vector<juce::TextButton*> newButtons = { &forwardButton, &backwardButton };
	for (auto* btn : newButtons)
	{
		addAndMakeVisible(btn);
		btn->addListener(this);
		// نفس ألوان أزرار التحكم الرئيسية
		btn->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGBA(80, 100, 150, 180));
		btn->setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGBA(100, 120, 200, 220));
		btn->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
	}

	loadButton.setVisible(false);
	// تخصيص لون Load button
	loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(70, 130, 180));
	loadButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(90, 150, 200));
	loadButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);

	// Playlist Panel Controls
	addAndMakeVisible(addToPlaylistButton);
	addToPlaylistButton.addListener(this);
	addToPlaylistButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(100, 120, 180));
	addToPlaylistButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);

	addAndMakeVisible(removeFromPlaylistButton);
	removeFromPlaylistButton.addListener(this);
	removeFromPlaylistButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(180, 80, 100));
	removeFromPlaylistButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
	muteButton.setWantsKeyboardFocus(false);

	// Mute button - transparent background and remove border
	muteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
	muteButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
	muteButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);

	addAndMakeVisible(savePlaylistButton);
	savePlaylistButton.addListener(this);
	savePlaylistButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(80, 150, 100));
	savePlaylistButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);

	addAndMakeVisible(addNewPlaylistButton);
	addNewPlaylistButton.addListener(this);
	addNewPlaylistButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(150, 100, 80));
	addNewPlaylistButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);

	// Playlist Title Label
	//ازار ال playlist
	playlistTitleLabel.setText(juce::String::fromUTF8("\xF0\x9F\x8E\xB6 Playlist"), juce::dontSendNotification);
	playlistTitleLabel.setFont(juce::Font(20.0f, juce::Font::bold));
	playlistTitleLabel.setJustificationType(juce::Justification::centred);
	playlistTitleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
	addAndMakeVisible(playlistTitleLabel);

	// Create Playlist ListBox
	playlistBox = std::make_unique<PlaylistListBox>(*this);
	addAndMakeVisible(playlistBox.get());

	// Label set up
	for (auto& [msg, textLabel] : labels) {
		textLabel->setText(msg, juce::dontSendNotification);
		textLabel->setFont(juce::Font(14.0f, juce::Font::bold));
		textLabel->setJustificationType(juce::Justification::centredLeft);
		textLabel->setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.9f));
		textLabel->setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);
		addAndMakeVisible(textLabel);
	}

	// Volume Slider setup
	volumeSlider.setRange(0.0, 1.0, 0.01);//نطاق الصوت من 0 صامت لـ 1 أعلى صوت
	volumeSlider.setValue(0.5);//الصوت الافتراضي 50
	volumeSlider.addListener(this);//تحرك السلايدر
	addAndMakeVisible(volumeSlider);

	// Speed Slider setup
	// Speed Slider 
	speedSlider.setSliderStyle(juce::Slider::LinearVertical);
	speedSlider.setRange(0.1, 2.0, 0.01);
	speedSlider.setValue(1.0);
	speedSlider.addListener(this);
	speedSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
	speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(200, 100, 255));
	speedSlider.setColour(juce::Slider::trackColourId, juce::Colour::fromRGB(100, 200, 255));
	speedSlider.setColour(juce::Slider::backgroundColourId, juce::Colour::fromRGBA(50, 50, 80, 100));

	speedSlider.textFromValueFunction = [](double value)
	{
		return juce::String(value, 2) + "x";
	};

	speedSlider.valueFromTextFunction = [](const juce::String& text)
	{
		return text.upToFirstOccurrenceOf("x", false, false).getDoubleValue();
	};

	addAndMakeVisible(speedSlider);


	// Settings Button setup
	addAndMakeVisible(settingsButton);
	settingsButton.addListener(this);
	settingsButton.setButtonText(juce::String::fromUTF8("\xE2\x9A\x99")); // أيقونة الترس (Gear)

	// استخدمنا نفس إعدادات الشفافية ومنع الإطار المزعج (زي زر Mute)
	settingsButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
	settingsButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
	settingsButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white.withAlpha(0.8f));
	settingsButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);

	// هذا السطر مهم جداً لمنع ظهور الإطار الخارجي
	settingsButton.setWantsKeyboardFocus(false);

	// Speed Label
	// سلايدر السرعة
	speedLabel.setText("Speed", juce::dontSendNotification);
	speedLabel.setFont(juce::Font(18.0f, juce::Font::bold));
	speedLabel.setJustificationType(juce::Justification::centred);
	speedLabel.setColour(juce::Label::textColourId, juce::Colours::white);
	addAndMakeVisible(speedLabel);

	// Position Slider setup
	positionSlider.setRange(0.0, 1.0, 0.01);//range
	positionSlider.setValue(0.0);
	positionSlider.addListener(this);
	positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);// أفقي
	positionSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 25);
	positionSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);//المقبض أبيض
	positionSlider.setColour(juce::Slider::trackColourId, juce::Colour::fromRGB(100, 200, 255));//المسار أزرق فاتح
	positionSlider.setColour(juce::Slider::backgroundColourId, juce::Colour::fromRGBA(255, 255, 255, 50));//الخلفية أبيض شفاف 
	positionSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
	positionSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB(60, 40, 100).withAlpha(0.7f));//صندوق النص بنفسجي شفاف + حواف بنفسجية
	positionSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromRGB(100, 80, 150));

	// Custom text display for position slider (show time in mm:ss format with "Time:" prefix)
	positionSlider.textFromValueFunction = [this](double value) -> juce::String {
		int totalSeconds = static_cast<int>(value);
		int minutes = totalSeconds / 60;
		int seconds = totalSeconds % 60;
		return "Time: " + juce::String::formatted("%d:%02d", minutes, seconds);
		};

	addAndMakeVisible(positionSlider);
	if (control)
		control->setSignalListener(this); 

	addAndMakeVisible(playlistSelector);
	playlistSelector.clear(juce::dontSendNotification);

	auto& manager = control->getPlaylistManager();
	auto playlistNames = manager.getPlaylistNames();

	int id = 1;
	for (const auto& name : playlistNames)
	{
		playlistSelector.addItem(name, id++);
	}

	if (!playlistNames.empty())
		playlistSelector.setSelectedId(1, juce::dontSendNotification);

	playlistSelector.setColour(juce::ComboBox::backgroundColourId, juce::Colour::fromRGB(60, 50, 100));
	playlistSelector.setColour(juce::ComboBox::textColourId, juce::Colours::white);
	playlistSelector.setColour(juce::ComboBox::arrowColourId, juce::Colours::white);
	playlistSelector.setColour(juce::ComboBox::outlineColourId, juce::Colour::fromRGB(120, 100, 180));
	playlistSelector.setVisible(showPlaylistPanel);

	playlistSelector.onChange = [this]() {
		auto selectedName = playlistSelector.getText().toStdString();
		if (!selectedName.empty())
		{
			control->getPlaylistManager().setActivePlaylist(selectedName);
			refreshPlaylist();
			refreshPlaylistSelector();
		}
	};

	auto setupButton = [this](juce::TextButton& button,
		const juce::String& text,
		std::function<void()> onClickLambda)
		{
			button.setButtonText(text);
			button.onClick = onClickLambda;
			addAndMakeVisible(button);
			button.setVisible(false);
		};

	setupButton(removeMarker, "Remove Marker", [this]()
		{
			int idx = Markers.getSelectedId() - 1;
			if (idx >= 0 && idx < markers.size())
			{
				control->removeMarker(idx);
				markers = control->getMarkers();
				Markers.clear();
				for (int i = 0; i < markers.size(); ++i)
					Markers.addItem(markers[i].label, i + 1);
				repaint(waveformArea);
				repaint();
			}
		});

	setupButton(setA, "Set A", [this]()
		{
			int idx = Markers.getSelectedId() - 1;
			if (idx >= 0 && idx < markers.size())
			{
				markerA = idx;
				control->setMarkerA(idx);
				markers = control->getMarkers();
				repaint(waveformArea);
				repaint();
			}
		});

	setupButton(setB, "Set B", [this]()
		{
			int idx = Markers.getSelectedId() - 1;
			if (idx >= 0 && idx < markers.size())
			{
				markerB = idx;
				control->setMarkerB(idx);
				markers = control->getMarkers();
				repaint(waveformArea);
				repaint();
			}
		});

	setupButton(StartLooping, "Start Looping", [this]()
		{
			control->toggleLoopAB();
			if (control->isMarkerLooping()) {
				StartLooping.setButtonText("Stop Looping");
			}
			else {
				StartLooping.setButtonText("Start Looping");
			}
		});

	setupButton(goToMarker, "Go To", [this]()
		{
			int idx = Markers.getSelectedId() - 1;
			if (idx >= 0 && idx < markers.size())
				control->goToMarker(markers[idx].position);
		});

	removeMarker.setVisible(false);
	setA.setVisible(false);
	setB.setVisible(false);
	StartLooping.setVisible(false);
	goToMarker.setVisible(false);

	addAndMakeVisible(Markers);
	Markers.setEditableText(false);
	Markers.setJustificationType(juce::Justification::centredLeft);
	Markers.setScrollWheelEnabled(true);

	addAndMakeVisible(labelEditor);
	labelEditor.setText("");
	labelEditor.setInputRestrictions(50);
	labelEditor.setVisible(false);

	labelEditor.onReturnKey = [this]()
	{
		Labels = labelEditor.getText();
		int idx = Markers.getSelectedId() - 1;
		if (idx >= 0 && idx < markers.size())
		{
			markers[idx].label = Labels;
			Markers.clear();
			for (int i = 0; i < markers.size(); ++i)
				Markers.addItem(markers[i].label, i + 1);

			if (control != nullptr)
				control->updateMarker(markers);

			repaint(MarkerPanelArea);
		}
	};

	Markers.onChange = [this]()
		{
			int idx = Markers.getSelectedId() - 1;
			if (idx >= 0 && idx < markers.size())
			{
				Labels = markers[idx].label;
				labelEditor.setText(Labels, juce::dontSendNotification);
				labelEditor.setVisible(true);

				int editorWidth = 150;
				int editorHeight = 25;
				labelEditor.setBounds(
					MarkerPanelArea.getX() + 10,
					MarkerPanelArea.getBottom() - editorHeight - 5,
					editorWidth,
					editorHeight
				);
			}
			else
			{
				labelEditor.setVisible(false);
			}
		};

	setSize(1400, 600);//حجم الشاشه 
}

PlayerGUI::PlayerGUI(PlayerAudio& control, juce::String url, juce::String fileName)
	: control(&control),
	thumbnail(512, this->control->getFormatManager(), this->control->getThumbnailCache()),
	showPlaylistPanel(false), isUserDraggingPosition(false),
	stoped(true), muted(false), lastVal(0.5), currentPlaylistIndex(-1)
{
	this->control->setSignalListener(this);

	initializeControls();
	thumbnail.addChangeListener(this);
	startTimerHz(30);

	if (url.isNotEmpty() && (url.startsWithIgnoreCase("http://") || url.startsWithIgnoreCase("https://"))) {
		this->control->startNewFromURL(juce::URL(url));
		positionSlider.setRange(0.0, this->control->getLength(), 0.01);
		positionSlider.setValue(0.0, juce::dontSendNotification);

		if (this->control->audioExist()) {
			playButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
			stoped = false;
		}
	}
	if (fileName.isNotEmpty()) {
		this->control->startNewFromFile(juce::File(fileName));
		positionSlider.setRange(0.0, this->control->getLength(), 0.01);
		positionSlider.setValue(0.0, juce::dontSendNotification);

		if (this->control->audioExist()) {
			playButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
			stoped = false;
		}
	}
	refreshPlaylist();
	refreshPlaylistSelector();
}

void PlayerGUI::loadSong(juce::String source) {
	if (source.isNotEmpty() && (source.startsWithIgnoreCase("http://") || source.startsWithIgnoreCase("https://"))) {
		source = control->startNewFromURL(juce::URL(source));
	}

	markers.clear();
	Markers.clear();
	control->updateMarker(markers);

	if (source.isNotEmpty()) {
		auto file = juce::File(source);
		control->startNewFromFile(file);
		thumbnail.setSource(new juce::FileInputSource(file));

		positionSlider.setRange(0.0, control->getLength(), 0.01);
		positionSlider.setValue(0.0, juce::dontSendNotification);

		playButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
		stoped = false;
	}
	resized();
	repaint();
}

void PlayerGUI::loadPlaylist(juce::String name) {
	control->getPlaylistManager().setActivePlaylist(name.toStdString());
	refreshPlaylist();
	refreshPlaylistSelector();
	resized();
	repaint();
}


void PlayerGUI::paint(juce::Graphics& g) {
	// Modern gradient background
	juce::ColourGradient gradient(
		juce::Colour::fromRGB(50, 20, 80),
		0.0f, 0.0f,
		juce::Colour::fromRGB(80, 120, 180),
		static_cast<float>(getWidth()),
		static_cast<float>(getHeight()),
		false
	);
	g.setGradientFill(gradient);
	g.fillRect(getLocalBounds());
	// Draw Playlist Panel if visible
	if (showPlaylistPanel)
	{
		g.setColour(juce::Colour::fromRGB(40, 20, 70).withAlpha(0.95f));
		g.fillRoundedRectangle(playlistPanelArea.toFloat(), 15.0f);
	}
	// Draw waveform area
	//حلوه اوى و جميلا لا تلعب
	g.setColour(juce::Colour::fromRGB(150, 50, 200).withAlpha(0.6f));
	g.fillRoundedRectangle(waveformArea.toFloat(), 25.0f);
	// Draw waveform
	//رسم الموجة الصوتية نفسها
	if (thumbnail.getTotalLength() > 0.0)
	{
		g.setColour(juce::Colour::fromRGB(100, 200, 255));
		thumbnail.drawChannel(g,
			waveformArea.reduced(10),
			0.0,
			thumbnail.getTotalLength(),
			0,
			1.0f);
	}
	else
	{
		g.setColour(juce::Colours::white.withAlpha(0.5f));
		g.drawText("No Track Loaded...", waveformArea, juce::Justification::centred);
	}
	// Draw playhead
	//الخط الاحمر 
	if (control != nullptr && control->isPlaying() && thumbnail.getTotalLength() > 0.0)
	{
		double position = control->getAudioPosition();
		double totalLength = thumbnail.getTotalLength();
		float playheadX = static_cast<float>(juce::jmap(position, 0.0, totalLength,// ← دالة بتحول الموضع من (0 → طول الأغنية) لـ (بداية المنطقة → نهاية المنطقة)
			static_cast<double>(waveformArea.getX() + 10),
			static_cast<double>(waveformArea.getRight() - 10)));
		g.setColour(juce::Colours::red);
		g.drawVerticalLine(static_cast<int>(playheadX),
			static_cast<float>(waveformArea.getY() + 10),
			static_cast<float>(waveformArea.getBottom() - 10));
	}
	// Draw dots pattern
	g.setColour(juce::Colours::black.withAlpha(0.3f));
	int dotSpacing = 20;
	int dotSize = 8;
	int verticalSpacing = 15;
	auto sliderBounds = positionSlider.getBounds();
	// First row (closest to slider)
	int dotCenterY_1 = sliderBounds.getY() - 10;
	int dotTopY_1 = dotCenterY_1 - (dotSize / 2);
	for (int x = sliderBounds.getX(); x < sliderBounds.getRight(); x += dotSpacing)
	{
		g.fillEllipse(static_cast<float>(x - dotSize / 2),
			static_cast<float>(dotTopY_1),
			static_cast<float>(dotSize),
			static_cast<float>(dotSize));
	}
	// Second row (above first row)
	int dotTopY_2 = dotTopY_1 - verticalSpacing;
	for (int x = sliderBounds.getX(); x < sliderBounds.getRight(); x += dotSpacing)
	{
		g.fillEllipse(static_cast<float>(x - dotSize / 2),
			static_cast<float>(dotTopY_2),
			static_cast<float>(dotSize),
			static_cast<float>(dotSize));
	}
	// Draw audio picture box
	if (headerBoxArea.getWidth() > 0 && headerBoxArea.getHeight() > 0) {
		juce::ColourGradient headerGradient(
			juce::Colour::fromRGB(120, 70, 200).withAlpha(0.8f),
			headerBoxArea.getTopLeft().toFloat(),
			juce::Colour::fromRGB(180, 120, 250).withAlpha(0.9f),
			headerBoxArea.getBottomRight().toFloat(),
			false
		);
		g.setGradientFill(headerGradient);
		g.fillRoundedRectangle(headerBoxArea.toFloat(), 20.0f);

		g.setColour(juce::Colours::white.withAlpha(0.9f));
		g.setFont(juce::Font((float)headerBoxArea.getHeight() * 0.6f, juce::Font::bold));
		g.drawText(juce::String::fromUTF8("🎵"), headerBoxArea, juce::Justification::centred, true);
	}
	// Draw background circle for volume control
	auto volumeCenter = volumeSlider.getBounds().toFloat().getCentre();
	float backgroundRadius = 47.0f;
	g.setColour(juce::Colour::fromRGB(25, 50, 95));
	g.fillEllipse(volumeCenter.x - backgroundRadius, volumeCenter.y - backgroundRadius,
	backgroundRadius * 2.0f, backgroundRadius * 2.0f);
	// Mute button background - match the circle color
	auto muteBtnBounds = muteButton.getBounds().toFloat();
	g.setColour(juce::Colour::fromRGB(25, 50, 95)); // نفس لون الدائرة
	g.fillEllipse(muteBtnBounds.getX(), muteBtnBounds.getY(),
	muteBtnBounds.getWidth(), muteBtnBounds.getHeight());
	// Draw rounded rectangle background for control buttons
	auto buttonsBg = controlButtonsArea.toFloat();
	g.setColour(juce::Colour::fromRGB(80, 60, 120).withAlpha(0.7f));
	g.fillRoundedRectangle(buttonsBg, 30.0f);
	// Draw individual button backgrounds
	std::vector<juce::TextButton*> controlBtns = {
	&stopButton, &playButton, &restartButton, &backwardButton, &forwardButton, &go_to_endButton, &repeatButton, &go_to_startButton
	};
	for (auto* btn : controlBtns)
	{
		if (btn->getWidth() > 0)
		{
			auto btnBounds = btn->getBounds().toFloat();
			g.setColour(juce::Colour::fromRGB(100, 120, 180).withAlpha(0.6f));
			g.fillRoundedRectangle(btnBounds, 10.0f);
		}
	}

	if (MarkerPanelArea.getWidth() > 0 && MarkerPanelArea.getHeight() > 0)
	{
		g.setColour(juce::Colour::fromRGB(60, 40, 120).withAlpha(0.8f));
		g.fillRoundedRectangle(MarkerPanelArea.toFloat(), 10.0f);
	}

	if (!markers.empty() && control != nullptr)
	{
		double totalLength = thumbnail.getTotalLength();
		if (totalLength <= 0.0)
			return;

		for (int i = 0; i < markers.size(); ++i)
		{
			auto& m = markers[i];

			int x = waveformArea.getX() + (int)((m.position / totalLength) * waveformArea.getWidth());

			if (m.type == 1) g.setColour(juce::Colours::green);
			else if (m.type == 2) g.setColour(juce::Colours::red);
			else g.setColour(juce::Colours::yellow);

			g.drawLine((float)x, (float)waveformArea.getY(), (float)x, (float)waveformArea.getBottom(), 2.0f);

			g.setColour(juce::Colours::white);
			g.setFont(12.0f);
			int labelHeight = 20;
			g.drawText(
				m.label,
				x - 50,
				waveformArea.getY() - 40,
				100,
				labelHeight,
				juce::Justification::centred
			);
		}

		if (markerA >= 0 && markerB >= 0 && markerA < markers.size() && markerB < markers.size())
		{
			int x1 = waveformArea.getX() + (int)((markers[markerA].position / totalLength) * waveformArea.getWidth());
			int x2 = waveformArea.getX() + (int)((markers[markerB].position / totalLength) * waveformArea.getWidth());

			g.setColour(juce::Colours::blue.withAlpha(0.3f));
			g.fillRect(x1, waveformArea.getY(), x2 - x1, waveformArea.getHeight());
		}
	}
}

void PlayerGUI::resized()
{
	auto bounds = getLocalBounds();
	auto mainArea = bounds;

	// ================= Playlist Panel =================
	if (showPlaylistPanel)
	{
		playlistPanelArea = bounds.removeFromRight(bounds.getWidth() * 0.14);
		auto playlistBounds = playlistPanelArea.reduced(15);
		mainArea = bounds;

		playlistTitleLabel.setBounds(playlistBounds.removeFromTop(40));
		playlistBounds.removeFromTop(10);

		auto buttonRow1 = playlistBounds.removeFromTop(35);
		addToPlaylistButton.setBounds(buttonRow1.removeFromLeft(buttonRow1.getWidth() / 2 - 5));
		buttonRow1.removeFromLeft(10);
		removeFromPlaylistButton.setBounds(buttonRow1);

		playlistBounds.removeFromTop(10);
		auto buttonRow2 = playlistBounds.removeFromTop(35);
		savePlaylistButton.setBounds(buttonRow2.removeFromLeft(buttonRow2.getWidth() / 2 - 5));
		buttonRow2.removeFromLeft(10);
		addNewPlaylistButton.setBounds(buttonRow2);

		playlistBounds.removeFromTop(10);

		playlistSelector.setBounds(playlistBounds.removeFromTop(35));
		playlistBox->setBounds(playlistBounds);
	}

	// ================= Right Panel =================
	auto rightPanel = mainArea.removeFromRight(180);
	rightPanel.removeFromTop(30);
	auto volumeControlArea = rightPanel.removeFromTop(180);
	auto centeredArea = volumeControlArea.withSizeKeepingCentre(170, 170);
	volumeSlider.setBounds(centeredArea);

	auto muteArea = centeredArea.withSizeKeepingCentre(60, 60);
	muteButton.setBounds(muteArea);

	// ================= Left Panel =================
	auto leftPanel = mainArea.removeFromLeft(80);
	leftPanel.removeFromTop(30);
	auto settingsArea = leftPanel.removeFromTop(45);
	settingsButton.setBounds(settingsArea.withSizeKeepingCentre(40, 40));
	leftPanel.removeFromTop(5);

	auto sliderArea = leftPanel.removeFromTop(leftPanel.getHeight() - 60);
	speedSlider.setBounds(sliderArea.reduced(20, 40));
	speedLabel.setBounds(leftPanel.removeFromTop(40).reduced(5));

	mainArea.removeFromTop(10);

	// ================= Control Buttons =================
	controlButtonsArea = mainArea.removeFromBottom(65).reduced(120, 5);

	int infoHeight = 50;
	int infoMargin = 5;
	auto infoY = controlButtonsArea.getY() - infoMargin - infoHeight;
	auto infoArea = juce::Rectangle<int>(
		controlButtonsArea.getX(),
		infoY,
		controlButtonsArea.getWidth(),
		infoHeight
	);

	int sliderHeight = 25;
	int sliderMargin = 5;
	int positionY = infoArea.getY() - sliderMargin - sliderHeight;
	positionSlider.setBounds(controlButtonsArea.getX(), positionY, controlButtonsArea.getWidth(), sliderHeight);

	// ================= Waveform Area =================
	int waveformHeight = 200;
	int spaceForDots = 50;
	int waveformY_new = positionY - spaceForDots - waveformHeight;

	auto originalWaveformBounds = mainArea.reduced(30, 30);
	waveformArea = juce::Rectangle<int>(
		originalWaveformBounds.getX(),
		waveformY_new,
		originalWaveformBounds.getWidth(),
		waveformHeight
	);

	// ================= Marker Panel Above Waveform =================
	if (thumbnail.getTotalLength() > 0.0)
	{
		// Determine Marker Panel size above waveform
		int markerPanelHeight = juce::jmin(80, waveformArea.getHeight() / 3);
		MarkerPanelArea = juce::Rectangle<int>(
			waveformArea.getX(),
			waveformArea.getY() - markerPanelHeight - 10,
			waveformArea.getWidth(),
			markerPanelHeight
		);

		// Layout buttons
		int buttonHeight = 28;
		int buttonSpacing = 10;
		int totalButtonWidth = MarkerPanelArea.getWidth() - ((6 + 1) * buttonSpacing);
		int buttonWidth = totalButtonWidth / 6;

		int currentX = MarkerPanelArea.getX() + buttonSpacing;
		int y = MarkerPanelArea.getY() + 10;

		std::vector<juce::TextButton*> markerButtons = {
			&removeMarker, &setA, &setB, &StartLooping, &goToMarker
		};

		for (auto* btn : markerButtons)
		{
			btn->setBounds(currentX, y, buttonWidth, buttonHeight);
			btn->setVisible(true);
			currentX += buttonWidth + buttonSpacing;
		}

		// Layout combo box
		int comboHeight = 25;
		int comboWidth = 200; // adjust to fit
		int comboX = MarkerPanelArea.getRight() - comboWidth - 10;
		int comboY = MarkerPanelArea.getY() + 10;

		Markers.setBounds(comboX, comboY, comboWidth, comboHeight);
		Markers.setVisible(true);
	}
	else
	{
		// No markers, hide panel and controls
		MarkerPanelArea = juce::Rectangle<int>(0, 0, 0, 0);

		removeMarker.setVisible(false);
		setA.setVisible(false);
		setB.setVisible(false);
		StartLooping.setVisible(false);
		goToMarker.setVisible(false);
		Markers.setVisible(false);
	}

	// ================= Header Box =================
	auto topEmptySpaceBounds = mainArea.reduced(20);
	int headerBoxWidth = juce::jmin(450, mainArea.getWidth() - 20);
	int headerBoxHeight = juce::jmin(350, mainArea.getHeight() - 20);
	int headerBoxX = mainArea.getCentreX() - (headerBoxWidth / 2);
	int headerBoxY = mainArea.getY() + 10;
	headerBoxArea = juce::Rectangle<int>(headerBoxX, headerBoxY, headerBoxWidth, headerBoxHeight);

	// ================= Info & Author =================
	auto leftInfo = infoArea.removeFromLeft(infoArea.getWidth() / 2);
	auto rightInfo = infoArea;

	auto authorRow = leftInfo.removeFromTop(25);
	author.setBounds(authorRow.removeFromLeft(80));
	name.setBounds(authorRow);

	auto titleRow = rightInfo.removeFromTop(25);
	songTitle.setBounds(titleRow.removeFromLeft(80));
	title.setBounds(titleRow);

	duration.setBounds(leftInfo.removeFromTop(25));
	durationHeader.setBounds(0, 0, 0, 0);
	loadButton.setBounds(0, 0, 0, 0);

	// ================= Control Buttons =================
	auto buttonRow = controlButtonsArea.reduced(15, 6);
	std::vector<juce::TextButton*> controlBtnsInOrder = {
		&stopButton, &go_to_startButton, &backwardButton, &playButton,
		&forwardButton, &go_to_endButton, &restartButton, &repeatButton
	};

	int numBtns = (int)controlBtnsInOrder.size();
	int buttonHeight2 = buttonRow.getHeight();
	int totalAvailableWidth = buttonRow.getWidth();
	int minSpacing = 5;
	int availableWidthForButtons = totalAvailableWidth - ((numBtns + 1) * minSpacing);
	int buttonWidth2 = availableWidthForButtons / numBtns;
	if (buttonWidth2 < 37) buttonWidth2 = 37;
	if (buttonWidth2 > 70) buttonWidth2 = 70;

	int spacing = (totalAvailableWidth - (numBtns * buttonWidth2)) / (numBtns + 1);
	int currentX2 = buttonRow.getX() + spacing;
	int y2 = buttonRow.getY();
	for (auto* btn : controlBtnsInOrder)
	{
		btn->setBounds(currentX2, y2, buttonWidth2, buttonHeight2);
		currentX2 += buttonWidth2 + spacing;
	}

	// Hide unused buttons
	loadButton.setBounds(0, 0, 0, 0);
	speedButton.setBounds(0, 0, 0, 0);
}

void PlayerGUI::buttonClicked(juce::Button* button) {
	if (control == nullptr && button != &loadButton) return;

	// Playlist buttons
	if (button == &addToPlaylistButton) {
		fileChooser = std::make_unique<juce::FileChooser>(
			"Select audio file(s)...",
			juce::File{},
			"*.wav;*.mp3;*.flac;*.aiff;*.ogg"
		);

		fileChooser->launchAsync(
			juce::FileBrowserComponent::openMode |
			juce::FileBrowserComponent::canSelectFiles |
			juce::FileBrowserComponent::canSelectMultipleItems,

			[this](const juce::FileChooser& fc)
			{
				auto files = fc.getResults();

				if (files.isEmpty())
					return;

				auto& playlist = control->getPlaylistManager();

				for (const auto& file : files)
				{
					if (file.existsAsFile())
					{
						std::string fileName = file.getFileNameWithoutExtension().toStdString();
						std::string filePath = file.getFullPathName().toStdString();

						playlist.add(fileName, filePath);
					}
				}

				refreshPlaylist();
				refreshPlaylistSelector();
			});
		return;
	}
	else if (button == &removeFromPlaylistButton) {
		std::string selectedKey = playlistBox->getSelectedItemKey();
		if (!selectedKey.empty()) {
			control->getPlaylistManager().remove(selectedKey);
			resized();
			repaint();
			repaint(waveformArea);
			refreshPlaylist();
		}
		return;
	}
	else if (button == &savePlaylistButton) {
		control->getPlaylistManager().save("UserPlayList.txt");
		return;
	}
	else if (button == &addNewPlaylistButton)
	{
		auto* popup = new NewPlaylistPopup([this](const juce::String& name)
			{
				if (name.isNotEmpty())
				{
					auto& manager = control->getPlaylistManager();
					manager.createNewPlaylist(name.toStdString());
					manager.setActivePlaylist(name.toStdString());
					refreshPlaylist();
					refreshPlaylistSelector();
				}
			});

		auto popupWidth = 200;
		auto popupHeight = 100;
		auto center = getLocalBounds().getCentre();
		popup->setBounds(center.x - popupWidth / 2, center.y - popupHeight / 2, popupWidth, popupHeight);
		addAndMakeVisible(popup);
	}


	else if (button == &forwardButton)
	{
		if (control->audioExist())
		{
			control->jumpTime(10.0);
			positionSlider.setValue(control->getAudioPosition());
		}
	}
	else if (button == &backwardButton)
	{
		if (control->audioExist())
		{
			control->jumpTime(-10.0);
			positionSlider.setValue(control->getAudioPosition());
		}
	}


	else if (button == &settingsButton)
	{
		juce::PopupMenu settingsMenu;
		settingsMenu.addItem(1, "Load Audio File...");

		if (showPlaylistPanel)
			settingsMenu.addItem(2, "Hide Playlist Panel");
		else
			settingsMenu.addItem(2, "Show Playlist Panel");

		if (twoGUIs)
			settingsMenu.addItem(3, "Close Second Player");
		else
			settingsMenu.addItem(3, "Open Second Player");

		settingsMenu.addItem(4, "Save");
		settingsMenu.addSeparator();
		settingsMenu.addItem(5, "Go to Home Page");
		settingsMenu.addItem(6, "Settings");

		settingsMenu.showMenuAsync(
			juce::PopupMenu::Options().withTargetComponent(settingsButton),
			[this](int result)
			{
				if (result == 0)
					return;

				switch (result)
				{
				case 1:
					buttonClicked(&loadButton);
					resized();
					repaint();
					break;

				case 2:
					showPlaylistPanel = !showPlaylistPanel;
					resized();
					repaint();
					break;

				case 3:
					twoGUIs = !twoGUIs;
					if (listen != nullptr)
						listen->twoGUI();
					resized();
					repaint();
					break;

				case 4:
					save("recent.txt");
					break;

				case 5:
					if (listen != nullptr)
						listen->returnToHomePage();
					break;

				case 6:
					if (listen != nullptr)
						listen->openSettings();
					break;

				default:
					break;
				}
			}
		);
	}

	// Audio control buttons
	if (button == &loadButton) {
		fileChooser = std::make_unique<juce::FileChooser>(
			"Select an audio file...",
			juce::File{},
			"*.wav;*.mp3;*.flac;*.aiff;*.ogg"
		);

		markers.clear();
		Markers.clear();
		control->updateMarker(markers);
		fileChooser->launchAsync(
			juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
			[this](const juce::FileChooser& fc) {
				auto file = fc.getResult();
				if (file.existsAsFile()) {
					control->startNewFromFile(file);
					thumbnail.setSource(new juce::FileInputSource(file));

					positionSlider.setRange(0.0, control->getLength(), 0.01);
					positionSlider.setValue(0.0, juce::dontSendNotification);

					playlistBox->deselectAll();

					if (control->audioExist()) {
						playButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
						stoped = false;
					}
				}
			});
	}
	else if (button == &restartButton) {
		if (control->audioExist()) {
			control->restart();
		}
	}
	else if (button == &stopButton) {
		if (control->audioExist()) {
			control->stop();
			control->setPosition(0.0);
			positionSlider.setValue(0.0);
			stoped = true;
			playButton.setButtonText(juce::String::fromUTF8("\xE2\x96\xB6"));
			repaint(waveformArea);
		}
	}
	else if (button == &playButton) {
		if (!control->audioExist()) return;
		if (stoped) {
			stoped = false;
			playButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
			if (control->reachEnd()) control->restart();
			else control->start();
		}
		else {
			stoped = true;
			playButton.setButtonText(juce::String::fromUTF8("\xE2\x96\xB6"));
			control->stop();
		}
	}
	else if (button == &muteButton)
	{
		if (control->audioExist())
		{
			if (muted)
			{
				muted = false;
				volumeSlider.setValue(lastVal);
				control->setGain(lastVal);
				muteButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x8A"));
			}
			else
			{
				if (volumeSlider.getValue() > 0.001) {
					lastVal = volumeSlider.getValue();
				}
				muted = true;
				volumeSlider.setValue(0.0);
				control->setGain(0.0);
				muteButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x87"));
			}
		}
		repaint();
	}
	else if (button == &go_to_startButton) {
		if (control->audioExist()) {
			if (currentPlaylistIndex > 0) {
				playTrackFromPlaylist(currentPlaylistIndex - 1);
				return;
			}
			control->setPosition(0.0);
			positionSlider.setValue(0.0);
			repaint(waveformArea);
		}
	}
	else if (button == &go_to_endButton) {
		if (control->audioExist()) {
			control->setPosition(control->getLength());
			positionSlider.setValue(control->getLength());
			repaint(waveformArea);
		}
	}
	else if (button == &repeatButton)
	{
		if (!control->audioExist()) return;
		control->toggleLooping();
		if (control->isLooping() == 0)
		{
			repeatButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x84"));
		}
		else if (control->isLooping() == 1)
		{
			repeatButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x82"));
		}
		else if (control->isLooping() == 2) {
			repeatButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x81"));
		}
		else {
			repeatButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x80"));
		}
	}
}

void PlayerGUI::timerCallback()
{
	if (control == nullptr)
		return; 

	if (control->isPlaying() && !isUserDraggingPosition)
	{
		double pos = control->getAudioPosition();
		positionSlider.setValue(pos, juce::dontSendNotification);
		repaint(waveformArea);

		if (control->reachEnd())
			playBackFinished();
	}

	if (control->isMarkerLooping()) {
		double pos = control->getAudioPosition();
		if (pos > std::max(markers[control->getMarkerA()].position, markers[control->getMarkerB()].position)) {
			control->setPosition(std::min(markers[control->getMarkerA()].position, markers[control->getMarkerB()].position));
			positionSlider.setValue(control->getAudioPosition(), juce::dontSendNotification);
			repaint(waveformArea);
		}
		if (pos < std::min(markers[control->getMarkerA()].position, markers[control->getMarkerB()].position)) {
			control->setPosition(std::min(markers[control->getMarkerA()].position, markers[control->getMarkerB()].position));
			positionSlider.setValue(control->getAudioPosition(), juce::dontSendNotification);
			repaint(waveformArea);
		}
	}
}

void PlayerGUI::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	if (source == &thumbnail)
	{
		double newLength = control->getLength();

		if (newLength > 0.0 && newLength != positionSlider.getMaximum())
		{
			positionSlider.setRange(0.0, newLength, 0.01);
			positionSlider.setValue(0.0, juce::dontSendNotification);

			loadMetaData();
		}
		repaint(waveformArea);
	}
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider) {
	if (control == nullptr) return;

	if (slider == &volumeSlider)
		control->setGain(slider->getValue());
	else if (slider == &speedSlider)
		control->setSpeed(slider->getValue());
}

void PlayerGUI::sliderDragStarted(juce::Slider* slider)
{
	if (slider == &positionSlider)
	{
		isUserDraggingPosition = true;
	}
}

void PlayerGUI::sliderDragEnded(juce::Slider* slider) {
	if (control == nullptr) return;

	if (slider == &volumeSlider) {
		if (slider->getValue() < 1e-8) {
			muted = true;
			muteButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x87"));
			return;
		}
		muted = false;
		muteButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x8A"));
		lastVal = slider->getValue();
	}
	else if (slider == &positionSlider)
	{
		if (control->audioExist())
		{
			control->setPosition(slider->getValue());
			if (control->reachEnd()) {
				playBackFinished();
			}
		}
		isUserDraggingPosition = false;
	}
}

void PlayerGUI::playBackFinished()
{
	playButton.setButtonText(juce::String::fromUTF8("\xE2\x96\xB6"));
	stoped = true;

	if (control == nullptr) return;

	int mode = control->isLooping();

	if (mode == 1 && control->reachEnd())
	{
		control->restart();
		playButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x82"));
		stoped = false;
		return;
	}
	else if (mode == 2 && control->reachEnd())
	{
		auto& manager = control->getPlaylistManager();
		std::string activePlaylistName = manager.getActivePlaylist();

		if (!activePlaylistName.empty())
		{
			auto songs = manager.getSongs(activePlaylistName);
			if (songs.size() > 0)
			{
				int nextIndex = (currentPlaylistIndex + 1) % songs.size();

				playTrackFromPlaylist(nextIndex);

				repeatButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x81"));
				return;
			}
		}
	}
	else if (mode == 3 && control->reachEnd())
	{
		auto& manager = control->getPlaylistManager();
		std::string activePlaylistName = manager.getActivePlaylist();

		if (!activePlaylistName.empty())
		{
			auto songs = manager.getSongs(activePlaylistName);
			if (songs.size() > 0)
			{
				int nextIndex = std::rand() % songs.size();

				playTrackFromPlaylist(nextIndex);

				repeatButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x80"));
				return;
			}
		}
	}
}

void PlayerGUI::loadMetaData()
{
	name.setText(control->getName(), juce::dontSendNotification);
	title.setText(control->getTitle(), juce::dontSendNotification);
	duration.setText(control->getDuration(), juce::dontSendNotification);
}

void PlayerGUI::loadWave(juce::File file)
{
	thumbnail.clear();
	thumbnail.setSource(new juce::FileInputSource(file));
	repaint(waveformArea);
	repaint();
	resized();
}

void PlayerGUI::refreshPlaylist()
{
	if (control == nullptr) return;

	auto& manager = control->getPlaylistManager();
	std::string activePlaylistName = manager.getActivePlaylist();

	if (activePlaylistName.empty())
	{
		playlistBox->updatePlaylist({});
		return;
	}

	auto songs = manager.getSongs(activePlaylistName);
	playlistBox->updatePlaylist(songs);
}

void PlayerGUI::playTrackFromPlaylist(int index)
{
	if (control == nullptr) return;

	markers.clear();
	Markers.clear();
	control->updateMarker(markers);

	auto& manager = control->getPlaylistManager();
	std::string activePlaylistName = manager.getActivePlaylist();
	if (activePlaylistName.empty())
		return;

	auto songs = manager.getSongs(activePlaylistName);

	if (index < 0 || index >= songs.size()) return;

	std::string filePath = songs[index].second;
	if (filePath.find("http://") != std::string::npos || filePath.find("https://") != std::string::npos) filePath = control->startNewFromURL(juce::URL(filePath)).toStdString();

	juce::File file(filePath);

	if (file.existsAsFile())
	{
		control->startNewFromFile(file);
		thumbnail.setSource(new juce::FileInputSource(file));
		loadMetaData();

		currentPlaylistIndex = index;
		currentPlaylistKey = songs[index].first;
		playlistBox->setCurrentPlayingIndex(index);

		positionSlider.setRange(0.0, control->getLength(), 0.01);
		positionSlider.setValue(0.0, juce::dontSendNotification);

		playButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
		stoped = false;
	}
	resized();
	repaint();
}
// لما ادوس على ال wave يتغير ال position 
void PlayerGUI::mouseDown(const juce::MouseEvent& event)
{
	if (control == nullptr || !control->audioExist() || thumbnail.getTotalLength() <= 0.0)
	{
		juce::Component::mouseDown(event);
		return;
	}

	auto clickableWaveformArea = waveformArea.reduced(10);
	if (!clickableWaveformArea.contains(event.getPosition()))
	{
		juce::Component::mouseDown(event);
		return;
	}

	// ================= Right click =================
	if (event.mods.isRightButtonDown())
	{
		double relativePos = (double)(event.x - waveformArea.getX()) / waveformArea.getWidth();
		double audioPos = relativePos * thumbnail.getTotalLength();

		const double tolerance = 0.05;
		bool tooClose = false;

		for (auto& m : markers)
		{
			if (std::abs(m.position - audioPos) < tolerance * thumbnail.getTotalLength())
			{
				tooClose = true;
				break;
			}
		}

		if (tooClose)
			return;

		juce::String label = "Marker " + juce::String(markers.size() + 1);
		control->addMarker(audioPos, label, 0);

		markers = control->getMarkers();

		Markers.clear();
		for (int i = 0; i < markers.size(); ++i)
			Markers.addItem(markers[i].label, (int)(i + 1));

		repaint(MarkerPanelArea);
		return;
	}

	// ================= Left click =================
	isUserDraggingPosition = true;
	mouseDrag(event);
}

void PlayerGUI::mouseDrag(const juce::MouseEvent& event)
{
	if (isUserDraggingPosition)
	{
		auto clickableWaveformArea = waveformArea.reduced(10);

		int mouseX = event.getPosition().getX();

		double totalLength = thumbnail.getTotalLength();

		double newPosition = juce::jmap(
			static_cast<double>(mouseX),
			static_cast<double>(clickableWaveformArea.getX()),
			static_cast<double>(clickableWaveformArea.getRight()),
			0.0,
			totalLength
		);

		newPosition = juce::jlimit(0.0, totalLength, newPosition);

		control->setPosition(newPosition);

		positionSlider.setValue(newPosition, juce::dontSendNotification);

		repaint(waveformArea);
	}
	else
	{
		juce::Component::mouseDrag(event);
	}
}


void PlayerGUI::mouseUp(const juce::MouseEvent& event)
{
	if (isUserDraggingPosition)
	{
		isUserDraggingPosition = false;

		if (control->reachEnd()) {
			playBackFinished();
		}
	}
	else
	{
		juce::Component::mouseUp(event);
	}
}

juce::Slider& PlayerGUI::getVolume()
{
	return volumeSlider;
}

juce::Slider& PlayerGUI::getPosition()
{
	return positionSlider;
}

void PlayerGUI::reset()
{
	if (control)
	{
		control->stop();
		control->setPosition(0.0);
		control->reset();

		if (control->isLooping())
		{
			control->toggleLooping();
			repeatButton.setButtonText(juce::String::fromUTF8("\xE2\x86\xBB"));
		}
	}

	markers.clear();
	Markers.clear();
	control->updateMarker(markers);

	thumbnail.clear();
	showPlaylistPanel = false;

	positionSlider.setRange(0.0, 1.0, 0.01);
	positionSlider.setValue(0.0, juce::dontSendNotification);
	volumeSlider.setValue(0.5, juce::dontSendNotification);
	speedSlider.setValue(1.0, juce::dontSendNotification);

	name.setText("", juce::dontSendNotification);
	title.setText("", juce::dontSendNotification);
	duration.setText("", juce::dontSendNotification);

	stoped = true;
	playButton.setButtonText(juce::String::fromUTF8("\xE2\x96\xB6"));

	muted = false;
	lastVal = 0.5;
	muteButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x8A"));

	currentPlaylistIndex = -1;
	currentPlaylistKey = "";
	if (playlistBox)
	{
		playlistBox->deselectAll();
		playlistBox->setCurrentPlayingIndex(-1);
	}

	repaint();
	resized();
}

void PlayerGUI::refreshPlaylistSelector()
{
	if (!control) return;

	playlistSelector.clear(juce::dontSendNotification);

	auto& manager = control->getPlaylistManager();
	auto playlistNames = manager.getPlaylistNames();

	int id = 1;
	for (const auto& name : playlistNames)
	{
		playlistSelector.addItem(name, id++);
	}

	auto activePlaylist = manager.getActivePlaylist();
	if (!activePlaylist.empty())
	{
		for (int i = 1; i <= playlistSelector.getNumItems(); ++i)
		{
			if (playlistSelector.getItemText(i - 1).toStdString() == activePlaylist)
			{
				playlistSelector.setSelectedId(i, juce::dontSendNotification);
				break;
			}
		}
	}
	else if (!playlistNames.empty())
	{
		playlistSelector.setSelectedId(1, juce::dontSendNotification);
	}
}
// Format  Title|artist|path|url|thumbnailFile|thumbnailUrl|duration|lastVolume|lastPosition|isLooping|activePlaylistName
void PlayerGUI::save(const std::string& path) {
	std::string line;
	std::ifstream in(path);
	if (!in) {
		return;
	}
	std::string result;
	while (std::getline(in,line)) {
		std::string title, artist, path, url, thumbnailFile, thumbnailUrl, duration, lastVolume, lastPosition, isLooping, activePlaylistName;
		if (line.empty()) continue;
		std::string Line = line;
		// titles
		int i = line.find('|');
		title = line.substr(0, i);
		line = line.substr(i + 1);
		// artist
		i = line.find('|');
		artist = line.substr(0, i);
		line = line.substr(i + 1);
		// path
		i = line.find('|');
		path = line.substr(0, i);
		line = line.substr(i + 1);
		// url
		i = line.find('|');
		url = line.substr(0, i);
		line = line.substr(i + 1);
		// thumbnailFile
		i = line.find('|');
		thumbnailFile = line.substr(0, i);
		line = line.substr(i + 1);
		// thumbnailUrl
		i = line.find('|');
		thumbnailUrl = line.substr(0, i);
		line = line.substr(i + 1);
		// duration
		i = line.find('|');
		duration = line.substr(0, i);
		line = line.substr(i + 1);
		// lastVolume
		i = line.find('|');
		lastVolume = line.substr(0, i);
		line = line.substr(i + 1);
		// lastPosition
		i = line.find('|');
		lastPosition = line.substr(0, i);
		line = line.substr(i + 1);
		// isLooping
		i = line.find('|');
		isLooping = line.substr(0, i);
		line = line.substr(i + 1);
		// activePlaylistName
		activePlaylistName = line;

		if (this->control->getFileSource().toStdString() == path) continue;
		result += Line;
	}
	in.close();
	//markers string
	std::string markersStr;
	const auto& markers = control->getMarkers();
	for (int i = 0; i < markers.size(); ++i)
	{
		markersStr += std::to_string(markers[i].position) + ":" +
			std::to_string(markers[i].type) + ":" +
			markers[i].label.toStdString();

		if (i + 1 < markers.size())
			markersStr += ",";
	}
	std::ofstream out(path);
	out << control->getTitle() << '|' << control->getName() << '|' << this->control->getFileSource() << '|' << "" << '|' << "" << '|'
		<< '|' << control->getDuration() << '|' << lastVal << '|' << this->control->getCurrentPosition() << '|' << control->isLooping() << '|'
		<< this->control->getCurrentPlaylistName() << '|' << markersStr << '\n';
	out << result;
	out.close();
}

void PlayerGUI::setAll(float lastVolume, float lastPosition, bool loop, std::vector<Marker> m) {
	volumeSlider.setValue(lastVolume);
	control->setPosition(lastPosition);
	control->setLooping(loop);
	sliderValueChanged(&volumeSlider);
	if (control->isLooping() == 0)
	{
		repeatButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x84"));
	}
	else if(control->isLooping() == 1)
	{
		repeatButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x82"));
	}
	else if(control->isLooping() == 2) {
		repeatButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x81"));
	}
	else {
		repeatButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x80"));
	}
	playButton.setButtonText(juce::String::fromUTF8("\xE2\x96\xB6"));
	stoped = true;
	twoGUIs = false;
	control->stop();
	playlistBox->deselectAll();
	positionSlider.setValue(lastPosition);
	markers = m;
	Markers.clear();
	for (int i = 0; i < markers.size(); ++i)
		Markers.addItem(markers[i].label, (int)(i + 1));
	control->updateMarker(markers);

	resized();
	repaint();
}

void PlayerGUI::updateMarkerLabel()
{
	int idx = Markers.getSelectedId() - 1;
	if (idx < 0 || idx >= markers.size())
		return;

	markers[idx].label = Labels;

	Markers.clear();
	for (int i = 0; i < markers.size(); ++i)
		Markers.addItem(markers[i].label, i + 1);

	if (control != nullptr)
		control->updateMarker(markers);

	repaint(MarkerPanelArea);
}