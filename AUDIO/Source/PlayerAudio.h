#pragma once

#include <JuceHeader.h>
#include <vector>
#include <algorithm>
#include <fileref.h>
#include <tag.h>
#include "PlaylistManager.h"

struct Marker {
    double position;
    juce::String label;
    int type;
};

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

    void toggleLooping();
    int isLooping() const;

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

    void setCurrentPlaylistName(const juce::String& name) { currentPlaylistName = name; }
    juce::String getCurrentPlaylistName() const { return currentPlaylistName; }
    juce::String getFileSource() const { return pathFile; }
    void setLooping(bool loop) { loopActive = loop; }

    void startSleepTimer(int seconds);
    void pauseSleepTimer();
    void resumeSleepTimer();
    void cancelSleepTimer();
    bool isSleepTimerRunning() const;

    // Marker
	void addMarker(double position, const juce::String& label, int type) { markers.push_back({ position, label, type }); }
    void removeMarker(int idx) {
        if (idx == markerB)
        {
            markerB = -1;
            markerLooping = false;
        }
        if (idx == markerA)
        {
            markerA = -1;
            markerLooping = false;
        }
        markers.erase(markers.begin() + idx); 
    }
    void updateMarker(const std::vector<Marker>& updated) { markers = updated; }
    void setMarkerA(int index) { 
        if(markerA >= 0)
            markers[markerA].type = 0 ;
        markerA = index; 
        markers[index].type = 1; 
        markerLooping = false;
    }
    void setMarkerB(int index) {
        if (markerB >= 0)
            markers[markerB].type = 0 ; 
        markerB = index; 
        markers[index].type = 2; 
        markerLooping = false;
    }
	void toggleLoopAB() { 
        if (markerA < 0 || markerB < 0) {
            markerLooping = false;
            return;
        }
        markerLooping = !markerLooping; 
    }
    void goToMarker(double pos) { setPosition(pos);  }
	bool isMarkerLooping() const { return markerLooping; }
	int getMarkerA() const { return markerA; }
	int getMarkerB() const { return markerB; }
	std::vector<Marker> getMarkers() const { return markers; }
private:
    class SleepTimer : public juce::Timer
    {
    public:
        SleepTimer(PlayerAudio& owner) : player(&owner) {}

        void start(int minutes)
        {
            remainingSeconds = minutes * 60;
            paused = false;
            startTimer(1000);
        }

        void pause() { paused = true; }
        void resume() { paused = false; }
        void cancel() { stopTimer(); remainingSeconds = 0; }
        bool isRunning() const { return isTimerRunning(); }

        void timerCallback() override
        {
            if (!paused && remainingSeconds > 0)
            {
                --remainingSeconds;
                if (remainingSeconds == 0)
                {
                    stopTimer();
                    player->stop();
                }
            }
        }

    private:
        PlayerAudio* player;
        int remainingSeconds = 0;
        bool paused = false;
    };

    std::unique_ptr<SleepTimer> sleepTimer;

    juce::String pathFile = "";
    PlayerAudioSignal* listen = nullptr;
    std::unique_ptr<juce::ResamplingAudioSource> resamplingSource;
    PlaylistManager playlist;

    juce::String currentPlaylistName = "Default";

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::AudioThumbnailCache thumbnailCache{ 5 };

    juce::String titleText = "No Track Loaded";
    juce::String nameText = "Unknown";
    juce::String durationText = "00:00";
    int loopActive = 0;

    // Marker
	std::vector<Marker> markers;
    int markerA = -1, markerB = -1;
	bool markerLooping = false;
};
