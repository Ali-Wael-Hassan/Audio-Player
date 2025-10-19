#pragma once

#include "PlayerAudio.h"


class PlayerGUI : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener
{
private:
    // Audio
    PlayerAudio* control;

    // GUI Controls
    std::vector<juce::TextButton*> buttons = { &loadButton, &restartButton, &stopButton };
    juce::TextButton loadButton{ "Load Files" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };

    // Volume Slider
    juce::Slider volumeSlider;

    std::unique_ptr<juce::FileChooser> fileChooser;

    void PlayerGUI::initializeControls();

public:
    PlayerGUI();
    PlayerGUI(PlayerAudio& control);
    void paint(juce::Graphics& g) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    juce::TextButton& getLoad();
    juce::TextButton& getRestart();
    juce::TextButton& getStop();
    juce::Slider& getVolume();
};