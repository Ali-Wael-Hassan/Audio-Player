#pragma once

#include <JuceHeader.h>
#include <vector>
#include <algorithm>

class PlayerAudio : public juce::AudioAppComponent {
private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    bool isMuted = false;
    float lastGain = 1.0f;
    bool isLooping = false;

public:
    PlayerAudio();
    ~PlayerAudio() override;

    // JUCE audio callbacks
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // Core audio control
    bool audioExist();
    void start();
    void stop();
    void restart();
    void reset();
    void startNew(juce::File file);
    void move(double val);
    void setGain(float val);

    // Loop
    void setLooping(bool shouldLoop);
    bool getLooping() const;

    // Position control
    double getCurrentPosition() const;
    double getLengthInSeconds() const;
    void setPosition(double newPositionInSeconds);
    bool isPlaying() const;

    // Mute
    void mute(bool shouldMute);
};
