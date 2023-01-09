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
#include "oled_ssd130x.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

DaisySeed hw;
std::unique_ptr<DynamicStiffString> dynamicStiffString;
ExcitationHandler excitationHandler; 
OledDisplay<SSD130x4WireSpi128x64Driver> display;

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

void configADC();
void initDisplay();

#define PIN_OLED_DC 9
#define PIN_OLED_RESET 30
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

bool excitationFlag = false; 

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	if(excitationFlag)
	{
		dynamicStiffString -> excite(excitationHandler.eMag, -1., excitationHandler.ePos, 10); 
		excitationFlag = false; 
	}
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
	initDisplay();
	excitationHandler.init();
	dynamicStiffString = std::make_unique<DynamicStiffString> (defaultDynamicStiffStringParameters, hw.AudioSampleRate());

	hw.StartAudio(AudioCallback);

	while(1) {
		for(int i = 0 ; i < parameterPins.size(); i ++)
		{
			// float input = hw.adc.GetFloat(i);
			// float value = map(input, 0., 1., parameterLimits[i][0], parameterLimits[i][1]);
			// dynamicStiffString->refreshParameter(i, value);
		}
		
		float val1 = hw.adc.GetFloat(7);
		float val2 = hw.adc.GetFloat(8); 
		
		if(excitationHandler.process(val1, val2))
		{
			excitationFlag = true; 
		}
		
		std::vector<float> u = dynamicStiffString -> getStringState();
		float screenSpacing = static_cast<float>(SCREEN_WIDTH) / u.size(); 
		
		display.Fill(false);
		for(int i = 1; i < u.size(); i++)
		{	
			int x1 = limit(0, SCREEN_WIDTH, floor((i-1)*screenSpacing));
			int x2 = limit(0, SCREEN_WIDTH, floor(i*screenSpacing));
			int y1 = limit(0, SCREEN_HEIGHT, 32 + floor(32*u[i-1]));
			int y2 = limit(0, SCREEN_HEIGHT, 32 + floor(32*u[i]));
			display.DrawLine(x1, y1, x2, y2, true);
		}

		int x = SCREEN_WIDTH * excitationHandler.ePos; 
		display.DrawLine(x, 32 - 16 * excitationHandler.eMag, x, 32 + 16 * excitationHandler.eMag, true);
		display.Update();		
	
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

void initDisplay()
{
    OledDisplay<SSD130x4WireSpi128x64Driver>::Config display_config;

    display_config.driver_config.transport_config.pin_config.dc
        = hw.GetPin(PIN_OLED_DC);
    display_config.driver_config.transport_config.pin_config.reset
        = hw.GetPin(PIN_OLED_RESET);

    display.Init(display_config);
}




