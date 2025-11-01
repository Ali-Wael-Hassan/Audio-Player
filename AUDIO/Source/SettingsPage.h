#pragma once
#include <JuceHeader.h>
#include <map>

using namespace std;

class SettingsPage : public juce::Component, public juce::ComboBox::Listener
{
    public:
        class Listener
        {
        public:
            virtual ~Listener() = default;
            virtual void themeSettingChanged(const juce::String& newThemeName) = 0;
            virtual void languageSettingChanged(const juce::String& newLanguageName) = 0;
        };

        void addListener(Listener* listener);
        void removeListener(Listener* listener);

        SettingsPage(const string& color);
        ~SettingsPage() override = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

        juce::TextButton& getCloseButton() { return closeButton; }
    private:
        juce::Label themeLabel{ "themeLabel", "Theme:" };
        juce::ComboBox themeComboBox;

        juce::Label languageLabel{ "languageLabel", "Language:" };
        juce::ComboBox languageComboBox;


        const map<string, juce::Colour> colorMap = {
            {"Dark", juce::Colour::fromRGB(0x1a, 0x1a, 0x1a)},
            {"Light", juce::Colour::fromRGB(0xf0, 0xf0, 0xf0)}
        };
        string currentColorKey;
        juce::Colour currentBackgroundColour;
        juce::TextButton closeButton{ "Close" };

        juce::ListenerList<Listener> listeners;

        void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
        void updateTheme(const juce::String& themeName);

};

