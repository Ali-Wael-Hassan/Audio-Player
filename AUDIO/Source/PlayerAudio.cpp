#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
    : playlist("UserPlaylist.txt")
{
    formatManager.registerBasicFormats();
    resamplingSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false, 2);
    sleepTimer = std::make_unique<SleepTimer>(*this);
}

PlayerAudio::~PlayerAudio() {
    transportSource.setSource(nullptr);
    resamplingSource.reset();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    resamplingSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    if (readerSource == nullptr) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    resamplingSource->getNextAudioBlock(bufferToFill);

    if (isMarkerLooping()) {
        double pos = getAudioPosition();
        if (pos > std::max(markers[getMarkerA()].position, markers[getMarkerB()].position)) {
            setPosition(std::min(markers[getMarkerA()].position, markers[getMarkerB()].position));
        }
        if (pos < std::min(markers[getMarkerA()].position, markers[getMarkerB()].position)) {
            setPosition(std::min(markers[getMarkerA()].position, markers[getMarkerB()].position));
        }
        return;
    }

    if (transportSource.hasStreamFinished())
    {
        if (loopActive == 1)
        {
            transportSource.setPosition(0.0);
            transportSource.start();
        }


        else
        {
            if (listen)
            {
                transportSource.stop();
                listen->playBackFinished();
            }
        }
    }
}


void PlayerAudio::releaseResources() {
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
    listen->playBackFinished();
}

void PlayerAudio::reset() {
    transportSource.stop();
    transportSource.setSource(nullptr);
    pathFile = "";
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

void PlayerAudio::startNewFromFile(juce::File file) {
    if (auto* reader = formatManager.createReaderFor(file)) {
        reset();
        pathFile = file.getFullPathName();

        TagLib::FileRef openFile(file.getFullPathName().toRawUTF8());

        juce::String artistText = "Unknown Artist";
        juce::String titleTempText = file.getFileNameWithoutExtension();
        const juce::StringArray artistKeys = { "TPE1", "ARTIST", "TCON", "PERFORMER", "IART" };
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
        
        listen->loadMetaData();
        listen->loadWave(file);
    }
}

juce::String PlayerAudio::startNewFromURL(juce::URL url)
{
    DBG("Attempting to download audio from: " << url.toString(false));

    juce::String filename = url.getFileName();
    if (filename.isEmpty())
        filename = "audio_download.mp3";

    juce::File audioDir = juce::File::getSpecialLocation(juce::File::userMusicDirectory)
        .getChildFile("MyAudioPlayer");
    audioDir.createDirectory();

    juce::File audioFile = audioDir.getChildFile(filename);

    if (audioFile.existsAsFile())
    {
        DBG("File already exists locally: " << audioFile.getFullPathName());
        return audioFile.getFullPathName();
    }

    std::unique_ptr<juce::InputStream> input(url.createInputStream(
        juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inAddress)
        .withConnectionTimeoutMs(15000)
        .withNumRedirectsToFollow(5)
    ));

    if (input == nullptr)
    {
        DBG("Failed to create input stream for URL: " << url.toString(false));
        return "";
    }

    DBG("Downloading to: " << audioFile.getFullPathName());

    std::unique_ptr<juce::FileOutputStream> output(audioFile.createOutputStream());
    if (output == nullptr || !output->openedOk())
    {
        DBG("Failed to open output stream for: " << audioFile.getFullPathName());
        return "";
    }

    output->writeFromInputStream(*input, -1);
    output->flush();
    output.reset();

    DBG("Download complete. File size: " << audioFile.getSize() << " bytes");

    return  audioFile.getFullPathName();
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
    loopActive = (loopActive + 1) % 4;
}

int PlayerAudio::isLooping() const
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

void PlayerAudio::jumpTime(double seconds)
{
    if (!audioExist()) return;

    double currentPos = getAudioPosition();
    double totalLength = getLength();
    double newPos = currentPos + seconds;

    newPos = juce::jlimit(0.0, totalLength, newPos);

    setPosition(newPos);
}

bool PlayerAudio::reachEnd() {
    return transportSource.hasStreamFinished();
}

void PlayerAudio::startSleepTimer(int seconds)
{
    if (sleepTimer)
        sleepTimer->start(seconds);
}

void PlayerAudio::pauseSleepTimer()
{
    if (sleepTimer)
        sleepTimer->pause();
}

void PlayerAudio::resumeSleepTimer()
{
    if (sleepTimer)
        sleepTimer->resume();
}

void PlayerAudio::cancelSleepTimer()
{
    if (sleepTimer)
        sleepTimer->cancel();
}

bool PlayerAudio::isSleepTimerRunning() const
{
    return sleepTimer && sleepTimer->isRunning();
}