#pragma once

#include "PlayerAudio.h"
#include <JuceHeader.h>
#include <map>


class PlayerGUI : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener, public PlayerAudioSignal, public juce::ListBoxModel, public juce::Timer
{
private:
    // Audio
    PlayerAudio* control;

    // GUI Controls
    juce::TextButton loadButton{ "Load Files" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton go_to_startButton{ "|<" };
    juce::TextButton go_to_endButton{ ">|" };
    juce::TextButton speedButton{ "Speed: 1.0x" };
    juce::TextButton repeatButton{ "Repeat: OFF" };
    juce::TextButton playlistButton{ "playlist" };
    juce::TextButton addlistButton{ "+ Add Song" };
    juce::TextButton deletelistButton{ "- Delete Song" };
    std::vector<juce::TextButton*> buttons = { &loadButton, &restartButton, &stopButton, &playButton, &muteButton, &go_to_startButton, &go_to_endButton, &speedButton, &repeatButton, &playlistButton};

    // Volume Slider
    juce::Slider volumeSlider;
    double lastVal = 0.5;
    bool muted = false;

    //Position Slider 
    juce::Slider positionSlider;
    double startPostion = 0.0;
    bool isStopped = false;
    bool isUserDraggingPosition = false;
    
    std::map<std::string, juce::TextButton*> mp = { {"load", &loadButton},
                                                    {"restart", &restartButton}, 
                                                    {"stop", &stopButton},
                                                    {"play", &playButton},
                                                    {"mute", &muteButton},
                                                    {"|<", &go_to_startButton},
                                                    {">|", &go_to_endButton},
                                                    {"speed", &speedButton},
                                                    {"repeat", &repeatButton},
                                                    {"playlist", &playlistButton} };

    juce::ListBox playlistListBox;
    std::vector<std::pair<std::string, std::string>> currentPlaylist;
    string currentKey;
    bool playlistSidebarVisible = false;

    // Author Name and Title
    juce::Label author;
    juce::Label songTitle;
    juce::Label durationHeader;
    juce::Label name;
    juce::Label title;
    juce::Label duration;
    std::vector<std::pair<juce::String, juce::Label*>> labels = { {"Author: ", &author}, {"Title: ", &songTitle}, {"Duration: ", &durationHeader}, {"Unknown", &name}, {"No Track Loaded", &title}, {"0:00", &duration} };
    bool stoped = true;

    std::unique_ptr<juce::FileChooser> fileChooser;

    void PlayerGUI::initializeControls();

    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void refreshPlaylistDisplay();
    void loadNextTrack();

public:
    PlayerGUI();
    PlayerGUI(PlayerAudio& control);
    void paint(juce::Graphics& g) override;
    void resized() override;

    void selectedRowsChanged(int lastRowSelected) override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;
    void timerCallback() override;

    void playBackFinished();

    juce::TextButton* getButton(std::string s);
    juce::Slider& getVolume();
    juce::Slider& getPosition();
};