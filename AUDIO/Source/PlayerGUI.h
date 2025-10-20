#pragma once

#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    private juce::Timer
{
private:
    PlayerAudio* control = nullptr;

<<<<<<< HEAD
    // === Buttons ===
=======
    // GUI Controls
    std::vector<juce::TextButton*> buttons = { &loadButton, &restartButton, &stopButton, &playButton, &muteButton, &go_to_startButton, &go_to_endButton };
>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
    juce::TextButton loadButton{ "Load Files" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton muteButton{ "Mute" };
<<<<<<< HEAD
    juce::TextButton loopButton{ "Loop" };
    juce::TextButton go_to_startButton{ "<< 10s" };
    juce::TextButton go_to_endButton{ "10s >>" };

    std::vector<juce::TextButton*> buttons{
        &loadButton, &restartButton, &stopButton,
        &playButton, &muteButton, &loopButton, &go_to_startButton, &go_to_endButton
    };

    // === Sliders & Labels ===
    juce::Slider volumeSlider;    // Controls audio volume
    juce::Slider positionSlider;  // Controls playback position
    juce::Label timeLabel;        // Displays current time / total time
    juce::Label hoverTimeLabel;   // <-- Added: displays hover time above the slider

    // === States ===
    float lastVal = 0.5f;
=======
    juce::TextButton go_to_startButton{ "Move to Start" };
    juce::TextButton go_to_endButton{ "Move to End" };

    // Volume Slider
    juce::Slider volumeSlider;
    double lastVal = 0.5;
>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
    bool muted = false;
    bool isLooping = false;

<<<<<<< HEAD
    // === File chooser ===
=======
    // Author Name and Title
    juce::Label name;
    juce::Label title;
    juce::Label duration;

>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
    std::unique_ptr<juce::FileChooser> fileChooser;

 
    void initializeControls();
    void timerCallback() override;

public:
    PlayerGUI();
    PlayerGUI(PlayerAudio& control);

    void paint(juce::Graphics& g) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    // === Accessors ===
    juce::TextButton& getLoad();
    juce::TextButton& getRestart();
    juce::TextButton& getStop();
    juce::Slider& getVolume();

    // === JUCE Macro ===
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
