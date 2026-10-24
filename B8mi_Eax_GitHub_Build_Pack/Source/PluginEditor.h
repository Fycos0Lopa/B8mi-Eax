#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
class B8miEaxAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit B8miEaxAudioProcessorEditor(B8miEaxAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    B8miEaxAudioProcessor& processor;
    juce::Label title;
    juce::Slider sliders[9];
    juce::Label labels[9];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachments[9];
    const char* ids[9] = {"input","low","high","echoMix","echoMs","feedback","reverbMix","room","damping"};
    const char* names[9] = {"Input","Low EQ","High EQ","Echo Mix","Echo Time","Feedback","Reverb Mix","Room Size","Damping"};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(B8miEaxAudioProcessorEditor)
};
