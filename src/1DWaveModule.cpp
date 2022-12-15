/*
  ==============================================================================

    1DWaveModule.cpp
    Created: 14 Dec 2022 1:24:42pm
    Author:  Helmer Nuijens

  ==============================================================================
*/

#include "daisy_seed.h"
#include "daisysp.h"
#include "StiffString.h"
#include "Global.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
StiffString stiffString;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	//stiffString.exciteSystem(0.8, 0.3, 10, false);

	for (size_t i = 0; i < size; i++)
	{
		stiffString.process();

		out[0][i] = limit(-1., 1., stiffString.getOutput(0.1f));
		out[1][i] = limit(-1., 1., stiffString.getOutput(0.9f));
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	
	stiffString.setFs(hw.AudioSampleRate());
	stiffString.setGrid(defaultBarParameters);
	stiffString.exciteSystem(0.8f, 1. / (rand() % 10 + 1), 10, false);

	hw.StartAudio(AudioCallback);

	while(1) {

	}
}


