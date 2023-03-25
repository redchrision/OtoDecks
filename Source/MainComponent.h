/*
  ==============================================================================

    MainComponent.h

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include "PlaylistComponent.h"
using namespace juce;

//==============================================================================

class MainComponent  : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================

    DeckGUI deckGUI1{1, &player1, formatManager, thumbCache};
    DeckGUI deckGUI2{2, &player2, formatManager, thumbCache};

    DJAudioPlayer player1{ formatManager };
    DJAudioPlayer player2{ formatManager };
    DJAudioPlayer playerForParsingMetaData{ formatManager };

    PlaylistComponent playlistComponent{ &deckGUI1, &deckGUI2, &playerForParsingMetaData };
    MixerAudioSource mixerSource;

    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{ 100 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
