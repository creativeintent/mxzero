/*
  ==============================================================================

    RestrictionProcessor.cpp
    Created: 15 Feb 2017 10:03:51pm
    Author:  Nick Thompson

  ==============================================================================
*/

#include "RestrictionProcessor.h"

const double kFreq = 0.05; // Twenty second period.
const double kDuty = 0.75; // Fifteen seconds of audio, five seconds of silence.
const String kValidHash = "19fa61d75522a4669b44e39c1d2e1726c530232130d407f89afee0964997f7a73e83be698b288febcf88e3e03c4f0757ea8964e59b63d93708b138cc42a66eb3";

//==============================================================================
RestrictionProcessor::RestrictionProcessor()
{
    m_smoothing = new LinearSmoothedValue<float>();
    m_alpha = 1.0f;
}

//==============================================================================
void RestrictionProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    m_sampleRate = sampleRate;
    m_delta = kFreq / m_sampleRate;
    m_currentAngle = 0.0;
    m_smoothing->reset(sampleRate, 1.0);
}

void RestrictionProcessor::releaseResources() {}

void RestrictionProcessor::processBlock(AudioSampleBuffer &buffer)
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    float** channelData = buffer.getArrayOfWritePointers();

    for (int j = 0; j < numSamples; ++j)
    {
        const float targetGain = static_cast<float>(m_currentAngle <= kDuty);

        m_smoothing->setValue(targetGain);
        const float gain = m_smoothing->getNextValue();

        for (int i = 0; i < numChannels; ++i)
        {
            const float in = channelData[i][j];
            channelData[i][j] = m_alpha * gain * in + (1.0f - m_alpha) * in;
        }

        m_currentAngle = fmod(m_currentAngle + m_delta, 1.0);
    }
}

//==============================================================================
File RestrictionProcessor::getKeyFile()
{
    File reg = File::getSpecialLocation(File::userApplicationDataDirectory)
        .getChildFile("Application Support")
        .getChildFile("Creative Intent")
        .getChildFile("mxzero.key");

    if (!reg.existsAsFile())
        reg.create();

    DBG("Accessing key file:");
    DBG(reg.getFullPathName());
    return reg;
}

void RestrictionProcessor::validateRegistration()
{
    File reg = getKeyFile();
    Whirlpool hash(reg);
    String hex = hash.toHexString();

    // TODO: Check this against a known list of hashes bundled into the plugin.
    if (hex == kValidHash)
    {
        DBG("Validation successful! Unlocking...");
        m_alpha = 0.0f;
    }
}