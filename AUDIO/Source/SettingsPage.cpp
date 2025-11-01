#include "SettingsPage.h"

SettingsPage::SettingsPage(const string& initialColorKey) : currentColorKey(initialColorKey), currentBackgroundColour(colorMap.at(initialColorKey))
{
    themeComboBox.addItem("Dark", 1);
    themeComboBox.addItem("Light", 2);
    themeComboBox.setSelectedId(1);

    themeComboBox.addListener(this);

    addAndMakeVisible(themeLabel);
    addAndMakeVisible(themeComboBox);

    languageComboBox.addItem("English", 1);
    languageComboBox.addItem("Arabic", 2);
    languageComboBox.setSelectedId(1);

    languageComboBox.addListener(this);

    addAndMakeVisible(languageLabel);
    addAndMakeVisible(languageComboBox);

    addAndMakeVisible(closeButton);

    updateTheme(juce::String(initialColorKey));
}

void SettingsPage::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &themeComboBox)
    {
        juce::String newThemeName = themeComboBox.getText();

        updateTheme(newThemeName);

        listeners.call(&Listener::themeSettingChanged, newThemeName);
    }
    else if (comboBoxThatHasChanged == &languageComboBox)
    {
        juce::String newLanguageName = languageComboBox.getText();

        listeners.call(&Listener::languageSettingChanged, newLanguageName);
    }
}

void SettingsPage::updateTheme(const juce::String& themeName)
{
    std::string key = themeName.toStdString();
    if (colorMap.count(key) > 0)
    {
        currentBackgroundColour = colorMap.at(key);

        repaint();
        resized();
    }

    juce::Colour textColour = ((themeName == "Dark") ? juce::Colours::white : juce::Colours::black);
    themeLabel.setColour(juce::Label::textColourId, textColour);
    languageLabel.setColour(juce::Label::textColourId, textColour);

    repaint();
}

void SettingsPage::paint(juce::Graphics& g)
{
    g.fillAll(currentBackgroundColour);

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds().toFloat(), 1.0f);
}

void SettingsPage::resized()
{
    auto area = getLocalBounds().reduced(20);

    int labelWidth = 100;
    int controlHeight = 30;
    int padding = 15;

    auto themeArea = area.removeFromTop(controlHeight);
    themeLabel.setBounds(themeArea.removeFromLeft(labelWidth));
    themeComboBox.setBounds(themeArea);

    area.removeFromTop(padding);

    auto languageArea = area.removeFromTop(controlHeight);
    languageLabel.setBounds(languageArea.removeFromLeft(labelWidth));
    languageComboBox.setBounds(languageArea);

    closeButton.setBounds(getLocalBounds().reduced(10).removeFromBottom(controlHeight).removeFromRight(80));
}

void SettingsPage::addListener(Listener* listener)
{
    listeners.add(listener);
}

void SettingsPage::removeListener(Listener* listener)
{
    listeners.remove(listener);
}