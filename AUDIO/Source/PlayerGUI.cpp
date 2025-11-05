#include "PlayerGUI.h"
#include <algorithm>
#include <ctime>

void PlayerGUI::initializeControls()
{
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
	restartButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xAE"));
	stopButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xB9"));
	playButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
	muteButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x8A"));
	go_to_endButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xAD"));
	repeatButton.setButtonText(juce::String::fromUTF8("\xE2\x86\xBB"));

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

	addAndMakeVisible(loadPlaylistButton);
	loadPlaylistButton.addListener(this);
	loadPlaylistButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(150, 100, 80));
	loadPlaylistButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);

	// Playlist Title Label
	//ازار ال playlist
	playlistTitleLabel.setText(juce::String::fromUTF8("\xF0\x9F\x8E\xB6 Playlist"), juce::dontSendNotification);
	playlistTitleLabel.setFont(juce::FontOptions(20.0f, juce::Font::bold));
	playlistTitleLabel.setJustificationType(juce::Justification::centred);
	playlistTitleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
	addAndMakeVisible(playlistTitleLabel);

	// Create Playlist ListBox
	playlistBox = std::make_unique<PlaylistListBox>(*this);
	addAndMakeVisible(playlistBox.get());

	// Label set up
	for (auto& [msg, textLabel] : labels) {
		textLabel->setText(msg, juce::dontSendNotification);
		textLabel->setFont(juce::FontOptions(14.0f, juce::Font::bold));
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
	speedSlider.setRange(0.1, 2.0, 0.1);
	speedSlider.setValue(1.0);
	speedSlider.addListener(this);
	speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(200, 100, 255));
	speedSlider.setColour(juce::Slider::trackColourId, juce::Colour::fromRGB(100, 200, 255));
	speedSlider.setColour(juce::Slider::backgroundColourId, juce::Colour::fromRGBA(50, 50, 80, 100));
	speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
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
	speedLabel.setFont(juce::FontOptions(18.0f, juce::Font::bold));
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
	//task 14//////////////////////////////////
	addAndMakeVisible(markerActionBox);
	addAndMakeVisible(markerSelectBox);
	addAndMakeVisible(addMarkerButton);
	addAndMakeVisible(applyMarkerActionButton);

	markerActionBox.addItem("Loop between two markers", 1);
	markerActionBox.addItem("Go to marker", 2);
	markerActionBox.addItem("Remove marker", 3);
	markerActionBox.setSelectedId(1);

	addMarkerButton.addListener(this);
	applyMarkerActionButton.addListener(this);
	//////////////////////////////////////////

	setSize(1400, 600);//حجم الشاشه 
}

PlayerGUI::PlayerGUI(PlayerAudio& control, juce::String url = "", juce::String fileName = "")
	: control(&control),
	thumbnail(512, this->control->getFormatManager(), this->control->getThumbnailCache()),
	showPlaylistPanel(false), isUserDraggingPosition(false),
	stoped(true), muted(false), lastVal(0.5), currentPlaylistIndex(-1)
{
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

	control.setSignalListener(this);

	refreshPlaylist();
}

void PlayerGUI::loadSong(juce::String source) {
	if (source.isNotEmpty() && (source.startsWithIgnoreCase("http://") || source.startsWithIgnoreCase("https://"))) {
		source = control->startNewFromURL(juce::URL(source));
	}

	if (source.isNotEmpty()) {
		auto file = juce::File(source);
		control->startNewFromFile(file);
		thumbnail.setSource(new juce::FileInputSource(file));

		positionSlider.setRange(0.0, control->getLength(), 0.01);
		positionSlider.setValue(0.0, juce::dontSendNotification);

		if (control->audioExist()) {
			playButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
			stoped = false;
		}
	}
}

//يلا نرسم يلاااااااااااااا😍😍😍
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
		
		// 🔹 Draw markers on top of waveform
		for (auto& m : markers)
		{
			float x = (m.position / thumbnail.getTotalLength()) * waveformArea.getWidth() + waveformArea.getX();

			g.setColour(juce::Colours::aqua);
			g.drawLine(x, waveformArea.getY(), x, waveformArea.getBottom(), 2.0f);
			g.setColour(juce::Colours::white);
			g.drawText(m.name, x + 3, waveformArea.getY() + 5, 60, 15, juce::Justification::left);
		}
	}
	else
	{
		g.setColour(juce::Colours::white.withAlpha(0.5f));
		g.drawText("Loading audio...", waveformArea, juce::Justification::centred);
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
	if (headerBoxArea.getWidth() > 0 && headerBoxArea.getHeight() > 0)
	{
		juce::ColourGradient headerGradient(
			juce::Colour::fromRGB(120, 70, 200).withAlpha(0.8f),
			headerBoxArea.getTopLeft().toFloat(),
			juce::Colour::fromRGB(180, 120, 250).withAlpha(0.9f),
			headerBoxArea.getBottomRight().toFloat(),
			false
		);
		g.setGradientFill(headerGradient);
		g.fillRoundedRectangle(headerBoxArea.toFloat(), 20.0f);

		g.setColour(juce::Colours::white.withAlpha(0.7f));
		g.setFont(juce::FontOptions(28.0f, juce::Font::bold));
		g.drawText("pictuer of Audio", headerBoxArea, juce::Justification::centred, true);
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
	&stopButton, &playButton, &restartButton, &backwardButton, &forwardButton, &go_to_endButton, &repeatButton
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
}

void PlayerGUI::resized() {
	auto bounds = getLocalBounds();

	// Playlist Panel on the right
	if (showPlaylistPanel)
	{
		playlistPanelArea = bounds.removeFromRight(350);
		auto playlistBounds = playlistPanelArea.reduced(15);

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
		loadPlaylistButton.setBounds(buttonRow2);
		playlistBounds.removeFromTop(10);
		playlistBox->setBounds(playlistBounds);
	}

	addToPlaylistButton.setVisible(showPlaylistPanel);
	removeFromPlaylistButton.setVisible(showPlaylistPanel);
	savePlaylistButton.setVisible(showPlaylistPanel);
	loadPlaylistButton.setVisible(showPlaylistPanel);
	playlistTitleLabel.setVisible(showPlaylistPanel);
	playlistBox->setVisible(showPlaylistPanel);

	// Volume controls on right
	auto rightPanel = bounds.removeFromRight(180);
	rightPanel.removeFromTop(30);
	auto volumeControlArea = rightPanel.removeFromTop(180);
	auto centeredArea = volumeControlArea.withSizeKeepingCentre(170, 170);
	volumeSlider.setBounds(centeredArea);

	// Mute button centered inside the circular volume slider
	auto muteArea = centeredArea.withSizeKeepingCentre(60, 60);
	muteButton.setBounds(muteArea);


	// Speed slider on left
	auto leftPanel = bounds.removeFromLeft(80);
	leftPanel.removeFromTop(30);

	auto settingsArea = leftPanel.removeFromTop(45);
	settingsButton.setBounds(settingsArea.withSizeKeepingCentre(40, 40));

	leftPanel.removeFromTop(5);



	// نخلي الـ speed label في الآخر (تحت)
	auto sliderArea = leftPanel.removeFromTop(leftPanel.getHeight() - 60);
	speedSlider.setBounds(sliderArea.reduced(20, 40));

	// Speed label في الآخر
	speedLabel.setBounds(leftPanel.removeFromTop(40).reduced(5));

	bounds.removeFromTop(10);

	// Control buttons area
	controlButtonsArea = bounds.removeFromBottom(65).reduced(120, 5);
	
	// Info labels (Metadata)
	int infoHeight = 50;
	int infoMargin = 5;
	auto infoY = controlButtonsArea.getY() - infoMargin - infoHeight;
	auto infoArea = juce::Rectangle<int>(
		controlButtonsArea.getX(),
		infoY,
		controlButtonsArea.getWidth(),
		infoHeight
	);

	// Position Slider
	int sliderHeight = 25;
	int sliderMargin = 5;
	auto positionY = infoArea.getY() - sliderMargin - sliderHeight;
	positionSlider.setBounds(
		controlButtonsArea.getX(),
		positionY,
		controlButtonsArea.getWidth(),
		sliderHeight
	);

	// Waveform area
	int waveformHeight = 200;
	int spaceForDots = 50;
	int waveformY_new = positionY - spaceForDots - waveformHeight;
	auto originalWaveformBounds = bounds.reduced(30, 30);

	waveformArea = juce::Rectangle<int>(
		originalWaveformBounds.getX(),
		waveformY_new,
		originalWaveformBounds.getWidth(),
		waveformHeight
	);

	// Header box (audio picture)
	auto topEmptySpaceBounds = bounds.reduced(20);
	int headerBoxWidth = 450;
	int headerBoxHeight = 350;
	headerBoxWidth = juce::jmin(headerBoxWidth, bounds.getWidth() - 20);
	headerBoxHeight = juce::jmin(headerBoxHeight, bounds.getHeight() - 20);

	int headerBoxX = bounds.getCentreX() - (headerBoxWidth / 2);
	int headerBoxY = bounds.getY() + 10;

	headerBoxArea = juce::Rectangle<int>(headerBoxX, headerBoxY, headerBoxWidth, headerBoxHeight);

	// Info area distribution
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

	// Load button
	int loadButtonWidth = 100;
	int loadButtonHeight = 45;
	int loadSpacing = 15;

	loadButton.setBounds(
		controlButtonsArea.getRight() + loadSpacing,
		controlButtonsArea.getY() + (controlButtonsArea.getHeight() - loadButtonHeight) / 2,
		loadButtonWidth,
		loadButtonHeight
	);



	// Control buttons
	auto buttonRow = controlButtonsArea.reduced(15, 6);

	int numMainButtons = 7;
	int buttonWidth = 60;
	int buttonHeight = 45;
	int buttonSpacing_ctrl = (buttonRow.getWidth() - (numMainButtons * buttonWidth)) / (numMainButtons + 1);

	int x = buttonRow.getX() + buttonSpacing_ctrl;
	int y = buttonRow.getY();

	//الترتيب ال علي عاوزه 
	stopButton.setBounds(x, y, buttonWidth, buttonHeight);
	x += buttonWidth + buttonSpacing_ctrl;


	restartButton.setBounds(x, y, buttonWidth, buttonHeight);
	x += buttonWidth + buttonSpacing_ctrl;


	backwardButton.setBounds(x, y, buttonWidth, buttonHeight);
	x += buttonWidth + buttonSpacing_ctrl;

	playButton.setBounds(x, y, buttonWidth, buttonHeight);
	x += buttonWidth + buttonSpacing_ctrl;

	forwardButton.setBounds(x, y, buttonWidth, buttonHeight);
	x += buttonWidth + buttonSpacing_ctrl;

	go_to_endButton.setBounds(x, y, buttonWidth, buttonHeight);
	x += buttonWidth + buttonSpacing_ctrl;

	repeatButton.setBounds(x, y, buttonWidth, buttonHeight);

	//==================the combo box task 14=======================================
	int startY = 250;
	markerActionBox.setBounds(20, startY, 200, 25);
	markerSelectBox.setBounds(230, startY, 120, 25);
	addMarkerButton.setBounds(360, startY, 100, 25);
	applyMarkerActionButton.setBounds(470, startY, 80, 25);
	//=====================================================//

	// Hide unused buttons
	go_to_startButton.setBounds(0, 0, 0, 0);
	speedButton.setBounds(0, 0, 0, 0);
}

void PlayerGUI::buttonClicked(juce::Button* button) {
	if (control == nullptr && button != &loadButton) return;

	// Playlist buttons
	if (button == &addToPlaylistButton) {
		fileChooser = std::make_unique<juce::FileChooser>(
			"Select an audio file...",
			juce::File{},
			".wav;.mp3;.flac;.aiff;*.ogg"
		);
		fileChooser->launchAsync(
			juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
			[this](const juce::FileChooser& fc) {
				auto file = fc.getResult();
				if (file.existsAsFile()) {
					std::string fileName = file.getFileNameWithoutExtension().toStdString();
					std::string filePath = file.getFullPathName().toStdString();

					auto& playlist = control->getPlaylistManager();
					auto songs = playlist.getSongs("front");

					if (songs.empty()) {
						playlist.add("front", fileName, filePath);
					}
					else {
						playlist.add(songs.back().first, fileName, filePath);
					}

					refreshPlaylist();
				}
			});
		return;
	}
	else if (button == &removeFromPlaylistButton) {
		std::string selectedKey = playlistBox->getSelectedItemKey();
		if (!selectedKey.empty()) {
			control->getPlaylistManager().remove(selectedKey);
			refreshPlaylist();
		}
		return;
	}
	else if (button == &savePlaylistButton) {
		control->getPlaylistManager().save("x.txt");
		return;
	}
	else if (button == &loadPlaylistButton) {
		control->getPlaylistManager().load("x.txt");
		refreshPlaylist();
		return;
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
		
	}

	//task 14
	
		else if (button == &applyMarkerActionButton)
		{
			int selectedAction = markerActionBox.getSelectedId();
			int selectedMarker = markerSelectBox.getSelectedId() - 1; // ComboBox IDs start from 1

				if (selectedAction == 1) // Loop between two markers
				{
					if (markers.size() >= 2)
					{
						control->setLoopBetweenMarkers(0, markers.size() - 1);
						DBG("Looping between " + markers.front().name + " and " + markers.back().name);
					}
				}
				else if (selectedAction == 2) // Go to marker
				{
					if (selectedMarker >= 0 && selectedMarker < markers.size())
					{
						control->goToMarker(selectedMarker);
					}
				}
				else if (selectedAction == 3) // Remove marker 
				{
					if (selectedMarker >= 0 && selectedMarker < markers.size())
					{
						control->removeMarkerr(selectedMarker);
						markers.erase(markers.begin() + selectedMarker);
						repaint();
					}
				}
		}

		else if (button == &addMarkerButton)
		{
			if (control != nullptr)
			{
				double position = control->getAudioPosition();
				juce::String markerName = "Marker " + juce::String(markers.size() + 1);

				// Add to internal list
				markers.push_back({ markerName, position });
				control->addMarker(markerName, position);
				markerSelectBox.addItem(markerName, markers.size());
			}
		}

		else if (button == &markerButton)
		{
			addMarkerMode = !addMarkerMode;
		}
	

	//==================================================//
	// Audio control buttons
	if (button == &loadButton) {
		fileChooser = std::make_unique<juce::FileChooser>(
			"Select an audio file...",
			juce::File{},
			"*.wav;*.mp3;*.flac;*.aiff;*.ogg"
		);

		fileChooser->launchAsync(
			juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
			[this](const juce::FileChooser& fc) {
				auto file = fc.getResult();
				if (file.existsAsFile()) {
					control->startNewFromFile(file);
					thumbnail.setSource(new juce::FileInputSource(file));

					positionSlider.setRange(0.0, control->getLength(), 0.01);
					positionSlider.setValue(0.0, juce::dontSendNotification);

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
			control->setPosition(0.0);
			repaint(waveformArea);
		}
	}
	else if (button == &go_to_endButton) {
		if (control->audioExist()) {
			control->setPosition(control->getLength());
		}
	}
	else if (button == &repeatButton)
	{
		if (!control->audioExist()) return;
		control->toggleLooping();
		if (control->isLooping())
		{
			repeatButton.setButtonText(juce::String::fromUTF8("\xF0\x9F\x94\x81"));
		}
		else
		{
			repeatButton.setButtonText(juce::String::fromUTF8("\xE2\x86\xBB"));
		}
	}
}

void PlayerGUI::timerCallback()
{
	if (control != nullptr && control->isPlaying() && !isUserDraggingPosition)
	{
		double pos = control->getAudioPosition();
		positionSlider.setValue(pos, juce::dontSendNotification);

		repaint(waveformArea);

		if (control->reachEnd()) {
			playBackFinished();
		}
	}

	if (control->isLoopingAB() && control->MarkerASet() && control->MarkerBSet())
	{
		double current = control->getAudioPosition();
		double markerB = control->getMarkerB();
		double markerA = control->getMarkerA();

		if (current >= markerB)
			control->setPosition(markerA);
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
}

void PlayerGUI::refreshPlaylist()
{
	if (control == nullptr) return;

	auto songs = control->getPlaylistManager().getSongs("front");
	playlistBox->updatePlaylist(songs);
}

void PlayerGUI::playTrackFromPlaylist(int index)
{
	if (control == nullptr) return;

	auto songs = control->getPlaylistManager().getSongs("front");
	if (index < 0 || index >= songs.size()) return;

	std::string filePath = songs[index].second;
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

		if (control->audioExist())
		{
			playButton.setButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
			stoped = false;
		}
	}
}
// لما ادوس على ال wave يتغير ال position 
void PlayerGUI::mouseDown(const juce::MouseEvent& event)
{
	if (control != nullptr && control->audioExist() && thumbnail.getTotalLength() > 0.0)
	{
		auto clickableWaveformArea = waveformArea.reduced(10);

		if (clickableWaveformArea.contains(event.getPosition()))
		{
			//to use it for putting a marker :
			if (addMarkerMode)
			{
				double clickX = event.position.x;
				double relativeX = juce::jlimit(0.0, (double)clickableWaveformArea.getWidth(), clickX - clickableWaveformArea.getX());
				double timeInSeconds = (relativeX / clickableWaveformArea.getWidth()) * thumbnail.getTotalLength();

				// Add marker both visually & logically
				juce::String markerName = "Marker " + juce::String(markers.size() + 1);
				control->addMarker(markerName, timeInSeconds);
				markers.push_back({ markerName, timeInSeconds });
				markerSelectBox.addItem(markerName, markers.size());

				DBG("Added " + markerName + " at " + juce::String(timeInSeconds, 2) + " seconds");

				// Reset marker mode
				addMarkerMode = false;
				markerButton.setButtonText("Add Marker");

				repaint();
				return; // don't drag position when adding a marker
			}
			//======================================//
			isUserDraggingPosition = true;

			mouseDrag(event);
			return;
		}
	}

	juce::Component::mouseDown(event);
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