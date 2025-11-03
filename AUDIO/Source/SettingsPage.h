#pragma once

#include <JuceHeader.h>
#include <map>
#include <string>

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
    SettingsPage(const std::string& color);
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
    juce::Label themeLabel{ "themeLabel", "Theme:" };
    juce::ComboBox themeComboBox;

    juce::Label languageLabel{ "languageLabel", "Language:" };
    juce::ComboBox languageComboBox;

    juce::TextButton closeButton{ "Close" };

    // Theme
    const std::map<std::string, juce::Colour> colorMap = {
        {"Dark", juce::Colour::fromRGB(0x1a, 0x1a, 0x1a)},
        {"Light", juce::Colour::fromRGB(0xf0, 0xf0, 0xf0)}
    };

    std::string currentColorKey;
    juce::Colour currentBackgroundColour;

    // to manage all listeners
    juce::ListenerList<Listener> listeners;
    // for updating Theme
    void updateTheme(const juce::String& themeName);
};