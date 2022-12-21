/*
  ==============================================================================

    DynamicStiffString.h
    Created: 7 Feb 2022 5:09:58pm
    Author:  Silvin Willemsen
    
    Edited by Helmer Nuijens

  ==============================================================================
*/

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


#pragma once

#include "Global.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <cmath>

//==============================================================================
/*
*/
class DynamicStiffString
{
public:
    //DynamicStiffString();
    DynamicStiffString(unordered_map<string, float> parameters, float sr);
    ~DynamicStiffString();

    void calculateScheme();
    void updateStates();

    void process();
    
    void refreshParameter (int changedParameterIdx, float changedParameterValue);
    
    void refreshCoefficients (bool init = false);
    
    //return u at the current sample at a location given by the length ratio

    float getOutput()
    {
        return v[1][6]; // set to be fixed due to varying N
    }
    
    void excite (float amp = 1., int loc = -1, float width = 10);
    //void mouseDown (const MouseEvent& e) override;
    
    bool shouldExcite() { return excitationFlag; };
    
    void addRemovePoint();
    void refreshCustomIp();
private: 
    // Model parameters
    float f0, L, rho, r, A, T, E, I, cSq, kappaSq, sigma0, sigma1, lambdaSq, muSq, h, k;
    float origR, origL, origE, origT, origRho;
    std::vector<float*> parameterPtrs; // to easily locate parameters
    std::vector<float> parametersToGoTo;
    std::vector<bool> parameterChanged;

    // Number of intervals (N+1 is number of points including boundaries)
    int N, Nmax, Nprev = 0;
    
    // Number of intervals of subsystems
    int Mv;
    const int Mw = 1; // Mw is static
    
    // Fractional number of intervals used for dynaic grid
    float Nfrac, NfracPrev;
    float alf, Iterm, A0, A1, A2, A3, AA;
    
    // (N+1) x 3 'matrices' containing the state of the left and right system at all time-steps
    std::vector<std::vector<float>> vStates;
    std::vector<std::vector<float>> wStates;

    // vectors of pointers that point to state vectors
    std::vector<float*> v;
    std::vector<float*> w;

    /* Scheme variables
        - Adiv for u^{n+1} (that all terms get divided by)
        - B for u^n
        - C for u^{n-1}
        - S for precalculated sigma terms
    */
    float Adiv, B0, Bss, B1, B2, C0, C1, S0, S1;
    
    // flag to tell MainComponent whether to excite the scheme or not
    bool excitationFlag = false;
    
    // initialise location of excitation
    float excitationLoc = 0.22;
    
    bool clamped = true;
    
    std::vector<float> customIp;
};
