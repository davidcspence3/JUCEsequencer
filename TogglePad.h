/*
  ==============================================================================

    TogglePad.h
    Created: 11 Oct 2020 12:55:44am
    Author:  David

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class TogglePad : public juce::ToggleButton

{
public:

	void paint(juce::Graphics& g)
	{
		juce::Colour fillColour = (getToggleState() == true ? juce::Colour(200, 0, 15) : juce::Colour(0, 31, 200));//(223, 116, 12)
		g.fillAll(fillColour);

		if (getState() == highlight) {

			g.setOpacity(0.8);
			g.fillAll();
		}


	}

	void lightUpStep()
	{
		setState(highlight);

	}

	void resetStep()
	{
		setState(ButtonState::buttonNormal);
	}

private:
	ButtonState highlight;

};
