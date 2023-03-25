/*
  ==============================================================================

    WaveformDisplay.cpp

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(int _id,
                                 AudioFormatManager& formatManager,
                                 AudioThumbnailCache& thumbCache
                                ) : audioThumb(1000, formatManager, thumbCache),
                                fileLoaded(false),
                                position(0),
                                id(_id)
                 
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId)); // clear the background
    g.setColour (Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
    g.setColour(Colours::orange);

    g.setFont(18.0f);
    g.drawText("Deck: " + std::to_string(id), getLocalBounds(),
            Justification::topLeft, true);

    if (fileLoaded)
    {
        g.setFont(15.0f);
        audioThumb.drawChannel(g, 
                               getLocalBounds(),
                               0,
                               audioThumb.getTotalLength(),
                               0,
                               1.0f
                              );
        g.setColour(Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
        g.setColour(Colours::white);
        g.drawText(fileName, getLocalBounds(),
            Justification::bottomLeft, true);
    }
    else
    {
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(),
            Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    repaint();
}

void WaveformDisplay::loadURL(URL audioURL)
{
    std::cout << "WaveformDisplay::loadURL called" << std::endl;
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded)
    {
        std::cout << "WaveformDisplay::loadURL file loaded" << std::endl;
        fileName = audioURL.getFileName();
        repaint();
    }
    else
    {
        std::cout << "WaveformDisplay::loadURL file NOT loaded" << std::endl;
    }
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}
