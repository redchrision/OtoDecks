/*
  ==============================================================================

    CustomComponent.h

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
using namespace juce;

//==============================================================================
/*
*/
class CustomComponent  : public Component,
                         public LookAndFeel_V4
{
public:
    CustomComponent();
    ~CustomComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // Draws a component with a rotary slider and custom graphics

    void drawRotarySlider(Graphics& g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float rotaryStartAngle,
        float rotaryEndAngle,
        Slider& slider)
    {
        float diameter = jmin(width, height);
        float radius = diameter / 2;
        float centreX = x + width / 2;
        float centreY = y + height / 2;
        float rx = centreX - radius;
        float ry = centreY - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

        Rectangle<float> dialArea(rx, ry, diameter, diameter);

        g.setColour(Colours::black);
        g.fillEllipse(dialArea);
        g.setColour(Colours::grey);
        g.drawRect(dialArea);

        g.setColour(Colours::lightgrey);
        g.fillEllipse(centreX - 15, centreY - 15, 30, 30);

        g.setColour(Colours::black);
        g.fillEllipse(centreX - 7.5, centreY - 7.5, 15, 15);

        g.setColour(Colours::white);
        Path dialTick;
        dialTick.addRectangle(0, -radius, 5.0f, radius * 0.33);
        g.fillPath(dialTick, AffineTransform::rotation(angle).translated(centreX, centreY));
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomComponent)
};
