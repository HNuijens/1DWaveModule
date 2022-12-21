/*
  ==============================================================================

    1DWaveModule.cpp
    Created: 14 Dec 2022 1:24:42pm
    Author:  Helmer Nuijens

  ==============================================================================
*/

#include "daisy_seed.h"
#include "daisysp.h"

#include <memory>

#include "Global.h"
#include "DynamicStiffString.h"


using namespace daisy;
using namespace daisysp;

DaisySeed hw;
std::unique_ptr<DynamicStiffString> dynamicStiffString;

float freq = 300; 
float freqChange = 0.05; 
float minFreq = 20;
float maxFreq = 600;
int t = 0;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	//stiffString.exciteSystem(0.8, 0.3, 10, false);

	freq+= freqChange; 
	if(freq > maxFreq || freq < minFreq)
	{
		freqChange*=-1;
		//dynamicString.exciteSystem(0.8f, 1. / (rand() % 10 + 1), 10);
		dynamicStiffString->excite(1.0, -1, 1. / (rand() % 10 + 1), 10);

	}

	//dynamicString.setDynamicGrid(freq);
	dynamicStiffString->refreshParameter(3, freq);

	for (size_t i = 0; i < size; i++)
	{
		dynamicStiffString->process();

		out[0][i] = limit(-1., 1., dynamicStiffString->getOutput());
		out[1][i] = limit(-1., 1., dynamicStiffString->getOutput());


	}
}

int main(void)
{	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);


	dynamicStiffString = std::make_unique<DynamicStiffString> (defaultDynamicStiffStringParameters, hw.AudioSampleRate());

	dynamicStiffString -> excite();

	hw.StartAudio(AudioCallback);

	while(1) {

	}
}


