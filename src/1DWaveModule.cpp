/*
  ==============================================================================

    1DWaveModule.cpp
    Created: 14 Dec 2022 1:24:42pm
    Author:  Helmer Nuijens

  ==============================================================================
*/

#include "daisy_seed.h"
#include "daisysp.h"

#include "Global.h"
#include "StiffString.h"
#include "DynamicString.h"


using namespace daisy;
using namespace daisysp;

DaisySeed hw;

StiffString stiffString;
DynamicString dynamicString;

float freq = 220; 
float freqChange = 0.01; 
float minFreq = 110;
float maxFreq = 880;
int t = 0;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	//stiffString.exciteSystem(0.8, 0.3, 10, false);

	freq+= freqChange; 
	if(freq > maxFreq || freq < minFreq)
	{
		freqChange*=-1;
		dynamicString.exciteSystem(0.8f, 1. / (rand() % 10 + 1), 10);
	}

	dynamicString.setDynamicGrid(freq);
	

	for (size_t i = 0; i < size; i++)
	{
		dynamicString.process();

		out[0][i] = limit(-1., 1., dynamicString.getOutput(0.2f));
		out[1][i] = limit(-1., 1., dynamicString.getOutput(0.8f));
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	
	// stiffString.setFs(hw.AudioSampleRate());
	// stiffString.setGrid(defaultBarParameters);
	// stiffString.exciteSystem(0.8f, 1. / (rand() % 10 + 1), 10, false);


	dynamicString.setFs(hw.AudioSampleRate());
	dynamicString.setGrid(defaultStiffStringParameters);
	dynamicString.exciteSystem(0.8f, 1. / (rand() % 10 + 1), 10);

	hw.StartAudio(AudioCallback);

	while(1) {

	}
}


