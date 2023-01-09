/*
  ==============================================================================

    ExcitationHandler.cpp
    Created: 30 Dec 2022 11:30:06am
    Author:  Helmer Nuijens

  ==============================================================================
*/

#include "ExcitationHandler.h"

ExcitationHandler::ExcitationHandler()
{

}

ExcitationHandler::~ExcitationHandler()
{

}

void ExcitationHandler::init(int bufferLength, float threshold)
{
    magBuffer = std::vector<float>(bufferLength, 0.);
    posBuffer = std::vector<float>(bufferLength, 0.);

	this->threshold = threshold; 
}

bool ExcitationHandler::process(float val1, float val2)
{
    // returns true if excited
    // excitation detection:

    // sensorVal1 = val1; 
    // sensorVal2 = val2; 

    float magnitude = getMagnitude(val1, val2); 
    float pos = getPosition(val1, val2);

    magBuffer[bufferIdx % bufferLength] = magnitude;
    posBuffer[bufferIdx % bufferLength] = pos;

    if(bufferIdx > bufferLength) 
        bufferIdx = 0;

    if(!excitationFlag && (magnitude > threshold))
    {
        excitationFlag = true; 
    }

    if(excitationFlag && (magnitude < threshold))
    {
        int maxIndex = distance(magBuffer.begin(), max_element(magBuffer.begin(), magBuffer.end()));
        eMag = limit(0,1,magBuffer[maxIndex]);
        ePos = limit(0,1,posBuffer[maxIndex]);
        excitationFlag = false; 
        return true;
    }

    bufferIdx ++; 
    return false; 
}

float ExcitationHandler::getPosition(float x1, float x2)
{
    return (abs(x1 - 0.5) - abs(x2 - 0.5) + 0.5);
}

float ExcitationHandler::getMagnitude(float x1, float x2)
{
    return 2 * std::max(abs(x1 - 0.5), abs(x2 - 0.5));
}


