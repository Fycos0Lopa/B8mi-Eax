#include "PluginProcessor.h"
#include "PluginEditor.h"

B8miEaxAudioProcessor::B8miEaxAudioProcessor()
 : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                  .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
   parameters(*this, nullptr, "PARAMETERS", createParameters())
{}
juce::AudioProcessorValueTreeState::ParameterLayout B8miEaxAudioProcessor::createParameters()
{
    using P = juce::AudioParameterFloat;
    juce::AudioProcessorValueTreeState::ParameterLayout p;
    p.add(std::make_unique<P>("input", "Input", juce::NormalisableRange<float>(0.0f, 2.0f, 0.001f), 1.0f));
    p.add(std::make_unique<P>("low", "Low", juce::NormalisableRange<float>(-18.0f, 18.0f, 0.1f), 0.0f));
    p.add(std::make_unique<P>("high", "High", juce::NormalisableRange<float>(-18.0f, 18.0f, 0.1f), 0.0f));
    p.add(std::make_unique<P>("echoMix", "Echo Mix", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.0f));
    p.add(std::make_unique<P>("echoMs", "Echo Time (ms)", juce::NormalisableRange<float>(1.0f, 1500.0f, 1.0f), 260.0f));
    p.add(std::make_unique<P>("feedback", "Echo Feedback", juce::NormalisableRange<float>(0.0f, 0.90f, 0.001f), 0.25f));
    p.add(std::make_unique<P>("reverbMix", "Reverb Mix", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.0f));
    p.add(std::make_unique<P>("room", "Room Size", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.35f));
    p.add(std::make_unique<P>("damping", "Damping", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.45f));
    return p;
}
void B8miEaxAudioProcessor::prepareToPlay(double sampleRate, int)
{
    sr = sampleRate;
    delay.reset();
    delay.setMaximumDelayInSamples(juce::jmax(1, (int)(sr * 1.5)));
    delay.prepare({ sampleRate, 512, (juce::uint32)getTotalNumOutputChannels() });
    reverb.reset();
    juce::dsp::ProcessSpec spec { sampleRate, 512, (juce::uint32)getTotalNumOutputChannels() };
    for (int ch = 0; ch < 2; ++ch) { lowFilters[ch].prepare(spec); highFilters[ch].prepare(spec); }
}
void B8miEaxAudioProcessor::releaseResources() { delay.reset(); reverb.reset(); }
bool B8miEaxAudioProcessor::isBusesLayoutSupported(const BusesLayout& l) const
{
    const auto in = l.getMainInputChannelSet(), out = l.getMainOutputChannelSet();
    return in == out && (out == juce::AudioChannelSet::mono() || out == juce::AudioChannelSet::stereo());
}
void B8miEaxAudioProcessor::processBlock(juce::AudioBuffer<float>& b, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int channels = getTotalNumOutputChannels();
    for (int ch=getTotalNumInputChannels(); ch<channels; ++ch) b.clear(ch,0,b.getNumSamples());
    const float input = *parameters.getRawParameterValue("input");
    const float low = *parameters.getRawParameterValue("low");
    const float high = *parameters.getRawParameterValue("high");
    const float echoMix = *parameters.getRawParameterValue("echoMix");
    const float echoMs = *parameters.getRawParameterValue("echoMs");
    const float feedback = *parameters.getRawParameterValue("feedback");
    const float revMix = *parameters.getRawParameterValue("reverbMix");
    const float room = *parameters.getRawParameterValue("room");
    const float damping = *parameters.getRawParameterValue("damping");
    auto lowC = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sr, 180.0, 0.707, juce::Decibels::decibelsToGain(low));
    auto highC = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sr, 4500.0, 0.707, juce::Decibels::decibelsToGain(high));
    for (int ch = 0; ch < 2; ++ch) { lowFilters[ch].coefficients = lowC; highFilters[ch].coefficients = highC; }
    juce::dsp::Reverb::Parameters rp; rp.roomSize=room; rp.damping=damping; rp.wetLevel=revMix; rp.dryLevel=1.0f-revMix; rp.width=1.0f; rp.freezeMode=0.0f;
    reverb.setParameters(rp);
    const int dSamples = juce::jlimit(1, (int)(sr*1.5), (int)(sr*echoMs/1000.0f));
    for (int i=0;i<b.getNumSamples();++i)
    {
        for (int ch=0;ch<channels;++ch)
        {
            float x=b.getSample(ch,i)*input;
            x=lowFilters[ch].processSample(x);
            x=highFilters[ch].processSample(x);
            const float delayed=delay.popSample(ch,dSamples);
            delay.pushSample(ch, x + delayed*feedback);
            x = x*(1.0f-echoMix) + delayed*echoMix;
            b.setSample(ch,i,x);
        }
    }
    if (channels == 2) reverb.processStereo(b.getWritePointer(0), b.getWritePointer(1), b.getNumSamples());
    else if (channels == 1) reverb.processMono(b.getWritePointer(0), b.getNumSamples());
}
juce::AudioProcessorEditor* B8miEaxAudioProcessor::createEditor(){ return new B8miEaxAudioProcessorEditor(*this); }
void B8miEaxAudioProcessor::getStateInformation(juce::MemoryBlock& dest){ auto s=parameters.copyState(); std::unique_ptr<juce::XmlElement> xml(s.createXml()); copyXmlToBinary(*xml,dest); }
void B8miEaxAudioProcessor::setStateInformation(const void* data,int size){ std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data,size)); if(xml && xml->hasTagName(parameters.state.getType())) parameters.replaceState(juce::ValueTree::fromXml(*xml)); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){ return new B8miEaxAudioProcessor(); }
