#pragma once

#include <JuceHeader.h>
#include <map>
#include <string>
#include "PlayerAudio.h"
//==============================================================================
// SETTINGS PAGE
//==============================================================================

class SettingsPage : public juce::Component,
    public juce::ComboBox::Listener
{
public:
    // Listener Class
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void themeSettingChanged(const juce::String& newThemeName) = 0;
        virtual void languageSettingChanged(const juce::String& newLanguageName) = 0;
    };

    // Constructor and Destructor
    SettingsPage(PlayerAudio* playerRef, const std::string& color);
    ~SettingsPage() override = default;

    // Visuals
    void paint(juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    // Listener Management
    void addListener(Listener* listener);
    void removeListener(Listener* listener);

    // Accessors
    juce::TextButton& getCloseButton() { return closeButton; }

private:
    PlayerAudio* player;
    juce::Label sleepLabel{ "Sleep Timer", "Sleep Timer (minutes):" };
    juce::Slider sleepSlider;
    juce::TextButton startTimerButton{ "Start" };
    juce::TextButton pauseTimerButton{ "Pause" };
    juce::TextButton resumeTimerButton{ "Resume" };
    juce::TextButton cancelTimerButton{ "Cancel" };

    juce::Label themeLabel{ u8"themeLabel", u8"Theme:" };
    juce::ComboBox themeComboBox;

    juce::Label languageLabel{ u8"languageLabel", u8"Language:" };
    juce::ComboBox languageComboBox;

    juce::TextButton closeButton{ u8"Close" };

    // Theme
    const std::map<std::string, juce::Colour> colorMap = {
        {u8"Dark", juce::Colour::fromRGB(0x1a, 0x1a, 0x1a)},
        {u8"Light", juce::Colour::fromRGB(0xf0, 0xf0, 0xf0)}
    };

    using LanguagesMap = std::map<juce::String, juce::String>;

    #define AR_SGL juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd8\xb3\xd8\xac\xd9\x84")
    #define AR_AD juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd8\xa7\xd8\xb9\xd8\xaf\xd8\xa7\xd8\xaf\xd8\xa7\xd8\xaa")
    #define AR_AGANI juce::String::fromUTF8("\xd8\xac\xd9\x85\xd9\x8a\xd8\xb9 \xd8\xa7\xd9\x84\xd8\xa3\xd8\xba\xd8\xa7\xd9\x86\xd9\x8a")
    #define AR_BA juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd8\xa8\xd8\xad\xd8\xab")
    #define AR_MKTBA juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd9\x85\xd9\x83\xd8\xaa\xd8\xa8\xd8\xa9")
    #define AR_MF juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd9\x85\xd9\x81\xd8\xb6\xd9\x84\xd8\xa9")
    #define AR_QAEMA juce::String::fromUTF8("\xd9\x82\xd8\xa7\xd8\xa6\xd9\x85\xd8\xaa\xd9\x8a")
    #define AR_TD juce::String::fromUTF8("\xd8\xaa\xd8\xb9\xd8\xaf\xd9\x8a\xd9\x84")
    #define AR_NORES juce::String::fromUTF8("\xd9\x84\xd8\xa7 \xd8\xaa\xd9\x88\xd8\xac\xd8\xaf \xd9\x86\xd8\xaa\xd8\xa7\xd8\xa6\xd8\xac \xd9\x84\xd9\x84\xd8\xa8\xd8\xad\xd8\xab \xd8\xb9\xd9\x86")
    #define AR_CLOSE juce::String::fromUTF8("\xd8\xa7\xd8\xba\xd9\x84\xd8\xa7\xd9\x82")
    #define AR_MTHR juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd9\x85\xd8\xb8\xd9\x87\xd8\xb1:")
    #define AR_LGHA juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd9\x84\xd8\xba\xd8\xa9")
    #define AR_DARK juce::String::fromUTF8("\xd8\xaf\xd8\xa7\xd9\x83\xd9\x86")
    #define AR_LIGHT juce::String::fromUTF8("\xd8\xb3\xd8\xa7\xd8\xb7\xd8\xb9")
    #define AR_ARABIC juce::String::fromUTF8("\xd8\xa7\xd9\x84\xd8\xb9\xd8\xb1\xd8\xa8\xd9\x8a\xd8\xa9")
    #define AR_SLEEP juce::String::fromUTF8("\xd9\x85\xd8\xa4\xd9\x82\xd8\xaa \xd8\xa7\xd9\x84\xd9\x86\xd9\x88\xd9\x85:")
    #define AR_START juce::String::fromUTF8("\xd8\xa7\xd8\xa8\xd8\xaf\xd8\xa3")
    #define AR_PAUSE juce::String::fromUTF8("\xd8\xa5\xd9\x8a\xd9\x82\xd8\xa7\xd9\x81 \xd9\x85\xd8\xa4\xd9\x82\xd8\xaa")
    #define AR_RESUME juce::String::fromUTF8("\xd8\xa7\xd8\xb3\xd8\xaa\xd8\xa6\xd9\x86\xd8\xa7\xd9\x81")
    #define AR_CANCEL juce::String::fromUTF8("\xd8\xa5\xd9\x84\xd8\xba\xd8\xa7\xd8\xa1")
    #define AR_MINUTES juce::String::fromUTF8("\xd8\xaf\xd9\x82\xd9\x8a\xd9\x82\xd8\xa9")

    const std::map<juce::String, LanguagesMap> LanguageMap = {
        { "English", {
            {AR_SGL,              "Logs"},
            {AR_AD,               "Settings"},
            {AR_AGANI,            "All Songs"},
            {AR_BA,               "Search"},
            {AR_MKTBA,            "Library"},
            {AR_MF,               "Favorites"},
            {AR_QAEMA,            "My List"},
            {AR_TD,               "Edit"},
            {AR_NORES,            "No results found for"},
            {AR_CLOSE,            "Close"},
            {AR_MTHR,             "Theme:"},
            {AR_LGHA,             "Language"},
            {AR_DARK,             "Dark"},
            {AR_LIGHT,            "Light"},
            {AR_ARABIC,           "Arabic"},
            {AR_SLEEP,            "Sleep Timer:"},
            {AR_START,            "Start"},
            {AR_PAUSE,            "Pause"},
            {AR_RESUME,           "Resume"},
            {AR_CANCEL,           "Cancel"},
            {AR_MINUTES,          "minutes"}
        }},
        { "Arabic", {
            {"Logs",                AR_SGL},
            {"Settings",            AR_AD},
            {"All Songs",           AR_AGANI},
            {"Search",              AR_BA},
            {"Library",             AR_MKTBA},
            {"Favorites",           AR_MF},
            {"My List",             AR_QAEMA},
            {"Edit",                AR_TD},
            {"No results found for", AR_NORES},
            {"Close",               AR_CLOSE},
            {"Theme:",              AR_MTHR},
            {"Language",            AR_LGHA},
            {"Dark",                AR_DARK},
            {"Light",               AR_LIGHT},
            {"Arabic",              AR_ARABIC},
            {"Sleep Timer:",        AR_SLEEP},
            {"Start",               AR_START},
            {"Pause",               AR_PAUSE},
            {"Resume",              AR_RESUME},
            {"Cancel",              AR_CANCEL},
            {"minutes",             AR_MINUTES}
        }}
    };

    #undef AR_SGL
    #undef AR_AD
    #undef AR_AGANI
    #undef AR_BA
    #undef AR_MKTBA
    #undef AR_MF
    #undef AR_QAEMA
    #undef AR_TD
    #undef AR_NORES
    #undef AR_CLOSE
    #undef AR_MTHR
    #undef AR_LGHA
    #undef AR_DARK
    #undef AR_LIGHT
    #undef AR_ARABIC
    #undef AR_SLEEP
    #undef AR_START
    #undef AR_PAUSE
    #undef AR_RESUME
    #undef AR_CANCEL
    #undef AR_MINUTES


    std::string currentColorKey;
    juce::Colour currentBackgroundColour;

    // to manage all listeners
    juce::ListenerList<Listener> listeners;
    // for updating Theme
    void updateTheme(const juce::String& themeName);
    void updateLanguage(const juce::String& themeName);
};