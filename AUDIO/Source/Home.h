#pragma once
#include <JuceHeader.h>
#include <vector>
#include <map>
#include "SettingsPage.h"

using namespace std;

class Home : public juce::Component, public juce::Button::Listener, SettingsPage::Listener
{
private:
    using ThemeColors = map<string, juce::Colour>;

    static const std::map<string, ThemeColors> ThemeColorMap;

    juce::TextButton tempSuggest{ "Suggest" };
    juce::TextButton tempAudio1{ "Audio" };
    juce::TextButton tempAudio2{ "Audio" };
    juce::TextButton tempAudio3{ "Audio" };
    juce::TextButton tempAudio4{ "Audio" };
    juce::TextButton tempAudio5{ "Audio" };
    // Top bar
    juce::TextEditor searchBar;

    juce::ImageButton logsButton;
    juce::ImageButton settingsButton;

    juce::Rectangle<int> topBarArea;

    // logo bar
    juce::Rectangle<int> logoArea;

    //Left 
    juce::ImageButton homeButton;
    juce::ImageButton searchButton;
    juce::ImageButton libraryButton;
    juce::ImageButton favoriteButton;
    juce::ImageButton listButton;
    juce::ImageButton editButton;

    juce::Rectangle<int> leftBarArea;
    
    // Labels
    juce::Label homeText;
    juce::Label searchText;
    juce::Label libraryText;
    juce::Label favoriteText;
    juce::Label listText;
    juce::Label editText;
    juce::Label settingsText;
    juce::Label logsText;
    juce::Label playList;
    juce::Label recent;

    std::vector<std::pair<juce::String, juce::Label*>> labels = { {"Home", &homeText},
                                                                  {"Search", &searchText},
                                                                  {"Library", &libraryText},
                                                                  {"Favorite", &favoriteText},
                                                                  {"Mylist", &listText},
                                                                  {"Edit", &editText},
                                                                  {"Settings", &settingsText}, 
                                                                  {"Logs", &logsText},
                                                                  {"Playlist", &playList},
                                                                  {"Recent", &recent}};

    // settings page
    unique_ptr<SettingsPage> settingsOverlay;
    bool isSettingsVisible = false;
    juce::Rectangle<int> backgroundDimmerArea;

    string currentTheme;
    string currentLanguage;


    void layoutTopBar(juce::Rectangle<int>& bounds);
    void layoutLeftBar(juce::Rectangle<int>& bounds);
    void layoutPageArea(juce::Rectangle<int>& bounds);
    void layoutSettingsOverlay();

    void paintBackgroundGradients(juce::Graphics& g, const map<string, juce::Colour>& themeMap);
    void paintModalDimmer(juce::Graphics& g, const map<string, juce::Colour>& themeMap);

    void themeSettingChanged(const juce::String& newThemeName) override;
    void languageSettingChanged(const juce::String& newLanguageName) override;

public:
    Home(const string&, const string&);
    ~Home() = default;
    void paint(juce::Graphics& g) override;
    void resized() override;

    void buttonClicked(juce::Button*) override;
};