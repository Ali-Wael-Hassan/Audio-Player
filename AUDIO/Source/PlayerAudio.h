#pragma once

#include <JuceHeader.h>
#include <vector>
#include <algorithm>
#include "PlayerAudioSignal.h"
#include "PlaylistManager.h"

class PlayerAudio : public juce::AudioAppComponent {
private:
    PlayerAudioSignal* listen = nullptr;
    std::unique_ptr<juce::ResamplingAudioSource> resamplingSource;

    PlaylistManager playlist;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    juce::String titleText = "No Track Loaded";
    juce::String nameText = "Unknown";
    juce::String durationText = "0:00";
    bool loopActive = false;
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

    double getAudioPosition();
    double getAudioLength();

    void toggleLooping();
    bool isLooping() const;   

    juce::String getName();
    juce::String getTitle();
    juce::String getDuration();
    PlaylistManager& getPlaylistManager();
    void setSignalListener(PlayerAudioSignal* l);
    bool reachEnd();
};