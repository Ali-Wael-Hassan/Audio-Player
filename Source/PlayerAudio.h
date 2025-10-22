#pragma once

#include <JuceHeader.h>
#include <vector>
#include <algorithm>

class PlayerAudio : public juce::AudioAppComponent {
private:
    std::unique_ptr<juce::ResamplingAudioSource> resamplingSource;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    juce::String titleText = "No Track Loaded";
    juce::String nameText = "Unknown";
    juce::String durationText = "0:00";
    //===========for the toggle=============//
    bool loopActive = false;  // is rebeat active?

public:
    PlayerAudio();
    ~PlayerAudio();
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    bool audioExist();
    void start();
    void stop();
    void restart();
    void reset();
    void setGain(float val);
    void startNew(juce::File file);
    void setPosition(double pos);
    double getLength();
    void setSpeed(double ratio);

    //=====================Toggle Loop=======================//
    //=========Omar====================//
    void toggleLooping();   // toggle looping on/off
    bool isLooping() const;   

    juce::String getName();
    juce::String getTitle();
    juce::String getDuration();

};