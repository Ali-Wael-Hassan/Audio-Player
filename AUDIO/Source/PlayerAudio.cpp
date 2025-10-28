#include "PlayerAudio.h"


PlayerAudio::PlayerAudio() : playlist("x.txt") {
	formatManager.registerBasicFormats();

	resamplingSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false, 2);
}

PlayerAudio::~PlayerAudio() {
	transportSource.setSource(nullptr);
	resamplingSource.reset();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
	// transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	resamplingSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}


// edited the getNextAudioBlock for the looping purposes 
/*
void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {

	transportSource->getNextAudioBlock(bufferToFill); 

	resamplingSource->getNextAudioBlock(bufferToFill);
	if (transportSource.hasStreamFinished()) {
		transportSource.setPosition(0.0);
		transportSource.start();
	}
}
*/

//Here's the Edited version :


void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
	resamplingSource->getNextAudioBlock(bufferToFill);
	if (loopActive && transportSource.hasStreamFinished()) {
		transportSource.setPosition(0.0);
		transportSource.start();
	}
}




void PlayerAudio::releaseResources() {
	// transportSource.releaseResources();
	resamplingSource->releaseResources();
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
	titleText = "No Track Loaded";
	nameText = "Unknown";
	durationText = "00:00";
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

		juce::String artistKeys[] = { "TPE1", "ARTIST", "IART" };
		juce::String artist;

		for (auto& key : artistKeys)
		{
			artist = reader->metadataValues[key].trim();
			if (artist.isNotEmpty())
				break;
		}

		nameText = (artist.isNotEmpty() ? artist : "Unknown Artist");

		juce::String titleKeys[] = { "TIT2", "TITLE", "INAM" };
		juce::String title;

		for (auto& key : titleKeys)
		{
			title = reader->metadataValues[key].trim();
			if (title.isNotEmpty())
				break;
		}

		readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
		transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
		titleText = (title.isNotEmpty() ? title : file.getFileNameWithoutExtension());

		int duration = transportSource.getLengthInSeconds();

		int hours = duration / 3600;
		duration %= 3600;
		int minutes = duration / 60;
		duration %= 60;
		int seconds = duration;

		juce::String time = "";
		if (hours > 0) time += juce::String::formatted("%d:", hours);

		time += juce::String::formatted("%02d:%02d", minutes, seconds);

		durationText = (time.isNotEmpty() ? time : "0:00");
		transportSource.start();
	}
}

void PlayerAudio::setPosition(double pos) {
	transportSource.setPosition(pos);
}

void PlayerAudio::setSpeed(double ratio)
{
	if (resamplingSource)
	{
		resamplingSource->setResamplingRatio(ratio);
		DBG("Speed changed to: " << ratio);
	}
}

double PlayerAudio::getAudioPosition()
{
	return transportSource.getCurrentPosition();
}

double PlayerAudio::getAudioLength()
{
	return transportSource.getLengthInSeconds();
}

double PlayerAudio::getLength() {
	return transportSource.getLengthInSeconds();
}


void PlayerAudio::toggleLooping()
{
	loopActive = !loopActive;

}

bool PlayerAudio::isLooping() const
{
	return loopActive;
}



juce::String PlayerAudio::getName() {
	return nameText;
}

juce::String PlayerAudio::getTitle() {
	return titleText;
}

juce::String PlayerAudio::getDuration() {
	return durationText;
}

PlaylistManager& PlayerAudio::getPlaylistManager() {
	return playlist;
}

void PlayerAudio::setSignalListener(PlayerAudioSignal* l) {
	listen = l;
}

bool PlayerAudio::reachEnd() {
	return transportSource.hasStreamFinished();
}