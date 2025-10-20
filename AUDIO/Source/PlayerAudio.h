#pragma once

#include <JuceHeader.h>
#include <vector>
#include <algorithm>

class PlayerAudio : public juce::AudioAppComponent {
private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

<<<<<<< HEAD
    bool isMuted = false;
    float lastGain = 1.0f;
    bool isLooping = false;
=======
    juce::String titleText = "No Track Loaded";
    juce::String nameText = "Artist: Unknown";
    juce::String durationText = "-1";
>>>>>>> 3931288ef8275e7716290f244e039e4f53808192

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
<<<<<<< HEAD
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
=======
    void setPosition(double pos);
    double getLength();
    juce::String getName();
    juce::String getTitle();
    juce::String getDuration();
};
>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
