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

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

StiffString stiffString;

// Resonator parameters:
map<string, float> parameters;

float f0 = 120; 		   // Fundamental Frequency
float L = 1.;              // length of string;

float sig0 = 0.1;           // frequency independent damping
float sig1 = 0.005;        // frequency dependent damping

float r = 0.00005;          // radius
float rho = 11000;          // density
float E = 2e11;            // Youngs modulus

float gain = 1.;


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	//stiffString.exciteSystem(0.8, 0.3, 10, false);

	for (size_t i = 0; i < size; i++)
	{
		float x = 0.0;

		x = stiffString.getNextSample(1.0 / M_PI);

		out[0][i] = x;
		out[1][i] = x;
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	
	// Resonator setup
	parameters["f0"] = f0;
	parameters["L"] =  L;
    parameters["E"] = E;
	parameters["rho"] = rho;
	parameters["r"] = r;
	parameters["sig0"] = sig0;
	parameters["sig1"] = sig1;

	stiffString.setFs(hw.AudioSampleRate());
	stiffString.setGrid(parameters);
	stiffString.exciteSystem(0.8f, 1. / (rand() % 10 + 1), 10, false);

	hw.StartAudio(AudioCallback);

	while(1) {}
}
