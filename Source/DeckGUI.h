/*
  ==============================================================================

    DeckGUI.h

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "CustomComponent.h"
using namespace juce;

//==============================================================================

class DeckGUI  : public Component,
                 public Button::Listener,
                 public Slider::Listener,
                 public FileDragAndDropTarget,
                 public Timer   
{
public:
    DeckGUI(int _id,
            DJAudioPlayer* player, 
            AudioFormatManager& formatManager, 
            AudioThumbnailCache& thumbCache);
    ~DeckGUI() override;

    void paint (Graphics&) override;
    void resized() override;

    // Implements Button::Listener
    void buttonClicked(Button* button) override;

    // Implements Slider::Listener
    void sliderValueChanged(Slider* slider) override;

    // Detects if the file is being dragged over the deck
    bool isInterestedInFileDrag(const StringArray& files) override;

    // Listens for changes to the waveform
    void timerCallback() override;

    // Detects if file is dropped onto the deck
    void filesDropped(const StringArray &files, int x, int y) override;

private:    
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };

    Slider volSlider;
    Label volLabel;

    Slider speedSlider;
    Label speedLabel;

    Slider posSlider;
    Label posLabel;

    void loadFile(URL audioURL);
    int id;

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    SharedResourcePointer< TooltipWindow > sharedTooltip;

    friend class PlaylistComponent;

    CustomComponent customComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
