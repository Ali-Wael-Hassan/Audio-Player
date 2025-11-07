#pragma once

#include <JuceHeader.h>
#include <vector>
#include "PlayerGUI.h"
#include "Home.h"

class MainComponent : public juce::AudioAppComponent,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    // Audio
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // GUI
    void paint(juce::Graphics& g) override;
    void resized() override;

    // Event handlers
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    // Audio
    PlayerAudio* audio1;
    PlayerAudio* audio2;

    PlayerGUI* GUI1;
    PlayerGUI* GUI2;

    Home* home;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};