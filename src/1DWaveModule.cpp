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
using namespace daisy::seed;

std::vector<Pin> parameterPins =  {A0, A1, A2, A3, A4, A5, A6}; 

enum AdcChannel {
   lengthKnob = 0,
   densityKnob,
   radiusKnob,
   tensionKnob,
   elasticityKnob,
   freqIndepKnob,
   freqDepKnob,
   NUM_ADC_CHANNELS
};

void excite(float eMag, float eLoc);
struct ExcitationHandler
{
	int bufferLength = 20; 
	std::vector<float> buffer; 
	int bufferIdx = 0; 
	
	float threshold = 0.1; 
	bool excitationFlag = false; 

	float getLoc(float x1, float x2)
	{
	return abs(x1 - 0.5) - abs(x2 - 0.5) + 0.5;
	}

	float getMagnitude(float x1, float x2)
	{
		return 2 * max(abs(x1 - 0.5), abs(x2 - 0.5));
	}

	void init(int bufferLength = 20, float threshold = 0.1)
	{
		buffer = std::vector<float>(bufferLength, 0.);
		this->threshold = threshold; 
	}

	void process(float val1, float val2)
	{
		// excitation detection:
		float magnitude = getMagnitude(val1, val2); 
		buffer[bufferIdx % bufferLength] = magnitude;
		if(bufferIdx > bufferLength) 
			bufferIdx = 0;

		if(!excitationFlag && (magnitude > threshold))
		{
			excitationFlag = true; 
		}

		//check if string is returned to start pos
		if(excitationFlag && (magnitude < threshold))
		{
			float eLoc = getLoc(val1, val2);
			float eMag = *max_element(buffer.begin(), buffer.end());
			excite(eMag, eLoc);
			excitationFlag = false; 
		}

		bufferIdx ++; 
	}
};

DaisySeed hw;
std::unique_ptr<DynamicStiffString> dynamicStiffString;
ExcitationHandler excitationHandler; 

float T = 300; 
int t = 0 ; 
float maxFreq = 600;

void configADC();


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	//stiffString.exciteSystem(0.8, 0.3, 10, false);

	if(t > 10000)
	{
		//1. / (rand() % 10 + 1)
		dynamicStiffString->excite(1.0, -1, 0.22, 10);
		t = 0; 
	}
	t++;

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
	
	configADC();

	dynamicStiffString = std::make_unique<DynamicStiffString> (defaultDynamicStiffStringParameters, hw.AudioSampleRate());

	dynamicStiffString -> excite();

	excitationHandler.init();
	hw.StartAudio(AudioCallback);

	while(1) {
		for(int i = 0 ; i < NUM_ADC_CHANNELS; i ++)
		{
			float input = hw.adc.GetFloat(i);
			float value = map(input, 0., 1., parameterLimits[i][0], parameterLimits[i][1]);
			dynamicStiffString->refreshParameter(i, value);
		}
		
		float val1 = 0.5, val2 = 0.5; // dummy values
		
		excitationHandler.process(val1, val2);
		
		System::Delay(5);
	}
}

void configADC()
{
	//const int num_adc_channels = static_cast<int>(parameterPins.size()); 
	AdcChannelConfig adc_config[NUM_ADC_CHANNELS];


	for(int i = 0; i < parameterPins.size(); i ++)
	{
		adc_config[i].InitSingle(parameterPins[i]);
	}

	hw.adc.Init(adc_config, NUM_ADC_CHANNELS);
	hw.adc.Start(); 
}

void excite(float eMag, float eLoc)
{
	dynamicStiffString -> excite(eMag, eLoc, 10);
}


