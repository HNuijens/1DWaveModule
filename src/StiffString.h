/*
  ==============================================================================

    StiffString.h
    Created: 13 Dec 2022 1:24:42pm
    Author:  Helmer Nuijens

  ==============================================================================
*/
#pragma once

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

class StiffString
{

public:
    StiffString();      // Constructor
    ~StiffString();     // Destructor
    
    void setFs(float Fs);
    void setGrid(unordered_map<string, float> parameters);
    
    //float getNextSample(float outputPos);
    void exciteSystem(float amp, float pos, int width, bool strike);

    void process();
    float getOutput(float outputPos);

    float Fs = 48000.0;

private:
    void calculateScheme();
    void updateStates();
   
    float h, k, L, c, f0, r, A, I, E, rho, sig0, sig1, kappaSq, lambdaSq;  // parameters
    float S0, S1, K, D, G0_0, G0_1, G0_2, G1_0, G1_1, G0_0SP;              // Stencil factors
    int N;                                                                 // grid size

    vector<vector<float>> uStates;                                         // vector containing the grid states
    vector<float*> u;                                                      // vector with pointers to the grid states
};