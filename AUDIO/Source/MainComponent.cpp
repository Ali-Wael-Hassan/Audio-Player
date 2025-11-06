#include "MainComponent.h"

MainComponent::MainComponent()
{
    audio1 = new PlayerAudio();
    GUI1 = new PlayerGUI(*audio1, "", "");
    audio2 = new PlayerAudio();
    GUI2 = new PlayerGUI(*audio2, "", "");

    home = new Home("Dark", "English", GUI1, audio1, GUI2, audio2);

    addAndMakeVisible(home);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    delete audio1;
    delete GUI1;
    delete audio2;
    delete GUI2;
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audio1->prepareToPlay(samplesPerBlockExpected, sampleRate);
    audio2->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    audio1->getNextAudioBlock(bufferToFill);

    juce::AudioBuffer<float> tempBuffer(
        bufferToFill.buffer->getNumChannels(),
        bufferToFill.numSamples
    );

    juce::AudioSourceChannelInfo tempInfo;
    tempInfo.buffer = &tempBuffer;
    tempInfo.numSamples = bufferToFill.numSamples;
    tempInfo.startSample = 0;

    audio2->getNextAudioBlock(tempInfo);

    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        bufferToFill.buffer->addFrom(
            channel,
            bufferToFill.startSample,
            tempBuffer,
            channel,
            0,
            bufferToFill.numSamples
        );
    }
}

void MainComponent::releaseResources()
{
    audio1->releaseResources();
    audio2->releaseResources();
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
    GUI1->sliderValueChanged(slider);
    GUI2->sliderValueChanged(slider);
}