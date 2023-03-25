/*
  ==============================================================================

    WaveformDisplay.h

  ==============================================================================
*/
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
using namespace juce;

//==============================================================================
/*
*/
class WaveformDisplay  : public Component,
                         public ChangeListener
{
public:
    WaveformDisplay(int _id,
                    AudioFormatManager& formatManager,
                    AudioThumbnailCache& thumbCache);
    ~WaveformDisplay() override;

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    void loadURL(URL audioURL);

    // Setting the relative position of the playhead
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    
    String fileName;
    int id;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
