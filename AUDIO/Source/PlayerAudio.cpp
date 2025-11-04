#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
    : playlist("x.txt")
{
    formatManager.registerBasicFormats();
    resamplingSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false, 2);
}

PlayerAudio::~PlayerAudio() {
    transportSource.setSource(nullptr);
    resamplingSource.reset();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    resamplingSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    if (readerSource == nullptr) return;
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

void PlayerAudio::startNewFromFile(juce::File file) {
    if (auto* reader = formatManager.createReaderFor(file)) {
        reset();

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
        startNewFromFile(audioFile);
        return "";
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

void PlayerAudio::setMarkers(double position)
{
    if (!markerASet)
    {
        markerA = position;
        markerASet = true;
        DBG("Marker A set at: " << markerA);
    }
    else if (!markerBSet)
    {
        markerB = position;
        markerBSet = true;
        // ensure order
        if (markerB < markerA)
            std::swap(markerA, markerB);

        loopingAB = true; // both set, enable looping
        DBG("Marker B set at: " << markerB << " | A�B Looping Enabled");
    }
    else
    {
        // reset if pressed again
        markerASet = markerBSet = false;
        loopingAB = false;
        markerA = markerB = -1.0;
    }
}



bool PlayerAudio::isLoopingAB()const
{
    return loopingAB;
}


bool PlayerAudio::MarkerASet() const
{
    return markerASet;
}

bool PlayerAudio::MarkerBSet() const
{
    return markerBSet;
}

double PlayerAudio::getMarkerA()
{
    return markerA;
}

double PlayerAudio::getMarkerB()
{
    return markerB;
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