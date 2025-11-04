#pragma once

#include <JuceHeader.h>
#include <vector>
#include <algorithm>
//#include <fileref.h>
//#include <tag.h>
#include "PlayerAudioSignal.h"
#include "PlaylistManager.h"

class PlayerAudio : public juce::AudioAppComponent {
public:
    class PlayerAudioSignal
    {
    public:
        virtual ~PlayerAudioSignal() = default;
        virtual void playBackFinished() = 0;
        virtual void loadMetaData() = 0;
        virtual void loadWave(juce::File file) = 0;
    };

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
    void startNewFromFile(juce::File file);
    juce::String startNewFromURL(juce::URL url);
    void setPosition(double pos);
    void jumpTime(double seconds);
    double getLength();
    void setSpeed(double ratio);
    double getAudioPosition();
    double getAudioLength();

    //Task 10
    void setMarkers(double position);
    bool isLoopingAB()const;
    bool MarkerASet()const;
    bool MarkerBSet()const;
    double getMarkerA();
    double getMarkerB();
    //Task 4
    void toggleLooping();
    bool isLooping() const;
    //task 14
   
    void addMarker(const juce::String& name, double position);
    void removeMarkerr(int index);
    void goToMarker(int index);
    void setLoopBetweenMarkers(int startIndex, int endIndex);
    void clearLoop();

    //====================================================//
    juce::String getName();
    juce::String getTitle();
    juce::String getDuration();

    PlaylistManager& getPlaylistManager();

    void setSignalListener(PlayerAudioSignal* l);
    bool reachEnd();

    juce::AudioFormatManager& getFormatManager() { return formatManager; }
    juce::AudioThumbnailCache& getThumbnailCache() { return thumbnailCache; }

    double getCurrentPosition() const { return transportSource.getCurrentPosition(); }

    bool isPlaying() const { return transportSource.isPlaying(); }

private:
    PlayerAudioSignal* listen = nullptr;
    std::unique_ptr<juce::ResamplingAudioSource> resamplingSource;
    PlaylistManager playlist;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::AudioThumbnailCache thumbnailCache{ 5 };

    juce::String titleText = "No Track Loaded";
    juce::String nameText = "Unknown";
    juce::String durationText = "00:00";
    bool loopActive = false;

    //Task 10
    double markerA = -1.0;
    double markerB = -1.0;
    bool loopingAB = false;
    bool markerASet = false;
    bool markerBSet = false;

    //Task 14 
    struct Marker {
        juce::String name;
        double position; // seconds
    };

    std::vector<Marker> markers;
    bool isLoopingBetweenMarkers = false;
    int loopStartIndex = -1;
    int loopEndIndex = -1;


};