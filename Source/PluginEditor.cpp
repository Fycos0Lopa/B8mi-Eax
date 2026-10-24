#include "PluginEditor.h"
B8miEaxAudioProcessorEditor::B8miEaxAudioProcessorEditor(B8miEaxAudioProcessor& p) : AudioProcessorEditor(&p), processor(p)
{
    setSize(680, 390);
    title.setText("B8mi Eax", juce::dontSendNotification);
    title.setFont(juce::Font(juce::FontOptions(27.0f, juce::Font::bold)));
    title.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(title);
    for (int i=0;i<9;++i) {
        labels[i].setText(names[i],juce::dontSendNotification);
        labels[i].setJustificationType(juce::Justification::centred);
        addAndMakeVisible(labels[i]);
        sliders[i].setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow,false,80,20);
        addAndMakeVisible(sliders[i]);
        attachments[i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,ids[i],sliders[i]);
    }
}
void B8miEaxAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff15171c));
    g.setColour(juce::Colour(0xff242832)); g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(8),12.0f);
    g.setColour(juce::Colour(0xffc8cbd2)); g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(8),12.0f,1.0f);
    g.setColour(juce::Colour(0xfff1f2f4)); g.setFont(14.0f);
    g.drawText("Microphone effects  •  EQ / Echo / Reverb", 24, 54, 620, 24, juce::Justification::centredLeft);
}
void B8miEaxAudioProcessorEditor::resized()
{
    title.setBounds(24,16,620,36);
    const int w= (getWidth()-48)/5;
    for(int i=0;i<9;++i) {
        const int row=i/5, col=i%5;
        const int x=24+col*w, y=92+row*142;
        labels[i].setBounds(x,y,w,22);
        sliders[i].setBounds(x+4,y+20,w-8,112);
    }
}
