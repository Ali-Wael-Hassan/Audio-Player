#include "MainComponent.h"

MainComponent::MainComponent()
{
    audio = new PlayerAudio();
    GUI = new PlayerGUI(*audio, "", "");

    home = new Home("Dark", "English", GUI, audio);

    addAndMakeVisible(home);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    delete audio;
    delete GUI;
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audio->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    audio->getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    audio->releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    home->paint(g);
}
void MainComponent::resized()
{
    home->setBounds(getLocalBounds());

    home->resized();
}

void MainComponent::buttonClicked(juce::Button* button)
{
    home->buttonClicked(button);
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    GUI->sliderValueChanged(slider);
}