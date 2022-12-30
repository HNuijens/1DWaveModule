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
#include "ExcitationHandler.h"
//#include "oled_ssd130x.h"


using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

//OledDisplay<SSD130x4WireSpi128x64Driver> display;

std::vector<Pin> parameterPins =  {A0, A1, A2, A3, A4, A5, A6}; 

enum AdcChannel {
   lengthKnob = 0,
   densityKnob,
   radiusKnob,
   tensionKnob,
   elasticityKnob,
   freqIndepKnob,
   freqDepKnob,
   exciter1,
   exciter2,
   NUM_ADC_CHANNELS
};

DaisySeed hw;
std::unique_ptr<DynamicStiffString> dynamicStiffString;
ExcitationHandler excitationHandler; 

void configADC();


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
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

	//dynamicStiffString -> excite();

	excitationHandler.init();
	hw.StartAudio(AudioCallback);

	while(1) {
		for(int i = 0 ; i < parameterPins.size(); i ++)
		{
			float input = hw.adc.GetFloat(i);
			float value = map(input, 0., 1., parameterLimits[i][0], parameterLimits[i][1]);
			dynamicStiffString->refreshParameter(i, value);
		}
		
		float val1 = hw.adc.GetFloat(7);
		float val2 = hw.adc.GetFloat(8); 
		
		if(excitationHandler.process(val1, val2))
		{
			dynamicStiffString -> excite(excitationHandler.eMag, -1., excitationHandler.ePos, 10); 
		}
		
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

	adc_config[7].InitSingle(A7);
	adc_config[8].InitSingle(A8);


	hw.adc.Init(adc_config, NUM_ADC_CHANNELS);
	hw.adc.Start(); 
}




