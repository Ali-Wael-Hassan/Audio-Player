#include "PlayerGUI.h"

void PlayerGUI::initializeControls()
{
    // Add all buttons and set listeners
    for (auto* btn : buttons)
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }

<<<<<<< HEAD
    // Volume slider setup
    //volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
=======
    name.setText("Artist: Unknown", juce::dontSendNotification);
    name.setFont(juce::Font(16.0f, juce::Font::bold));
    name.setJustificationType(juce::Justification::centredLeft);
    name.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(name);

    title.setText("No Track Loaded", juce::dontSendNotification);
    title.setFont(juce::Font(14.0f, juce::Font::bold));
    title.setJustificationType(juce::Justification::centredLeft);
    title.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(title);

    duration.setText("-1", juce::dontSendNotification);
    duration.setFont(juce::Font(14.0f, juce::Font::bold));
    duration.setJustificationType(juce::Justification::centredLeft);
    duration.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(duration);

>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    // Position slider setup
    positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0); // Removed text box under the slider
    positionSlider.setPopupDisplayEnabled(false, false, nullptr); // Disable default popup
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

    // Time label 
    addAndMakeVisible(timeLabel);
    timeLabel.setJustificationType(juce::Justification::centred);
    timeLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    //  hover time label 
    addAndMakeVisible(hoverTimeLabel);
    hoverTimeLabel.setJustificationType(juce::Justification::centred);
    hoverTimeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    hoverTimeLabel.setColour(juce::Label::backgroundColourId, juce::Colours::black.withAlpha(0.7f));
    hoverTimeLabel.setSize(50, 20);
    hoverTimeLabel.setVisible(false);

    setSize(500, 250);
<<<<<<< HEAD
    startTimer(100);
=======

>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
}

// Constructors
PlayerGUI::PlayerGUI() : control(nullptr) { initializeControls(); }
PlayerGUI::PlayerGUI(PlayerAudio& control) : control(&control) { initializeControls(); }

// Painting 
void PlayerGUI::paint(juce::Graphics& g) {
    juce::ColourGradient gradient(juce::Colours::darkslateblue, 0, 0,
        juce::Colours::black, getWidth(), getHeight(), false);
    g.setGradientFill(gradient);
    g.fillAll();
}

// Layout
void PlayerGUI::resized() {
    auto bounds = getLocalBounds().reduced(10);
    auto buttonRow = bounds.removeFromTop(40).reduced(5);
    auto buttonWidth = buttonRow.getWidth() / 8;

    for (auto* btn : buttons)
        btn->setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));

    auto posSliderArea = bounds.removeFromTop(40);
    positionSlider.setBounds(posSliderArea.reduced(5));

<<<<<<< HEAD
    auto timeLabelArea = bounds.removeFromTop(25);
    timeLabel.setBounds(timeLabelArea);

    auto volumeSliderArea = bounds.removeFromTop(40);
=======
    auto volumeSliderArea = bounds.removeFromTop(50).reduced(5);
>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
    volumeSlider.setBounds(volumeSliderArea.reduced(5));

    auto nameLabelArea = bounds.removeFromTop(20).reduced(0, 5);
    name.setBounds(nameLabelArea);

    auto titleLabelArea = bounds.removeFromTop(20).reduced(0, 5);
    title.setBounds(titleLabelArea);

    auto durationArea = bounds.removeFromTop(20).reduced(0, 5);
    duration.setBounds(durationArea);
}

// Button clicks
void PlayerGUI::buttonClicked(juce::Button* button) {
    if (control == nullptr) return;

    if (button == &loadButton) {
        fileChooser = std::make_unique<juce::FileChooser>("Select an audio file...", juce::File{}, "*.wav;*.mp3");
        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc) {
                auto file = fc.getResult();
                if (file.existsAsFile()) {
                    control->startNew(file);
<<<<<<< HEAD
                    double total = control->getLengthInSeconds();
                    positionSlider.setRange(0.0, total, 0.01);
=======
                    name.setText(control->getName(), juce::dontSendNotification);
                    title.setText(control->getTitle(), juce::dontSendNotification);
                    duration.setText(control->getDuration(), juce::dontSendNotification);
>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
                }
            });
    }
    else if (button == &restartButton) {
        if (control->audioExist()) control->restart();
    }
    else if (button == &stopButton) {
        if (control->audioExist()) {
            control->stop();
        }
    }
    else if (button == &playButton) {
        if (control->audioExist()) control->start();
    }
    else if (button == &muteButton) {
        if (control->audioExist()) {
            if (muted) {
                muted = false;
                volumeSlider.setValue(lastVal);
                control->setGain((double)lastVal);
                muteButton.setButtonText("Mute");
            }
            else {
                if (volumeSlider.getValue() > 0.001) lastVal = volumeSlider.getValue();
                muted = true;
                volumeSlider.setValue(0.0);
                control->setGain(0.0);
                muteButton.setButtonText("Unmute");
            }
        }
    }
    else if (button == &loopButton) {
        if (control->audioExist()) {
<<<<<<< HEAD
            isLooping = !isLooping;
            control->setLooping(isLooping);
            loopButton.setButtonText(isLooping ? "Unloop" : "Loop");
=======
            control->stop();
            control->setPosition(0.0);
>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
        }
    }
    else if (button == &go_to_startButton) {
        if (control->audioExist()) control->move(-10);
    }
    else if (button == &go_to_endButton) {
<<<<<<< HEAD
        if (control->audioExist()) control->move(10);
=======
        if (control->audioExist()) {
            control->setPosition(control->getLength());
        }
>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
    }
}

// Slider logic
void PlayerGUI::sliderValueChanged(juce::Slider* slider) {
    if (control == nullptr) return;

    if (slider == &volumeSlider)
        control->setGain((float)slider->getValue());

    else if (slider == &positionSlider && control->audioExist()) {
        // Show hover time above the slider when dragging
        if (slider->isMouseButtonDown()) {
            double hoverPos = slider->getValue();
            int mins = (int)(hoverPos / 60);
            int secs = (int)std::fmod(hoverPos, 60);
            hoverTimeLabel.setText(juce::String::formatted("%02d:%02d", mins, secs), juce::dontSendNotification);

            int x = positionSlider.getX() + (int)positionSlider.getPositionOfValue(hoverPos) - 25;
            int y = positionSlider.getY() - 25;
            hoverTimeLabel.setBounds(x, y, 50, 20);
            hoverTimeLabel.setVisible(true);
        }
        else {
            hoverTimeLabel.setVisible(false);
        }
    }
}

<<<<<<< HEAD
void PlayerGUI::sliderDragEnded(juce::Slider* slider) {
    if (control == nullptr) return;

    if (slider == &positionSlider && control->audioExist()) {
        control->setPosition(slider->getValue());
        hoverTimeLabel.setVisible(false); // Hide hover label after drag
    }
}

// Timer update
void PlayerGUI::timerCallback() {
    if (control == nullptr || !control->audioExist()) return;

    double pos = control->getCurrentPosition();
    double total = control->getLengthInSeconds();

    if (isLooping && pos >= total - 0.05 && !control->isPlaying())
        control->restart();

    if (!positionSlider.isMouseButtonDown())
        positionSlider.setValue(pos, juce::dontSendNotification);

    auto formatTime = [](double seconds) {
        int mins = (int)(seconds / 60);
        int secs = (int)std::fmod(seconds, 60);
        return juce::String::formatted("%02d:%02d", mins, secs);
        };

    juce::String text = formatTime(pos) + " / " + formatTime(total);
    timeLabel.setText(text, juce::dontSendNotification);
}

// Getters
juce::TextButton& PlayerGUI::getLoad() { 
=======
juce::TextButton& PlayerGUI::getLoad()
{
>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
    return loadButton;
}

juce::TextButton& PlayerGUI::getRestart() {
    return restartButton;
}

juce::TextButton& PlayerGUI::getStop() { 
    return stopButton;
}

juce::Slider& PlayerGUI::getVolume() {
    return volumeSlider;
}
