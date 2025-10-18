#pragma once

#include <JuceHeader.h>
#include <vector>
#include <algorithm>

class PlayerAudio : public juce::AudioAppComponent {
private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

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
    void move(double val);
};