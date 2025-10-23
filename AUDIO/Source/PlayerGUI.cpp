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
    
    // Slider set up
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    setSize(500, 250);

}

PlayerGUI::PlayerGUI() : control(nullptr)
{
    initializeControls();
}

PlayerGUI::PlayerGUI(PlayerAudio& control) : control(&control)
{
    initializeControls();
}

void PlayerGUI::paint(juce::Graphics& g) {

    /***********************************************************/

    /******
    kareem ahmed

    change background to GradientFill to mix two color


    *******/
    juce::Colour darkForest = juce::Colour::fromRGB(10, 25, 20);
    juce::Colour tealGlow = juce::Colour::fromRGB(30, 90, 80);

    juce::ColourGradient gradient( /* بتخلى الالوان mix بدرجه 45  */
        darkForest, 0, 0,
        tealGlow, (float)getWidth(), (float)getHeight(),
        false
    );

    g.setGradientFill(gradient);
    g.fillRect(getLocalBounds());


    /***********************************************************/


}
void PlayerGUI::resized() {
    auto bounds = getLocalBounds();

    auto buttonRow = bounds.removeFromTop(40).reduced(5);

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


    auto volumeSliderArea = bounds.removeFromTop(50).reduced(5);
    volumeSlider.setBounds(volumeSliderArea.reduced(5));

    auto nameLabelArea = bounds.removeFromTop(30).reduced(0, 5);
    author.setBounds(nameLabelArea.removeFromLeft(70).reduced(5));
    name.setBounds(nameLabelArea.reduced(5));

    auto titleLabelArea = bounds.removeFromTop(30).reduced(0, 5);
    songTitle.setBounds(titleLabelArea.removeFromLeft(70).reduced(5));
    title.setBounds(titleLabelArea.reduced(5));

    auto durationArea = bounds.removeFromTop(30).reduced(0, 5);
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
                if (file.existsAsFile()) {
                    control->startNew(file);
                    name.setText(control->getName(), juce::dontSendNotification);
                    title.setText(control->getTitle(), juce::dontSendNotification);
                    duration.setText(control->getDuration(), juce::dontSendNotification);
                    if (control->audioExist()) {
                        playButton.setButtonText("Pause ||");
                        stoped = false;
                    }
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
        if (!control->audioExist()) return;
        if (stoped) {
            stoped = false;
            playButton.setButtonText("Pause ||");
            control->start();
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
                muteButton.setButtonText("Mute");// next update you will add icon next to title 
            }
            else
            {
                if (volumeSlider.getValue() > 0.001) {
                    lastVal = volumeSlider.getValue();
                }
                muted = true;
                volumeSlider.setValue(0.0);
                control->setGain(0.0);
                muteButton.setButtonText("Unmute");// next update you will add icon next to title 
            }
        }
    }
    else if (button == &go_to_startButton) {
        if (control->audioExist()) {
            control->setPosition(0.0);
        }
    }
    else if (button == &go_to_endButton) {
        if (control->audioExist()) {
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
    /*next update*/
    else if (button == &speedButton)
    {
        static int state = 0;
        state = (state + 1) % 4; //  0 → 1 → 2 → 3 → 0 

        double newSpeed = 1.0;
        if (state == 0) newSpeed = 1.0;
        else if (state == 1) newSpeed = 1.25;
        else if (state == 2) newSpeed = 1.5;
        else if (state == 3) newSpeed = 2.0;

        control->setSpeed(newSpeed);
        juce::String label = "Speed: " + juce::String(newSpeed, 2) + "X";
        speedButton.setButtonText(label);
    }
    /**************************************************************/

}

void PlayerGUI::sliderValueChanged(juce::Slider* slider) {
    if (control == nullptr) return;

    if (slider == &volumeSlider)
        control->setGain((double)slider->getValue());
}
/*********************kareem*****************************/

void PlayerGUI::sliderDragEnded(juce::Slider* slider) {
    if (control == nullptr) return;

    if (slider == &volumeSlider) {
        if ((double)slider->getValue() < 1e-8) {
            muted = true;
            return;
        }

        lastVal = (double)slider->getValue();
    }
}
/*************************************************/

juce::TextButton& PlayerGUI::getLoad()
{
    return loadButton;
}

juce::TextButton& PlayerGUI::getRestart()
{
    return restartButton;
}

juce::TextButton& PlayerGUI::getStop()
{
    return stopButton;
}

juce::Slider& PlayerGUI::getVolume()
{
    return volumeSlider;
}