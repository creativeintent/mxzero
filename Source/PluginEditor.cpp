/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
MxzeroAudioProcessorEditor::MxzeroAudioProcessorEditor (MxzeroAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), m_vts(vts)
{
    addAndMakeVisible(m_main = new MainComponent(m_vts));
    addAndMakeVisible(m_vizPre = new SpectroscopeComponent());
    addAndMakeVisible(m_vizPost = new SpectroscopeComponent());

    m_main->setAlwaysOnTop(true);
    m_vizPre->setColours(Colour::fromRGBA(255, 51, 34, 255),
                         Colour::fromRGBA(223, 19, 19, 255).withAlpha(0.7f),
                         Colour::fromRGBA(123, 0, 0, 255).withAlpha(0.7f));
    m_vizPost->setColours(Colour::fromRGBA(255, 186, 34, 255),
                          Colour::fromRGBA(253, 174, 25, 255).withAlpha(0.7f),
                          Colour::fromRGBA(255, 126, 0, 255).withAlpha(0.7f));

    m_vizPre->toBehind(m_vizPost);

    setSize (744, 476);
    setLookAndFeel(&laf);
}

MxzeroAudioProcessorEditor::~MxzeroAudioProcessorEditor()
{
}

//==============================================================================
void MxzeroAudioProcessorEditor::paint (Graphics& g)
{
    Image graphBackground = ImageCache::getFromMemory(BinaryData::GraphBackground_png,
                                                      BinaryData::GraphBackground_pngSize);

    g.drawImageAt(graphBackground.rescaled(396, 134), 194, 181);
}

void MxzeroAudioProcessorEditor::resized()
{
    m_main->setBounds(0, 0, 744, 476);
    m_vizPre->setBounds(194, 181, 396, 134);
    m_vizPost->setBounds(194, 181, 396, 134);
}
