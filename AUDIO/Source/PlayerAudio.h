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

    //Task 10
    double markerA = -1.0;
    double markerB = -1.0;
    bool loopingAB = false;
    bool markerASet = false;
    bool markerBSet = false;
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
    //Task 10
    void setMarkers(double position);
    bool isLoopingAB()const;
    bool MarkerASet()const;
    bool MarkerBSet()const;
    double getMarkerA();
    double getMarkerB();

    juce::String getName();
    juce::String getTitle();
    juce::String getDuration();
    PlaylistManager& getPlaylistManager();
    void setSignalListener(PlayerAudioSignal* l);
    bool reachEnd();
};