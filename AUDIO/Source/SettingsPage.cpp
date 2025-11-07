#include "SettingsPage.h"

SettingsPage::SettingsPage(PlayerAudio* playerRef, const std::string& initialColorKey)
    : player(playerRef),
    currentColorKey(initialColorKey),
    currentBackgroundColour(colorMap.count(initialColorKey) ? colorMap.at(initialColorKey) : colorMap.at("Dark"))
{
    themeComboBox.addItem("Dark", 1);
    themeComboBox.addItem("Light", 2);
    themeComboBox.setText(juce::String(initialColorKey), juce::dontSendNotification);
    themeComboBox.addListener(this);

    languageComboBox.addItem("English", 1);
    languageComboBox.addItem("Arabic", 2);
    languageComboBox.setSelectedId(1);
    languageComboBox.addListener(this);

    sleepLabel.setText("Sleep Timer:", juce::dontSendNotification);
    sleepLabel.setJustificationType(juce::Justification::centredLeft);

    sleepSlider.setRange(1, 120, 1);
    sleepSlider.setValue(30);
    sleepSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);

    startTimerButton.setButtonText("Start");
    pauseTimerButton.setButtonText("Pause");
    resumeTimerButton.setButtonText("Resume");
    cancelTimerButton.setButtonText("Cancel");

    startTimerButton.onClick = [this]() {
        int minutes = static_cast<int>(sleepSlider.getValue());
        player->startSleepTimer(minutes);
        };

    pauseTimerButton.onClick = [this]() {
        player->pauseSleepTimer();
        };

    resumeTimerButton.onClick = [this]() {
        player->resumeSleepTimer();
        };

    cancelTimerButton.onClick = [this]() {
        player->cancelSleepTimer();
        };

    addAndMakeVisible(themeLabel);
    addAndMakeVisible(themeComboBox);
    addAndMakeVisible(languageLabel);
    addAndMakeVisible(languageComboBox);
    addAndMakeVisible(sleepLabel);
    addAndMakeVisible(sleepSlider);
    addAndMakeVisible(startTimerButton);
    addAndMakeVisible(pauseTimerButton);
    addAndMakeVisible(resumeTimerButton);
    addAndMakeVisible(cancelTimerButton);
    addAndMakeVisible(closeButton);

    updateTheme(juce::String(initialColorKey));
}

void SettingsPage::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &themeComboBox)
    {
        juce::String displayedText = themeComboBox.getText();
        int selectedId = themeComboBox.getSelectedId();

        juce::String englishThemeName;
        if (selectedId == 1)
            englishThemeName = "Dark";
        else if (selectedId == 2)
            englishThemeName = "Light";
        else
            englishThemeName = "Dark";

        updateTheme(englishThemeName);
        listeners.call(&Listener::themeSettingChanged, englishThemeName);
    }
    else if (comboBoxThatHasChanged == &languageComboBox)
    {
        juce::String newLanguageName = languageComboBox.getText();
        updateLanguage(newLanguageName);
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

    juce::Colour textColour = (themeName == "Dark") ? juce::Colours::white : juce::Colours::black;
    juce::Colour accentColour = (themeName == "Dark") ? juce::Colour::fromRGB(150, 100, 255) : juce::Colour::fromRGB(0x00, 0x78, 0xD4);
    juce::Colour controlBgColor = colorMap.at(currentColorKey).brighter(0.2f);


    themeLabel.setColour(juce::Label::textColourId, textColour);
    languageLabel.setColour(juce::Label::textColourId, textColour);
    sleepLabel.setColour(juce::Label::textColourId, textColour);

    closeButton.setColour(juce::TextButton::textColourOffId, textColour);
    closeButton.setColour(juce::TextButton::buttonColourId, currentBackgroundColour.brighter(0.1f));

    themeComboBox.setColour(juce::ComboBox::backgroundColourId, controlBgColor);
    themeComboBox.setColour(juce::ComboBox::textColourId, textColour);
    languageComboBox.setColour(juce::ComboBox::backgroundColourId, controlBgColor);
    languageComboBox.setColour(juce::ComboBox::textColourId, textColour);

    auto updateTimerButton = [&](juce::TextButton& button) {
        button.setColour(juce::TextButton::buttonColourId, controlBgColor.darker(0.1f));
        button.setColour(juce::TextButton::buttonOnColourId, accentColour.withAlpha(0.7f));
        button.setColour(juce::TextButton::textColourOffId, textColour);
        button.setColour(juce::TextButton::textColourOnId, textColour);
        };
    updateTimerButton(startTimerButton);
    updateTimerButton(pauseTimerButton);
    updateTimerButton(resumeTimerButton);
    updateTimerButton(cancelTimerButton);

    sleepSlider.setColour(juce::Slider::thumbColourId, accentColour);
    sleepSlider.setColour(juce::Slider::trackColourId, controlBgColor.darker(0.1f));
    sleepSlider.setColour(juce::Slider::textBoxTextColourId, textColour);
    sleepSlider.setColour(juce::Slider::textBoxBackgroundColourId, controlBgColor.darker(0.1f));

    repaint();
}


void SettingsPage::updateLanguage(const juce::String& newLanguageName)
{
    const juce::String& langKey = (LanguageMap.count(newLanguageName)
        ? newLanguageName
        : juce::String("English"));

    const auto& langMap = LanguageMap.at(langKey);

    auto getTranslation = [&](const juce::String& key) -> juce::String {
        if (langMap.count(key)) {
            return langMap.at(key);
        }
        return key;
        };

    themeLabel.setText(getTranslation("Theme:"), juce::dontSendNotification);
    languageLabel.setText(getTranslation("Language:"), juce::dontSendNotification);
    sleepLabel.setText(getTranslation("Sleep Timer:"), juce::dontSendNotification);

    startTimerButton.setButtonText(getTranslation("Start"));
    pauseTimerButton.setButtonText(getTranslation("Pause"));
    resumeTimerButton.setButtonText(getTranslation("Resume"));
    cancelTimerButton.setButtonText(getTranslation("Cancel"));
    closeButton.setButtonText(getTranslation("Close"));

    themeComboBox.changeItemText(1, getTranslation("Dark"));
    themeComboBox.changeItemText(2, getTranslation("Light"));
    languageComboBox.changeItemText(1, getTranslation("English"));
    languageComboBox.changeItemText(2, getTranslation("Arabic"));

    sleepSlider.setTextValueSuffix(" " + getTranslation("minutes"));

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

    // Theme
    auto themeArea = bounds.removeFromTop(controlHeight);
    themeLabel.setBounds(themeArea.removeFromLeft(labelWidth));
    themeComboBox.setBounds(themeArea.reduced(5));

    bounds.removeFromTop(padding);

    // Language
    auto languageArea = bounds.removeFromTop(controlHeight);
    languageLabel.setBounds(languageArea.removeFromLeft(labelWidth));
    languageComboBox.setBounds(languageArea.reduced(5));

    bounds.removeFromTop(padding * 2);

    // Sleep Timer
    auto sleepArea = bounds.removeFromTop(controlHeight);
    sleepLabel.setBounds(sleepArea.removeFromLeft(labelWidth));
    sleepSlider.setBounds(sleepArea.reduced(5));

    bounds.removeFromTop(padding);

    auto buttonRow = bounds.removeFromTop(controlHeight);
    int buttonWidth = 150;
    startTimerButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(3));
    pauseTimerButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(3));
    resumeTimerButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(3));
    cancelTimerButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(3));

    auto closeArea = getLocalBounds().reduced(10).removeFromBottom(controlHeight);
    closeButton.setBounds(closeArea.removeFromRight(80));
}

void SettingsPage::addListener(Listener* listener)
{
    listeners.add(listener);
}

void SettingsPage::removeListener(Listener* listener)
{
    listeners.remove(listener);
}
