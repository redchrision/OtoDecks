/*
  ==============================================================================

    PlaylistComponent.cpp

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1, 
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _playerForParsingMetaData
                                    ) : deckGUI1(_deckGUI1),
                                        deckGUI2(_deckGUI2),
                                        playerForParsingMetaData(_playerForParsingMetaData)
{   
    // makes visible library components (library, buttons and search field)
    addAndMakeVisible(musicLibrary);
    addAndMakeVisible(playInDeck1Button);
    addAndMakeVisible(playInDeck2Button);
    addAndMakeVisible(loadFileInLibraryButton);
    addAndMakeVisible(searchTrackInLibrary);

    // attaches listeners to buttons
    playInDeck1Button.addListener(this);
    playInDeck2Button.addListener(this);
    loadFileInLibraryButton.addListener(this);
    searchTrackInLibrary.addListener(this);
    
    // setup table and load library from file
    musicLibrary.getHeader().addColumn("Tracks", 1, 1);
    musicLibrary.getHeader().addColumn("Length", 2, 1);
    musicLibrary.getHeader().addColumn("", 3, 1);
    musicLibrary.setModel(this);
    loadMusicLibrary();

    // configures searchTrackInLibrary with text to display
    searchTrackInLibrary.setTextToShowWhenEmpty("SEARCH (Track + Enter)", Colours::white);
    searchTrackInLibrary.onReturnKey = [this] { searchMusicLibrary(searchTrackInLibrary.getText()); };
}

PlaylistComponent::~PlaylistComponent()
{
    saveMusicLibrary();
}

void PlaylistComponent::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(16.0f);
}

void PlaylistComponent::resized()
{
    //configures the postion of the music library

    double rowH = getHeight() / 8;
    musicLibrary.setBounds(0, 0, getWidth(), 7 * rowH);

    //configures the postion of buttons
    playInDeck1Button.setBounds(getWidth() / 4, 7 * rowH, getWidth() / 4, rowH);
    playInDeck2Button.setBounds(getWidth() / 2, 7 * rowH, getWidth() / 4, rowH);
    loadFileInLibraryButton.setBounds(getWidth() / 4 + getWidth() / 2, 7 * rowH, getWidth() / 4, rowH);

    //configures the postion of the search field
    searchTrackInLibrary.setBounds(0, 7 * rowH, getWidth() / 4, rowH);

    //configures the postion of the music library headers
    musicLibrary.getHeader().setColumnWidth(1, 12.9 * getWidth() / 20);
    musicLibrary.getHeader().setColumnWidth(2, 5 * getWidth() / 20);
    musicLibrary.getHeader().setColumnWidth(3, 2 * getWidth() / 20);
}

int PlaylistComponent::getNumRows()
{
    return songs.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected
                                          )
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else
    {
        g.fillAll(Colours::grey);
    }
}

void PlaylistComponent::paintCell(Graphics& g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected
                                 )
{
    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            g.drawText(songs[rowNumber].name,
                2,
                0,
                width - 4,
                height,
                Justification::centredLeft,
                true
            );
        }
        if (columnId == 2)
        {
            g.drawText(songs[rowNumber].length,
                2,
                0,
                width - 4,
                height,
                Justification::topLeft,
                true
            );
        }
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component* existingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            //adds the delete X TextButton
            TextButton* btn = new TextButton{ "X" };
            
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

// Button-related events

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &loadFileInLibraryButton)
    {
        musicLibrary.updateContent();
        loadToMusicLibrary();
    }
    else if (button == &playInDeck1Button)
    {
        loadInPlayer(deckGUI1);
    }
    else if (button == &playInDeck2Button)
    {
        loadInPlayer(deckGUI2);
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        deleteSong(id);
        musicLibrary.updateContent();
    }
}

/* Allows the user to add files to their library */

void PlaylistComponent::loadToMusicLibrary()
{
    //sets file chooser
    FileChooser chooser{ "Select files" };
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const File& file : chooser.getResults())
        {
            String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            if (!foundInSongs(fileNameWithoutExtension)) 
            {
                Song newSong{ file };
                URL audioURL{ file };
                newSong.length = getLength(audioURL) ;
                songs.push_back(newSong);
            }
            else 
            {
                //displays Info Box
                AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon,
                                                         "Info Box:",
                                                         fileNameWithoutExtension + 
                                                         " is already loaded in the library. Please select a different track to add.",
                                                         "OK",
                                                         nullptr
                                            );
            }
        }
    }
}

/* Parses and displays meta data such as song length */

String PlaylistComponent::getLength(URL audioURL)
{
    playerForParsingMetaData->loadURL(audioURL);
    double seconds{ playerForParsingMetaData->getLengthInSeconds() };
    String minutes{ secondsToMinutes(seconds) };
    return minutes;
}

// Finds seconds and minutes. Converts and formats them into a string. 
String PlaylistComponent::secondsToMinutes(double seconds)
{
    //Find seconds and minutes and converts them into strings
    int roundedSeconds{ int(std::round(seconds)) };
    String minute{ std::to_string(roundedSeconds / 60) };
    String second{ std::to_string(roundedSeconds % 60) };
    if (second.length() < 2)
    {
        second = second.paddedLeft('0', 2);
    }
    return String{ minute + ":" + second };
}

// Loads the file in the player or diplays Info box, if there is no song in the music library
void PlaylistComponent::loadInPlayer(DeckGUI* deckGUI)
{
    int selectedRow{ musicLibrary.getSelectedRow() };
    if (selectedRow != -1)
    {
        // Loads the file in player
        deckGUI->loadFile(songs[selectedRow].URL);
    }
    else
    {
        // Displays Info Box
        AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon,
                                                 "Info Box:",
                                                 "Please choose a track to add to one of the decks",
                                                 "OK",
                                                 nullptr
                                    );
    }
}

// Searches for songs which name contains searchText, by calling whereInSongs, and selects it
void PlaylistComponent::searchMusicLibrary(String searchText)
{
    if (searchText != "")
    {
        int rowId = whereInSongs(searchText);
        musicLibrary.selectRow(rowId);
    }
    else
    {
        musicLibrary.deselectAllRows();
    }
}

// Searches for a song whose name contains searchText
int PlaylistComponent::whereInSongs(String searchText)
{
    auto iterator = find_if(songs.begin(), 
                            songs.end(), 
                            [&searchText](const Song& obj)     
                            {return obj.name.contains(searchText); });
    int j = -1;

    if (iterator != songs.end())
    {
        j = std::distance(songs.begin(), iterator);
    }
    return j;
}

// Tests if one of the songs has a certain file name (true or false)
bool PlaylistComponent::foundInSongs(String fileNameWithoutExtension)
{
    return (std::find(songs.begin(), songs.end(), fileNameWithoutExtension) != songs.end());
}

// Saves the music library to a csv file
void PlaylistComponent::saveMusicLibrary()
{
    // ofstream will create a new csv file
    std::ofstream musicLibrary("music-library.csv");
    // for all songs in the list, it outputs them in the csv file
    for (Song& song : songs)
    {
        musicLibrary << song.file.getFullPathName() << "," << song.length << "\n";
    }
}

// Loads the music library in the application
void PlaylistComponent::loadMusicLibrary()
{
    // Opens the file
    std::ifstream musicLibrary("music-library.csv");
    // Creates two strings
    std::string filePath;
    std::string length;
    //
    if (musicLibrary.is_open())
    {
        //getline() is a standard library function used to read the filePath and length
        while (getline(musicLibrary, filePath, ',')) {
            File file{ filePath };
            Song newSong{ file };
            getline(musicLibrary, length);
            newSong.length = length;
            // Adding the information in the songs list
            songs.push_back(newSong);
        }
    }
    musicLibrary.close();
}

// Deletes a certain selected song
void PlaylistComponent::deleteSong(int id)
{
    songs.erase(songs.begin() + id);
}