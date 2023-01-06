/*
  ==============================================================================

    ExcitationHandler.h
    Created: 30 Dec 2022 11:30:06am
    Author:  Helmer Nuijens

  ==============================================================================
*/

#pragma once

#include <vector>
#include <cmath>
#include <algorithm>


class ExcitationHandler
{
    public:
        ExcitationHandler();
        ~ExcitationHandler();

        void init(int bufferLength = 20, float threshold = 0.05);
        bool process(float val1, float val2);

        float ePos, eMag; 
    private:
        int bufferLength = 20; 
        std::vector<float> magBuffer; 
        std::vector<float> posBuffer;
        int bufferIdx = 0; 
        float threshold = 0.1; 
        bool excitationFlag = false; 

        //float sensorVal1, sensorVal2;

        float getPosition(float x1, float x2);

        float getMagnitude(float x1, float x2);
};
