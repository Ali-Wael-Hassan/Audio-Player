#pragma once

#include "PlayerAudio.h"


class PlayerGUI : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener
{
private:
    // Audio
    PlayerAudio* control;

    // GUI Controls
    std::vector<juce::TextButton*> buttons = { &loadButton, &restartButton, &stopButton, &playButton, &muteButton, &go_to_startButton, &go_to_endButton, &speedButton };
    juce::TextButton loadButton{ "Load Files" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton go_to_startButton{ "Move to Start" };
    juce::TextButton go_to_endButton{ "Move to End" };
    juce::TextButton speedButton{ "Speed: 1.0x" };

    // Volume Slider
    juce::Slider volumeSlider;
    double lastVal = 0.5;
    bool muted = false;

    // Author Name and Title
    juce::Label name;
    juce::Label title;
    juce::Label duration;

    std::unique_ptr<juce::FileChooser> fileChooser;

    void PlayerGUI::initializeControls();

public:
    PlayerGUI();
    PlayerGUI(PlayerAudio& control);
    void paint(juce::Graphics& g) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;

    juce::TextButton& getLoad();
    juce::TextButton& getRestart();
    juce::TextButton& getStop();
    juce::Slider& getVolume();
};