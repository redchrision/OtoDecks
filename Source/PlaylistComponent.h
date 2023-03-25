/*
  ==============================================================================

    PlaylistComponent.h

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include "Song.h"
#include <fstream>
using namespace juce;

//==============================================================================
/*
*/
class PlaylistComponent  : public Component,
                           public TableListBoxModel,
                           public Button::Listener,
                           public TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1, 
                      DeckGUI* _deckGUI2, 
                      DJAudioPlayer* _playerForParsingMetaData
                     );
    ~PlaylistComponent() override;

    void paint (Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(Graphics& g, 
                            int rowNumber, 
                            int width,
                            int height,
                            bool rowIsSelected
                           ) override;
    void paintCell(Graphics& g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected
                  ) override;
    
    Component* refreshComponentForCell(int rowNumber, 
                                       int columnId, 
                                       bool isRowSelected, 
                                       Component* existingComponentToUpdate) override;
    void buttonClicked(Button* button) override;

private:
    std::vector<Song> songs;
    
    TableListBox musicLibrary;
    TextEditor searchTrackInLibrary;
    TextButton playInDeck1Button{ "PLAY IN DECK 1" };
    TextButton playInDeck2Button{ "PLAY IN DECK 2" };
    TextButton loadFileInLibraryButton{ "LOAD TRACK IN LIBRARY" };

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    // Parses and displays the lenght meta data of songs
    String getLength(URL audioURL);
    DJAudioPlayer* playerForParsingMetaData;

    // Finds seconds and minutes. Converts and formats them into a string.
    String secondsToMinutes(double seconds);

    // Loads the music library
    void loadMusicLibrary();

    // Function that allows the user to add files to their library
    void loadToMusicLibrary();

    // Loads the file selected in the player
    void loadInPlayer(DeckGUI* deckGUI);

    //Searches for songs which name contains searchText by calling whereInSongs and selects it
    void searchMusicLibrary(String searchText);

    // Searches for a track whose name contains searchText
    int whereInSongs(String searchText);

    // Tests if one of the songs has a certain file name
    bool foundInSongs(String fileNameWithoutExtension);

    // Saves the music library to a csv file
    void saveMusicLibrary();

    // Deletes song
    void deleteSong(int id);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
