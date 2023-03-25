/*
  ==============================================================================

    Song.h

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
using namespace juce;

// This class offers access to the files in order to use them (all the data about songs is stored)

class Song
{
public:
    Song(File _file);

    // It is a handle to the file itself, so it can be read
    File file;

    // This is the name and length of song
    String name;
    String length;

    // It creates a juce URL out of the file name
    URL URL;

    // A function for equality check: objects are being compared by their name
    // If a comparison of the songs will ever happen, it will be true if the name is the same
    bool operator==(const String& other) const;
};
