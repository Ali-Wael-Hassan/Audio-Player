#include "SettingsPage.h"

SettingsPage::SettingsPage(const std::string& initialColorKey)
    : currentColorKey(initialColorKey),
    currentBackgroundColour(colorMap.count(initialColorKey) ? colorMap.at(initialColorKey) : colorMap.at("Dark"))
{
    // Theme box
    themeComboBox.addItem("Dark", 1);
    themeComboBox.addItem("Light", 2);
    // Find the ID corresponding to the initial color key and set it
    themeComboBox.setText(juce::String(initialColorKey), juce::dontSendNotification);
    themeComboBox.addListener(this);

    languageComboBox.addItem("English", 1);
    languageComboBox.addItem("Arabic", 2);
    languageComboBox.setSelectedId(1);
    languageComboBox.addListener(this);

    // Visibility
    themeLabel.setJustificationType(juce::Justification::centredLeft);
    languageLabel.setJustificationType(juce::Justification::centredLeft);

    addAndMakeVisible(themeLabel);
    addAndMakeVisible(themeComboBox);
    addAndMakeVisible(languageLabel);
    addAndMakeVisible(languageComboBox);
    addAndMakeVisible(closeButton);

    // Repaint
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
        currentColorKey = key;
    }

    juce::Colour textColour = (key == "Dark") ? juce::Colours::white : juce::Colours::black;

    themeLabel.setColour(juce::Label::textColourId, textColour);
    languageLabel.setColour(juce::Label::textColourId, textColour);

    closeButton.setColour(juce::TextButton::textColourOffId, textColour);
    closeButton.setColour(juce::TextButton::buttonColourId, currentBackgroundColour.brighter(0.1f));

    juce::Colour controlBgColor = colorMap.at(currentColorKey);
    themeComboBox.setColour(juce::ComboBox::backgroundColourId, controlBgColor.brighter(0.2f));
    themeComboBox.setColour(juce::ComboBox::textColourId, textColour);
    languageComboBox.setColour(juce::ComboBox::backgroundColourId, controlBgColor.brighter(0.2f));
    languageComboBox.setColour(juce::ComboBox::textColourId, textColour);

    repaint();
}

void SettingsPage::paint(juce::Graphics& g)
{
    g.setColour(currentBackgroundColour);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 8.0f);

    g.setColour(juce::Colours::grey.darker(0.5f));
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 8.0f, 1.0f);
}

void SettingsPage::resized()
{
    auto bounds = getLocalBounds().reduced(20);

    int labelWidth = 100;
    int controlHeight = 30;
    int padding = 15;

    auto themeArea = bounds.removeFromTop(controlHeight);
    themeLabel.setBounds(themeArea.removeFromLeft(labelWidth));
    themeComboBox.setBounds(themeArea.reduced(5));

    bounds.removeFromTop(padding);

    auto languageArea = bounds.removeFromTop(controlHeight);
    languageLabel.setBounds(languageArea.removeFromLeft(labelWidth));
    languageComboBox.setBounds(languageArea.reduced(5));

    auto buttonArea = getLocalBounds().reduced(10).removeFromBottom(controlHeight);
    closeButton.setBounds(buttonArea.removeFromRight(80));
}

void SettingsPage::addListener(Listener* listener)
{
    listeners.add(listener);
}

void SettingsPage::removeListener(Listener* listener)
{
    listeners.remove(listener);
}