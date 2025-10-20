#include "PlayerAudio.h"

PlayerAudio::PlayerAudio() {
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio() {
    transportSource.setSource(nullptr);  /*to prevent memoryleak*/
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    transportSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources() {
    transportSource.releaseResources();
    readerSource.reset();
}

bool PlayerAudio::audioExist() {
    return readerSource.get() != nullptr;
}

void PlayerAudio::start() {
    transportSource.start();
}

void PlayerAudio::stop() {
    transportSource.stop();
}

void PlayerAudio::reset() {
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();
}

void PlayerAudio::restart() {
    transportSource.setPosition(0.0);
    transportSource.start();
}

void PlayerAudio::setGain(float val) {
    transportSource.setGain(val);
    lastGain = val;
}

void PlayerAudio::startNew(juce::File file) {
    if (auto* reader = formatManager.createReaderFor(file)) {
        reset();
        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        readerSource->setLooping(isLooping);
        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
        transportSource.start();
    }
}

void PlayerAudio::move(double val) {
    if (readerSource != nullptr) {
        double curPos = transportSource.getCurrentPosition();
        double newPos = std::max(0.0, std::min(transportSource.getLengthInSeconds(), curPos + val));
        transportSource.setPosition(newPos);
    }
}

double PlayerAudio::getCurrentPosition() const {
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLengthInSeconds() const {
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::setPosition(double newPositionInSeconds) {
    if (readerSource != nullptr)
        transportSource.setPosition(newPositionInSeconds);
}

bool PlayerAudio::isPlaying() const {
    return transportSource.isPlaying();
}

void PlayerAudio::mute(bool shouldMute) {
    if (shouldMute) {
        transportSource.setGain(0.0f);
        isMuted = true;
    }
    else {
        transportSource.setGain(lastGain);
        isMuted = false;
    }
}

void PlayerAudio::setLooping(bool shouldLoop) {
    isLooping = shouldLoop;
    if (readerSource)
        readerSource->setLooping(shouldLoop);
}

bool PlayerAudio::getLooping() const {
    return isLooping;
}
