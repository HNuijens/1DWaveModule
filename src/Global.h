/*
  ==============================================================================

    Global.h
    Created: 15 Dec 2022 1:24:42pm
    Author:  Helmer Nuijens

  ==============================================================================
*/

#pragma once

#include <unordered_map>
#include <string>

using namespace std; 

 static double N_MAX_CHANGE = 1.0 / 20.0;
 static double SIG_1_MIN = 0.0002;

static float limit(float min, float max, float x)
{
    if(x < min)
        return min;
    if(x > max)
        return max;
    else
        return x;
}

static float map(float value, float oldMin, float oldMax, float newMin, float newMax)
{
    return (value - oldMin) / (oldMax - oldMin) * (newMax - newMin) + newMin;
}

static unordered_map<string, float> defaultStiffStringParameters
    = {{"f0", 220.0},
       {"L", 1.0},
       {"rho", 7850.0},
       {"A", 0.0005 * 0.0005 * M_PI},
       {"r", 0.0005},
       {"E", 2e11},
       {"I", pow(0.0005, 4) * M_PI * 0.25},
       {"sig0", 1.0},
       {"sig1", 0.005}};

static unordered_map<string, float> defaultBarParameters
    = {{"f0", 220.0},
       {"L", 0.5},
       {"rho", 1190},
       {"A", 0.004 * 0.004 * M_PI},
       {"r", 0.004},
       {"E", 3.2e9},
       {"I", pow(0.004, 4) * M_PI * 0.25},
       {"sig0", 1},
       {"sig1", 0.05}};

static unordered_map<string, float> defaultStringParameters
    = {{"f0", 220.0},
       {"L", 0.5},
       {"sig0", 1},
       {"sig1", 0.05}};

static unordered_map<string, float> defaultDynamicStiffStringParameters
    = {{"f0", 220.0},
       {"L", 1.0},
       {"rho", 7850.0},
       {"T", 300},
       {"A", 0.0005 * 0.0005 * M_PI},
       {"r", 0.0005},
       {"E", 2e11},
       {"I", pow(0.0005, 4) * M_PI * 0.25},
       {"sig0", 1.0},
       {"sig1", 0.005}};      


static float parameterLimits[7][2] = {
    {0.1, 2.},
    {3925., 15700.},
    {0.00025, 0.001},
    {0., 600.},
    {10000000000., 40000000000000.},
    {0., 2.},
    {0.0002, 0.05}
    }; 


