#include "PlayerAudio.h"


PlayerAudio::PlayerAudio() {
	formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio() {
	transportSource.setSource(nullptr);
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
}

void PlayerAudio::startNew(juce::File file) {
	if (auto* reader = formatManager.createReaderFor(file)) {
		reset();
		readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
		transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
		transportSource.start();
	}
}

void PlayerAudio::move(double val) {
	double curPos = transportSource.getCurrentPosition();
	transportSource.setPosition(std::max(0.0, std::min(transportSource.getLengthInSeconds(), curPos + val)));
}
