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
<<<<<<< HEAD
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();
=======
	transportSource.stop();
	transportSource.setSource(nullptr);
	titleText = "No Track Loaded";
	nameText = "Artist: Unknown";
	durationText = "-1";
	readerSource.reset();
>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
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
<<<<<<< HEAD
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
=======
	if (auto* reader = formatManager.createReaderFor(file)) {
		reset();

		juce::String artistKeys[] = { "IART" };
		juce::String artist;

		for (auto& key : artistKeys)
		{
			artist = reader->metadataValues[key].trim();
			if (artist.isNotEmpty())
				break;
		}

		nameText = (artist.isNotEmpty() ? artist : "Unknown Artist");

		juce::String titleKeys[] = { "INAM" };
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
		if (hours != 0) time += juce::String(hours) + " : ";
		if (minutes > 0) time += juce::String(minutes) + " : ";
		else if(hours > 0) time += "00 : ";
		if (seconds > 0) time += juce::String(seconds);
		else if(minutes > 0 || hours > 0) time += "00 : ";

		durationText = (time.isNotEmpty() ? time : "-1");
		transportSource.start();
	}
}

void PlayerAudio::setPosition(double pos) {
	transportSource.setPosition(pos);
>>>>>>> 3931288ef8275e7716290f244e039e4f53808192
}

double PlayerAudio::getLength() {
	return transportSource.getLengthInSeconds();
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