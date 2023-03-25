/*
  ==============================================================================

    DeckGUI.cpp

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(int _id,
                 DJAudioPlayer* _player,
                 AudioFormatManager& formatManager,
                 AudioThumbnailCache& thumbCache
                 ) : player(_player),
                     id(_id),
                     waveformDisplay(id, formatManager, thumbCache)
{
    // add all components and make visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(waveformDisplay);

    //style

    volSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volSlider.setLookAndFeel(&customComponent);

    speedSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    speedSlider.setLookAndFeel(&customComponent);

    posSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    posSlider.setLookAndFeel(&customComponent);

    // add listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    //configure volume slider and label
    double volDefaultValue = 2.5;
    volSlider.setRange(0.0, 5.0);
    volSlider.setNumDecimalPlacesToDisplay(3);
    volSlider.setTextBoxStyle(Slider::TextBoxBelow,
                              false, 
                              50, 
                              volSlider.getTextBoxHeight()
                             );
    volSlider.setValue(volDefaultValue);
    volSlider.setSkewFactorFromMidPoint(volDefaultValue);
    volLabel.setText("Volume", dontSendNotification);
    volLabel.setJustificationType(Justification::centredTop);
    volLabel.attachToComponent(&volSlider, false);

    //configure speed slider and label
    double speedDefaultValue = 1.0;
    speedSlider.setRange(0.10, 3.0);
    speedSlider.setNumDecimalPlacesToDisplay(3);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow,
                              false,
                              50,
                              speedSlider.getTextBoxHeight()
                             );
    speedSlider.setValue(speedDefaultValue);
    speedSlider.setSkewFactorFromMidPoint(speedDefaultValue);
    speedLabel.setText("Speed", dontSendNotification);
    speedLabel.setJustificationType(Justification::centredTop);
    speedLabel.attachToComponent(&speedSlider, false);

    //configure position slider and label
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(3);
    posSlider.setTextBoxStyle(Slider::TextBoxBelow,
                              false,
                              50,
                              posSlider.getTextBoxHeight()
                             );
    posLabel.setText("Position", dontSendNotification);
    posLabel.setJustificationType(Justification::centredTop);
    posLabel.attachToComponent(&posSlider, false);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::darkgrey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void DeckGUI::resized()
{
     /*This method is where you should set the bounds of any child
     components that your component contains..*/

    //                   x start, y start, width, height

    double rowH = getHeight() / 8;
    playButton.setBounds(0, rowH * 7.5, getWidth() / 4, rowH / 2);
    stopButton.setBounds(getWidth() / 4, rowH * 7.5, getWidth() / 4, rowH / 2);
    loadButton.setBounds(getWidth() / 2, rowH * 7.5, getWidth() / 2, rowH / 2);
    
    volSlider.setBounds(-30, rowH * 2.7, getWidth() / 2, rowH * 2.2);
    
    speedSlider.setBounds(getWidth() / 4, rowH * 5, getWidth() / 2, rowH * 2.2);
    
    posSlider.setBounds(getWidth() / 2 + 30, rowH * 2.7, getWidth() / 2, rowH * 2.2);
    
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        player->play();
    }
    if (button == &stopButton)
    {
        player->stop();
    }
    if (button == &loadButton)
    {
        FileChooser chooser{"Select a file"};
        if (chooser.browseForFileToOpen())
        {
            loadFile(URL{ chooser.getResult() });
        }
    }
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    if (files.size() == 1)
    {
        loadFile(URL{ File{files[0]} });
    }
}

void DeckGUI::loadFile(URL audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}

void DeckGUI::timerCallback()
{   
    //check if the relative position is greater than 0
    //otherwise loading file causes error
    if (player->getPositionRelative() > 0)
    {
        waveformDisplay.setPositionRelative(player->getPositionRelative());
    }
}
