#include "PlayerGUI.h"


void PlayerGUI::initializeControls()
{
    for (auto* btn : buttons)
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    setSize(500, 250);
}

PlayerGUI::PlayerGUI() : control(nullptr)
{
    initializeControls();
}

PlayerGUI::PlayerGUI(PlayerAudio& control) : control(&control)
{
    initializeControls();
}

void PlayerGUI::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::resized() {
    auto bounds = getLocalBounds();

    auto buttonRow = bounds.removeFromTop(40).reduced(5);

    auto buttonWidth = buttonRow.getWidth() / 3;

    loadButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));
    restartButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));
    stopButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));

    auto sliderArea = bounds.reduced(10);

    auto volumeSliderArea = sliderArea.removeFromTop(40);
    volumeSlider.setBounds(volumeSliderArea.reduced(5));
}

void PlayerGUI::buttonClicked(juce::Button* button) {
    if (control == nullptr) return;

    if (button == &loadButton) {
        fileChooser = std::make_unique<juce::FileChooser>("Select an audio file...", juce::File{}, "*.wav;*.mp3");
        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc) {
                auto file = fc.getResult();
                if (file.existsAsFile()) {
                    control->startNew(file);
                }
            });
    }
    else if (button == &restartButton) {
        if (control->audioExist()) {
            control->restart();
        }
    }
    else if (button == &stopButton) {
        if (control->audioExist()) {
            control->stop();
        }
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider) {
    if (control == nullptr) return;

    if (slider == &volumeSlider)
        control->setGain((float)slider->getValue());
}

juce::TextButton& PlayerGUI::getLoad()
{
    return loadButton;
}

juce::TextButton& PlayerGUI::getRestart()
{
    return restartButton;
}

juce::TextButton& PlayerGUI::getStop()
{
    return stopButton;
}

juce::Slider& PlayerGUI::getVolume()
{
    return volumeSlider;
}