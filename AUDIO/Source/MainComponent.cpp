#include "MainComponent.h"

MainComponent::MainComponent()
{
    audio = new PlayerAudio();
    GUI = new PlayerGUI(*audio);

    addAndMakeVisible(GUI);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
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
    GUI->paint(g);
}
void MainComponent::resized()
{
    GUI->setBounds(getLocalBounds());

    GUI->resized();
}

void MainComponent::buttonClicked(juce::Button* button)
{
    GUI->buttonClicked(button);
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    GUI->sliderValueChanged(slider);
}