#include "PlayerGUI.h"


void PlayerGUI::initializeControls()
{
    // Button set up
    for (auto* btn : buttons)
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }
    // Label set up
    for (auto& [msg, textLabel] : labels) {
        textLabel->setText(msg, juce::dontSendNotification);
        textLabel->setFont(juce::Font(16.0f, juce::Font::bold));
        textLabel->setJustificationType(juce::Justification::centredLeft);
        textLabel->setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(textLabel);
    }

    playlistListBox.setModel(this);
    addAndMakeVisible(playlistListBox);
    playlistListBox.setVisible(false);
    addAndMakeVisible(addlistButton);
    addlistButton.addListener(this);
    addlistButton.setVisible(false);
    addAndMakeVisible(deletelistButton);
    deletelistButton.addListener(this);
    deletelistButton.setVisible(false);

    refreshPlaylistDisplay();

    // Slider set up
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
    control->setSignalListener(this);

    // Position Slider Setup
    positionSlider.setRange(0.0, control->getLength());
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);
    positionSlider.setNumDecimalPlacesToDisplay(2);

    setSize(500, 250);
}

int PlayerGUI::getNumRows() {
    return (int)currentPlaylist.size();
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) {
    if (rowNumber < getNumRows()) {
        std::string songName = currentPlaylist[rowNumber].first;
        g.setColour(rowIsSelected ? juce::Colours::cyan : juce::Colours::white);
        g.drawText(songName, 5, 0, width - 10, height, juce::Justification::centredLeft, true);
    }
}

void PlayerGUI::refreshPlaylistDisplay() {
    currentPlaylist = control->getPlaylistManager().getSongs("front");
    playlistListBox.updateContent();
}

void PlayerGUI::updateSelectedRow() {
    if (currentKey.empty())
    {
        playlistListBox.deselectAllRows();
        return;
    }

    int rowIndexToSelect = -1;
    for (auto i = 0; i < currentPlaylist.size(); ++i)
    {
        if (currentPlaylist[i].first == currentKey)
        {
            rowIndexToSelect = i;
            break;
        }
    }

    if (rowIndexToSelect >= 0)
    {
        playlistListBox.deselectAllRows();

        playlistListBox.selectRow(rowIndexToSelect, true, false);
    }
}

void PlayerGUI::loadNextTrack() {
    string nextKey = control->getPlaylistManager().getNext(currentKey);

    if (nextKey == "END" || nextKey.empty()) {
        if (control->isLooping()) {
            nextKey = control->getPlaylistManager().getNext("front");

            if (nextKey == "END" || nextKey.empty()) {
                control->stop();
                currentKey = "";
                playButton.setButtonText("Play");
                return;
            }
        }
        else {
            control->stop();
            currentKey = "";
            playButton.setButtonText("Play");
            return;
        }
    }

    std::string nextPath = control->getPlaylistManager().getUrl(nextKey);

    juce::File nextFile(nextPath);
    control->startNew(nextFile);

    currentKey = nextKey;
    name.setText(control->getName(), juce::dontSendNotification);
    title.setText(control->getTitle(), juce::dontSendNotification);
    duration.setText(control->getDuration(), juce::dontSendNotification);


    playButton.setButtonText("Pause ||");
    stoped = false;
    positionSlider.setRange(0.0, control->getLength(), 0.01);
    positionSlider.setValue(0.0);
    updateSelectedRow();
}


PlayerGUI::PlayerGUI() : control(nullptr)
{
    initializeControls();
    startTimerHz(100);
}

PlayerGUI::PlayerGUI(PlayerAudio& control) : control(&control)
{
    initializeControls();
    startTimerHz(100);

}

void PlayerGUI::paint(juce::Graphics& g) {
    juce::Colour darkForest = juce::Colour::fromRGB(10, 25, 20);
    juce::Colour tealGlow = juce::Colour::fromRGB(30, 90, 80);

    juce::ColourGradient gradient( /* بتخلى الالوان mix بدرجه 45  */
        darkForest, 0, 0,
        tealGlow, (float)getWidth(), (float)getHeight(),
        false
    );

    g.setGradientFill(gradient);
    g.fillRect(getLocalBounds());
}
void PlayerGUI::resized() {
    auto bounds = getLocalBounds();

    juce::Rectangle<int> playerArea = bounds;
    juce::Rectangle<int> sidebarArea;

    if (playlistSidebarVisible)
    {
        playerArea = bounds.removeFromLeft(bounds.getWidth() * 0.7);
        sidebarArea = bounds;
        auto sidebarButtonRow = sidebarArea.removeFromTop(30).reduced(5);

        addlistButton.setBounds(sidebarButtonRow.removeFromLeft(sidebarButtonRow.getWidth() / 2).reduced(2));
        deletelistButton.setBounds(sidebarButtonRow.reduced(2));

        playlistListBox.setBounds(sidebarArea.reduced(5));
    }

    auto buttonRow = playerArea.removeFromTop(40).reduced(5);

    auto buttonWidth = buttonRow.getWidth() / buttons.size();

    loadButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));
    restartButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));
    stopButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));
    playButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));
    muteButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));
    go_to_startButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));
    go_to_endButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));
    speedButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));
    repeatButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));
    playlistButton.setBounds(buttonRow.removeFromLeft(buttonWidth).reduced(4));


    auto volumeSliderArea = playerArea.removeFromTop(50).reduced(5);
    volumeSlider.setBounds(volumeSliderArea.reduced(5));

    auto positionSliderArea = playerArea.removeFromTop(50).reduced(5);
    positionSlider.setBounds(positionSliderArea.reduced(5));

    auto nameLabelArea = playerArea.removeFromTop(30).reduced(0, 5);
    author.setBounds(nameLabelArea.removeFromLeft(70).reduced(5));
    name.setBounds(nameLabelArea.reduced(5));

    auto titleLabelArea = playerArea.removeFromTop(30).reduced(0, 5);
    songTitle.setBounds(titleLabelArea.removeFromLeft(70).reduced(5));
    title.setBounds(titleLabelArea.reduced(5));

    auto durationArea = playerArea.removeFromTop(30).reduced(0, 5);
    durationHeader.setBounds(durationArea.removeFromLeft(70).reduced(5));
    duration.setBounds(durationArea.reduced(5));
}

void PlayerGUI::buttonClicked(juce::Button* button) {
    if (control == nullptr) return;

    if (button == &loadButton) {
        fileChooser = std::make_unique<juce::FileChooser>("Select an audio file...", juce::File{}, "*.wav;*.mp3");
        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc) {
                auto file = fc.getResult();
                if (file.existsAsFile()) 
                {
                    std::string newSongPath = file.getFullPathName().toStdString();
                    std::string newSongName = file.getFileNameWithoutExtension().toStdString();

                    std::string key = (currentPlaylist.empty() ? "front" : currentPlaylist.back().first);

                    control->getPlaylistManager().add(
                        key,
                        newSongName,
                        newSongPath
                    );

                    if (currentKey.empty()) currentKey = "front";

                    refreshPlaylistDisplay();
                    loadNextTrack();
                    control->start();

                    playButton.setButtonText("Pause ||");
                    stoped = false;
                    positionSlider.setRange(0.0, control->getLength(), 0.01);
                    positionSlider.setValue(0.0);
                }
            });
    }
    else if (button == &restartButton) {
        if (control->audioExist()) {
            control->restart();
        }
    }
    else if (button == &stopButton) {
        if (control->audioExist()) {
            control->stop();
            control->setPosition(0.0);
            stoped = true;
            playButton.setButtonText("Play");
        }
    }
    else if (button == &playButton) {
        if (!control->audioExist()) {
            if (currentKey.empty()) {
                currentKey = "front";
                loadNextTrack();
            }
            return;
        }
        if (stoped) {
            stoped = false;
            playButton.setButtonText("Pause ||");
            if (control->reachEnd()) control->restart();
            else control->start();
        }
        else {
            stoped = true;
            playButton.setButtonText("Play");
            control->stop();
        }
    }
    else if (button == &muteButton)
    {
        if (control->audioExist())
        {
            if (muted)
            {
                muted = false;
                volumeSlider.setValue(lastVal);
                control->setGain((double)lastVal);
                muteButton.setButtonText("Mute");
            }
            else
            {
                if (volumeSlider.getValue() > 0.001) {
                    lastVal = volumeSlider.getValue();
                }
                muted = true;
                volumeSlider.setValue(0.0);
                control->setGain(0.0);
                muteButton.setButtonText("Unmute");
            }
        }
    }
    else if (button == &repeatButton)
    {
        if (!control->audioExist()) {
            return;
        }
        control->toggleLooping();
        if (control->isLooping())
        {
            repeatButton.setButtonText("Repeat: ON");
        }
        else
        {
            repeatButton.setButtonText("Repeat: OFF");
        }
    }


    else if (button == &go_to_startButton) {
        if (control->audioExist()) {
            control->setPosition(0.0);
        }
    }
    else if (button == &go_to_endButton) {
        if (control->audioExist() && !currentKey.empty()) {
            loadNextTrack();
        }
        else if (control->audioExist()) {
            control->setPosition(control->getLength());
        }
    }
    else if (button == &repeatButton)
    {

        if (!control->audioExist()) {
            return;
        }
        control->toggleLooping();
        if (control->isLooping())
        {
            repeatButton.setButtonText("Repeat: ON");
        }
        else
        {
            repeatButton.setButtonText("Repeat: OFF");
        }
    }
    else if (button == &speedButton)
    {
        static int state = 0;
        state = (state + 1) % 4;

        double newSpeed = 1.0;
        if (state == 0) newSpeed = 1.0;
        else if (state == 1) newSpeed = 1.25;
        else if (state == 2) newSpeed = 1.5;
        else if (state == 3) newSpeed = 2.0;

        control->setSpeed(newSpeed);
        juce::String label = "Speed: " + juce::String(newSpeed, 2) + "X";
        speedButton.setButtonText(label);
    }
    else if (button == &playlistButton)
    {
        playlistSidebarVisible = !playlistSidebarVisible;
        playlistListBox.setVisible(playlistSidebarVisible);
        deletelistButton.setVisible(playlistSidebarVisible);
        addlistButton.setVisible(playlistSidebarVisible);
        resized();
    }
    else if (button == &addlistButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>("Select audio file to add...", juce::File{}, "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc) {
                auto file = fc.getResult();

                if (file.existsAsFile()) {
                    std::string newSongPath = file.getFullPathName().toStdString();
                    std::string newSongName = file.getFileNameWithoutExtension().toStdString();

                    std::string key = (currentPlaylist.empty() ? "front" : currentPlaylist.back().first);

                    control->getPlaylistManager().add(
                        key,
                        newSongName,
                        newSongPath
                    );

                    refreshPlaylistDisplay();
                }
            });
    }

    else if (button == &deletelistButton)
    {
        int selectedRow = playlistListBox.getSelectedRow();

        if (selectedRow >= 0 && selectedRow < (int)currentPlaylist.size())
        {
            string songToDelete = currentPlaylist[selectedRow].first;
            if (songToDelete == currentKey)
            {
                control->stop();
                control->setPosition(0.0);

                currentKey = "";
                stoped = true;
                playButton.setButtonText("Play");

                name.setText("Unknown", juce::dontSendNotification);
                title.setText("No Track Loaded", juce::dontSendNotification);
                duration.setText("00:00", juce::dontSendNotification);
                positionSlider.setRange(0.0, 1.0, 0.01);
                positionSlider.setValue(0.0);
                control->reset();
            }

            control->getPlaylistManager().remove(songToDelete);

            refreshPlaylistDisplay();
        }
    }
}

void PlayerGUI::selectedRowsChanged(int lastRowSelected)
{
    if (lastRowSelected >= 0 && lastRowSelected < (int)currentPlaylist.size())
    {
        std::string selectedKey = currentPlaylist[lastRowSelected].first;
        std::string selectedPath = currentPlaylist[lastRowSelected].second;

        currentKey = selectedKey;

        juce::File fileToPlay(selectedPath);
        control->startNew(fileToPlay);
        name.setText(control->getName(), juce::dontSendNotification);
        title.setText(control->getTitle(), juce::dontSendNotification);
        duration.setText(control->getDuration(), juce::dontSendNotification);
        positionSlider.setRange(0.0, control->getLength(), 0.01);
        positionSlider.setValue(0.0);


        playButton.setButtonText("Pause ||");
        stoped = false;
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider) {
    if (control == nullptr) return;

    if (slider == &volumeSlider)
        control->setGain((double)slider->getValue());
}


void PlayerGUI::sliderDragStarted(juce::Slider* slider)
{
    if (slider == &positionSlider)
    {
        isUserDraggingPosition = true;
    }
}



void PlayerGUI::sliderDragEnded(juce::Slider* slider) {
    if (control == nullptr) return;

    if (slider == &volumeSlider) {
        if ((double)slider->getValue() < 1e-8) {
            muted = true;
            return;
        }

        lastVal = (double)slider->getValue();
    }

    //posision 
    if (slider == &positionSlider)
    {
        if (control != nullptr && control->audioExist())
        {
            control->setPosition(slider->getValue());
            if (control->reachEnd()) {
                playBackFinished();
            }
        }
        isUserDraggingPosition = false;
        return;
    }
}
void PlayerGUI::timerCallback()
{
    if (control == nullptr || isUserDraggingPosition)
        return;

    if (control != nullptr && control->audioExist())
    {
        // later this will track playback position
        double pos = control->getAudioPosition();
        positionSlider.setValue(pos, juce::dontSendNotification);
        if (control->reachEnd()) {
            playBackFinished();
        }
        ////handling time units convertions
        //int totalMillis = static_cast<int>(pos * 1000.0); // convert to ms
        //int minutes = totalMillis / 60000;
        //int seconds = (totalMillis % 60000) / 1000;
        //int millis = totalMillis % 1000;

        //juce::String formattedTime = juce::String::formatted("%02d:%02d:%03d", minutes, seconds, millis);

        //
        //juce::Logger::outputDebugString("Position: " + formattedTime);
    }
}

void PlayerGUI::playBackFinished() {
    playButton.setButtonText("Play");
    stoped = true;

    loadNextTrack();
}

juce::TextButton* PlayerGUI::getButton(std::string s) {
    if (mp.find(s) == mp.end()) return nullptr;
    return mp[s];
}

juce::Slider& PlayerGUI::getVolume()
{
    return volumeSlider;
}

juce::Slider& PlayerGUI::getPosition()
{
    return positionSlider;

}
