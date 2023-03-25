/*
  ==============================================================================

    DJAudioPlayer.h

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
using namespace juce;

class DJAudioPlayer : public AudioSource
{
    public:
        DJAudioPlayer(AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        // Loads the audio file
        void loadURL(URL audioURL);

        // Plays the audio file
        void play();

        // Stops the audio file
        void stop();

        // Sets the relative position of the audio file
        void setPositionRelative(double pos);

        // Sets the volume
        void setGain(double gain);
        
        // Sets the speed
        void setSpeed(double ratio);

        // Gets the relative position of playhead
        double getPositionRelative();

        // Gets the length of transport source in seconds
        double getLengthInSeconds();

    private:
        void setPosition(double posInSecs);
        AudioFormatManager& formatManager;
        std::unique_ptr<AudioFormatReaderSource> readerSource;
        AudioTransportSource transportSource;
        ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};