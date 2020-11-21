#pragma once

#include "JuceHeader.h"
#include "TogglePad.h"
//#include "NoteControls.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component,
                       public juce::Slider::Listener,
                       public ComboBox::Listener,
                       public juce::Button::Listener,
                       private juce::Timer

{
    Viewport view;

public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged(juce::Slider * slider) override;
    void buttonClicked(juce::Button * button) override;
    void timerCallback() override;

    void MainComponent::setMidiOutput(const juce::String& identifier);

private:


    CustomLookAndFeel customLookAndFeel;
    CustomLookAndFeel2 customLookAndFeel2;

    TogglePad pad[32][8];
    TogglePad pad2[32][8];

    bool sustain[32][8];

    //==============================================================================
    juce::ShapeButton pauseButton, stopButton, playButton;
    juce::Slider bpmSlider;
    std::unique_ptr<MidiOutput> midiOutput;
    MidiMessage midiMessage;
    juce::MidiMessageSequence midiEventList;
    MidiFile midiFile;
    ComboBox deviceList;
    ComboBox presetList;

    unsigned int bpm;
    unsigned long int timeCount;
    int beatCount, track;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
