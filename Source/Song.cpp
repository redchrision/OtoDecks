/*
  ==============================================================================
    Song.cpp
  ==============================================================================
*/

#include "Song.h"
#include <filesystem>

Song::Song(File _file) : name(_file.getFileNameWithoutExtension()),
file(_file),
URL(juce::URL{ _file })
{
}

// Function for equality check: objects are being compared by their name (true or false)
bool Song::operator==(const String& further) const
{
    return name == further;
}