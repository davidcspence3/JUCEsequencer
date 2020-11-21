#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : playButton("playButton", juce::Colour(20, 31, 51), juce::Colour(20, 31, 51), juce::Colour(20, 31, 51)),
                                    stopButton("stopButton", juce::Colour(20, 31, 51), juce::Colour(20, 31, 51), juce::Colour(20, 31, 51)),
                                    pauseButton("pauseButton", juce::Colour(20, 31, 51), juce::Colour(20, 31, 51), juce::Colour(20, 31, 51))
{

    setSize (900, 570);//310
    timeCount = 0;
    beatCount = 0;
    bpm = 200;
    bpmSlider.setSliderStyle(juce::Slider::LinearBar);
    bpmSlider.setRange(0, 400, 1);
    bpmSlider.setValue(100);
    bpmSlider.addListener(this);
    addAndMakeVisible(bpmSlider);

    juce::Rectangle<float> buttonArea(40., 60.);

    juce::Path playPath;
    playPath.addTriangle(0, 0, 0, buttonArea.getHeight(), buttonArea.getWidth() * sqrt(3) / 2, buttonArea.getHeight() / 2);
    playButton.setShape(playPath, false, true, true);
    playButton.setOnColours(juce::Colours::forestgreen, juce::Colours::forestgreen, juce::Colours::forestgreen);
    playButton.shouldUseOnColours(true);
    playButton.setClickingTogglesState(true);
    playButton.addListener(this);
    addAndMakeVisible(playButton);

    juce::Path pausePath;
    pausePath.addRectangle(0., 0., 13., buttonArea.getHeight());
    pausePath.addRectangle(20., 0., 13., buttonArea.getHeight());
    pauseButton.setShape(pausePath, false, true, true);
    pauseButton.setOnColours(juce::Colour(8, 12, 20), juce::Colour(8, 12, 20), juce::Colour(8, 12, 20));
    pauseButton.shouldUseOnColours(true);
    pauseButton.setClickingTogglesState(true);
    pauseButton.addListener(this);
    addAndMakeVisible(pauseButton);

    juce::Path stopPath;
    stopPath.addRectangle(0., 0., buttonArea.getWidth(), buttonArea.getHeight());
    stopButton.setShape(stopPath, false, true, true);
    stopButton.setOnColours(juce::Colour(8, 12, 20), juce::Colour(8, 12, 20), juce::Colour(8, 12, 20));
    stopButton.shouldUseOnColours(true);
    stopButton.setClickingTogglesState(true);
    stopButton.addListener(this);
    addAndMakeVisible(stopButton);


    addAndMakeVisible(deviceList);
    auto midiDeviceList = MidiOutput::getAvailableDevices();
    juce::StringArray midiOutNames;
    for (int i = 0; i < midiDeviceList.size(); ++i) {
        midiOutNames.add(midiDeviceList[i].name);
    }
    deviceList.addItemList(midiOutNames, 1);
    deviceList.onChange = [this] { setMidiOutput(deviceList.getItemText(deviceList.getSelectedItemIndex())); };
    deviceList.setSelectedId(1, dontSendNotification);
    deviceList.setLookAndFeel(&customLookAndFeel2);
    deviceList.addListener(this);
    midiDeviceList = MidiOutput::getAvailableDevices();
    for (int i = 0; i < midiDeviceList.size(); ++i)
    {
        if (midiDeviceList[i].name == deviceList.getItemText(deviceList.getSelectedItemIndex()))
        {
            midiOutput = MidiOutput::openDevice(midiDeviceList[i].identifier);
        }
    }

    addAndMakeVisible(presetList);
    juce::StringArray presetNames;
    presetNames.add("Clear");
    presetNames.add("Rhythm 1");
    presetNames.add("Rhythm 2");
    presetNames.add("Piano 1");
    presetList.addItemList(presetNames, 1);
    presetList.onChange = [this] { setMidiOutput(presetList.getItemText(presetList.getSelectedItemIndex())); };
    presetList.setSelectedId(1, dontSendNotification);
    presetList.addListener(this);


    
    //Creates toggle arrays, not GUI
    for (int i = 0; i < 32; ++i) {

        for (int j = 0; j < 8; ++j) {

            pad[i][j].juce::Button::setToggleState(false, juce::dontSendNotification);
            pad[i][j].addListener(this);
            addAndMakeVisible(pad[i][j]);
            pad2[i][j].juce::Button::setToggleState(false, juce::dontSendNotification);
            pad2[i][j].addListener(this);
            addAndMakeVisible(pad2[i][j]);
            sustain[i][j] = false;
        }
    }
}

void MainComponent::setMidiOutput(const juce::String& identifier)
{
    // Find the passed-in MIDI output in the available MIDI outputs list and set the midiOutPtr_ accordingly
    auto midiOutputs = MidiOutput::getAvailableDevices();
    for (auto i = 0; i < midiOutputs.size(); ++i)
    {
        if (midiOutputs[i].name == identifier)
        {
            midiOutput = MidiOutput::openDevice(midiOutputs[i].identifier);
            //setUnsavedChangesFlag(true);
        }
    }
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)///////////////////////////////////////////
    g.fillAll(juce::Colour(8, 12, 20));

    //Drum Grid Box
    g.setColour(juce::Colour(42, 215, 238));
    juce::Rectangle<float> toggleRect(10, 90, 880, 230);
    g.fillRoundedRectangle(toggleRect, 5.0f);

    //Note Grid Box
    juce::Rectangle<float> toggleRect1(10, 330, 880, 230);
    g.fillRoundedRectangle(toggleRect1, 5.0f);
    
    //First
    g.fillRoundedRectangle(10.0f, 10.0f, 300.0f, 70.0f, 5.0f);
    //Second
    g.fillRoundedRectangle(320.0f, 10.0f, 210.0f, 70.0f, 5.0f);
    //Third
    g.fillRoundedRectangle(540.0f, 10.0f, 350.0f, 70.0f, 5.0f);

    g.setColour(juce::Colour(12, 20, 31));
    g.setFont(juce::Font(14, 1));
    g.drawText("Select MIDI device:", 50, 10, 200, 30.0f, juce::Justification::centredLeft);
    g.drawText("Select Preset Loop:", 600, 10, 200, 30.0f, juce::Justification::centredLeft);
    g.drawText("B P M", 740, 10, 100, 30.0f, juce::Justification::horizontallyCentred);
    g.drawText("Drum Section", 10, 88, 200, 30.0f, juce::Justification::horizontallyCentred);
    g.drawText("Piano Section", 10, 330, 200, 30.0f, juce::Justification::horizontallyCentred);

    g.drawText("Kick2", 20, 113, 200, 30.0f, juce::Justification::left);
    g.drawText("Kick1", 20, 138, 200, 30.0f, juce::Justification::left);
    g.drawText("Stick", 20, 163, 200, 30.0f, juce::Justification::left);
    g.drawText("Snare", 20, 188, 200, 30.0f, juce::Justification::left);
    g.drawText("Clap", 20, 213, 200, 30.0f, juce::Justification::left);
    g.drawText("Bongo", 20, 238, 200, 30.0f, juce::Justification::left);
    g.drawText("Tom", 20, 263, 200, 30.0f, juce::Justification::left);
    g.drawText("HiHat", 20, 288, 200, 30.0f, juce::Justification::left);


}

void MainComponent::resized()
{
    juce::Rectangle<int> area = getLocalBounds().reduced(10);
    bpmSlider.setBounds(740, 40, 100, 20); //change bpm slider position

    int xx = 340;
    playButton.setBounds(xx, 25, 40, 40);
    pauseButton.setBounds(xx + 60, 25, 40, 40);
    stopButton.setBounds(xx + 130, 25, 40, 40);

    deviceList.setBounds(50, 40, 100, 20);
    presetList.setBounds(600, 40, 100, 20);

    int x= 65;//Left Side Drum Grid


    for (int i = 0; i < 32; ++i) {
        int y = 115;//Top of Drum Grid
        for (int j = 0; j < 8; ++j) {
            pad[i][j].setBounds(x, y, 22, 22);
            y += 25;
        }
        x += 25;
    }

    int u = 65;// Left Side Note Grid

    for (int i = 0; i < 32; ++i) {
        int y = 355;//Top of Note Grid
        for (int j = 0; j < 8; ++j) {
            pad2[i][j].setBounds(u, y, 22, 22);
            y += 25;
        }
        u += 25;
    }


}

void MainComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged = &presetList) {

        switch (presetList.getSelectedId())
        {
        case 1:
            for (int i = 0; i < 32; ++i) {

                for (int j = 0; j < 8; ++j) {

                    pad[i][j].juce::Button::setToggleState(false, juce::dontSendNotification);
                    pad2[i][j].juce::Button::setToggleState(false, juce::dontSendNotification);
                    sustain[i][j] = false;
                }
            }
            break;

        //Rhythm 1
        case 2:

            //Reset all of Drum grid
            for (int i = 0; i < 32; ++i) {

                for (int j = 0; j < 8; ++j) {

                    pad[i][j].juce::Button::setToggleState(false, juce::dontSendNotification);
                }
            }

            //HiHAT
            pad[0][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[2][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[4][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[6][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[8][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[10][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[12][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[14][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[16][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[18][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[20][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[22][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[24][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[26][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[28][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[30][7].juce::Button::setToggleState(true, juce::dontSendNotification);

            //KICK
            pad[0][1].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[4][1].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[16][1].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[20][1].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[22][1].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[28][1].juce::Button::setToggleState(true, juce::dontSendNotification);


            //SNARE
            pad[8][3].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[24][3].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[30][3].juce::Button::setToggleState(true, juce::dontSendNotification);

            //CLAP
            pad[2][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[6][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[10][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[14][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[18][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[22][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[26][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[30][4].juce::Button::setToggleState(true, juce::dontSendNotification);


            break;
            
        //Rhythm 2
        case 3:

            //Reset all of Drum grid
            for (int i = 0; i < 32; ++i) {

                for (int j = 0; j < 8; ++j) {

                    pad[i][j].juce::Button::setToggleState(false, juce::dontSendNotification);
                }
            }
             
            //HiHAT
            pad[0][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[1][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[2][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[3][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[4][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[5][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[6][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[7][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[8][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[9][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[10][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[11][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[12][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[13][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[14][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[15][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[16][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[17][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[18][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[19][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[20][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[21][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[22][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[23][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[24][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[25][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[26][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[27][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[28][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[29][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[30][7].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[31][7].juce::Button::setToggleState(true, juce::dontSendNotification);

            //KICK
            pad[0][1].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[4][1].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[8][1].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[12][1].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[16][1].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[20][1].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[24][1].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[28][1].juce::Button::setToggleState(true, juce::dontSendNotification);


            //SNARE
            pad[4][3].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[12][3].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[20][3].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad[28][3].juce::Button::setToggleState(true, juce::dontSendNotification);
            break;


            break;

        case 4:

            //Reset all of Note grid
            for (int i = 0; i < 32; ++i) {

                for (int j = 0; j < 8; ++j) {

                    pad2[i][j].juce::Button::setToggleState(false, juce::dontSendNotification);
                }
            }
            pad2[0][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[1][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[2][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[3][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[4][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[5][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[6][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[8][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[9][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[10][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[11][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[12][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[13][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[14][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[20][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[22][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[23][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[24][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[25][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[26][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[27][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[28][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[29][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[30][0].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[31][0].juce::Button::setToggleState(true, juce::dontSendNotification);

            pad2[0][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[1][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[2][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[3][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[4][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[5][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[6][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[8][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[9][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[10][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[11][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[12][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[13][4].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[16][4].juce::Button::setToggleState(true, juce::dontSendNotification);

            pad2[18][2].juce::Button::setToggleState(true, juce::dontSendNotification);

            pad2[22][5].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[23][5].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[24][5].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[25][5].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[26][2].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[27][2].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[28][2].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[29][2].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[30][2].juce::Button::setToggleState(true, juce::dontSendNotification);
            pad2[31][2].juce::Button::setToggleState(true, juce::dontSendNotification);

            break;
        }
    }
    



    if (comboBoxThatHasChanged = &deviceList) {

        auto list = juce::MidiOutput::getAvailableDevices();

        for (auto i = 0; i < list.size(); ++i)
        {
                midiOutput = MidiOutput::openDevice(list[i].identifier);
        }


        if (midiOutput == nullptr && MidiOutput::getDevices().size() > 0)
        {
            midiOutput = MidiOutput::openDevice(deviceList.getSelectedId());
        }
        }
    }

    
    void MainComponent::sliderValueChanged(juce::Slider * slider){
        if (slider == &bpmSlider) {

            bpm = bpmSlider.getValue();
        }
    }
    
    void MainComponent::buttonClicked(juce::Button * button)
    {
        if (playButton.getToggleState() == true) {

            Timer::startTimer(60000 / (bpm * 4));
            playButton.setRadioGroupId(1, juce::NotificationType::dontSendNotification);
            midiEventList.begin();
        }

        if (pauseButton.getToggleState() == true) {

            Timer::stopTimer();
            pauseButton.setRadioGroupId(1, juce::NotificationType::dontSendNotification);
            midiOutput->clearAllPendingMessages();

        }

        if (stopButton.getToggleState() == true) {

            Timer::stopTimer();
            stopButton.setRadioGroupId(1, juce::NotificationType::dontSendNotification);

            for (int i = 0; i < 32; ++i) {
                for (int j = 0; j < 8; ++j) {
                    pad[i][j].resetStep();
                    pad2[i][j].resetStep();
                }
            }
            midiOutput->clearAllPendingMessages();
            timeCount = 0;
            beatCount = 0;
        }
    }

    void MainComponent::timerCallback()
    {
        if (beatCount > 31) {
            beatCount = 0;

            midiEventList.clear();
        }


        for (track = 0; track < 8; ++track) {

            pad[beatCount][track].lightUpStep();
            pad2[beatCount][track].lightUpStep();

            if (pad[beatCount][track].getToggleState() == true) {

                midiMessage = MidiMessage::noteOn(10, track + 35, 0.5f);

            }

            midiOutput->sendMessageNow(midiMessage);

        

            if (pad2[beatCount][track].getToggleState() == true) {
                
                if (sustain[beatCount][track] == false) {
                    midiMessage = MidiMessage::noteOn(1, track + 60, 0.5f);
                }
                
                if (beatCount < 31) {
                    sustain[beatCount + 1][track] = true;
                }
            }

            else if (pad2[beatCount][track].getToggleState() == false) {

                if (sustain[beatCount][track] == false) {
                    midiMessage = MidiMessage::noteOff(1, track + 60, 0.5f);
                }
                if (beatCount < 31) {
                    sustain[beatCount + 1][track] = false;
                }
            }

            midiOutput->sendMessageNow(midiMessage);
        }

        //GUI Grid
        if (beatCount == 0) {

            for (int i = 0; i < 8; ++i) {
                pad[31][i].resetStep();
                pad2[31][i].resetStep();
            }
        }

        else {

            for (int i = 0; i < 8; ++i) {
                pad[beatCount - 1][i].resetStep();
                pad2[beatCount - 1][i].resetStep();

            }
        }

        ++beatCount;
        ++timeCount;
    }

    
