#include "PlayerAudio.h"


PlayerAudio::PlayerAudio() {
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

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
	//transportSource->getNextAudioBlock(bufferToFill);
	resamplingSource->getNextAudioBlock(bufferToFill);
	if (loopActive && transportSource.hasStreamFinished()) {
		transportSource.setPosition(0.0);
		transportSource.start();
	}
	else if (transportSource.hasStreamFinished()) {
		if (listen) {
			listen->playBackFinished();
		}
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
	durationText = "-1";
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

		TagLib::FileRef openFile(file.getFullPathName().toRawUTF8());

		juce::String artistText = "Unknown Artist";
		juce::String titleTempText = file.getFileNameWithoutExtension();
		const juce::StringArray artistKeys = { "TPE1", "ARTIST", "TCON", "PERFORMER", "IART"};
		const juce::StringArray titleKeys = { "TIT2", "TITLE", "INAM", "NAME" };
		for (const auto& key : artistKeys)
		{
			juce::String value = reader->metadataValues[key].trim();
			if (value.isNotEmpty()) {
				artistText = value.trim();
				break;
			}
		}

		for (const auto& key : titleKeys)
		{
			juce::String value = reader->metadataValues[key].trim();
			if (value.isNotEmpty()) {
				titleTempText = value.trim();
				break;
			}
		}

		if (!openFile.isNull() && openFile.tag()) {
			TagLib::Tag* tagObject = openFile.tag();

			juce::String artist = juce::String(tagObject->artist().toCString(true));
			if (artist.isNotEmpty()) {
				artistText = artist.trim();
			}

			juce::String title = juce::String(tagObject->title().toCString(true));
			if (title.isNotEmpty()) {
				titleTempText = title.trim();
			}
		}

		nameText = artistText;
		titleText = titleTempText;

		readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
		transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

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

double PlayerAudio::getLength() {
	return transportSource.getLengthInSeconds();
}

juce::String PlayerAudio::getName() {
	return nameText;
}

void PlayerAudio::toggleLooping()
{
	loopActive = !loopActive;
}

bool PlayerAudio::isLooping() const
{
	return loopActive;
}

juce::String PlayerAudio::getTitle() {
	return titleText;
}

juce::String PlayerAudio::getDuration() {
	return durationText;
}

void PlayerAudio::setSignalListener(PlayerAudioSignal* l) {
	listen = l;
}

bool PlayerAudio::reachEnd() {
	return transportSource.hasStreamFinished();
}