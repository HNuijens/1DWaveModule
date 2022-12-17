/*
  ==============================================================================

    DynamicString.h
    Created: 25 Mar 2022 11:33:06am
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
#include <algorithm>

using namespace std;

class DynamicString
{
public:

    DynamicString(); // Constructor

    ~DynamicString();                 // Destructor

    // Public Methods:
    void setFs(float Fs);
    void setGrid(unordered_map<string, float> parameters);  // set grid size N
    void setDynamicGrid(float f0);
    void exciteSystem(float width, float excitationLoc);
    void process();  
    float getOutput(float outputPos);       
    void setMaxChange();

private:

    // Private Methods:
    void addPoint();
    void removePoint();
    void getVirtualGridPoints();
    void getSchemeWeights();
    void calculateScheme();
    void updateStates();
    void resetGrid();

    // System defined:
    float Fs = 48000.f;         // Sample rate 
    float k;                    // Descrete time grid spacing
    float f0;                   // Fundamental Frequency
    
                                 // User defined:
    float L = 1.;               // String length
    float c;                    // Dynamic wave speed
    float h;                    // Grid spacing
    float N = 0;                // Current grid size
    float N1 = 0;               // Previous grid size
    float alpha;                // Distance between the systems

    // damping
    float sig0, sig1; 

    int M;                       // Length of left system
    int Mw;                      // lengtg of right system

    // State vectors:
    vector<vector<float>> u;      // State vectors of left system
    //vector<float*> u;                   // Pointer to the left state vectors

    vector<vector<float>> w;      // State vectors of right system
    //vector<float*> w;                   // Pointer to the right state vectors 

    //vector<float> U;                    // Full string
    //vector<float> newU;                 // Resized string
    // Interpolation vectors:
    vector<float> I2 = { 0,0,0 };       // Kwadratic interpolation vector 
    vector<float> I3 = { 0,0,0,0 };     // Cubic interpolation vector
    vector<float> I2Flipped = I2;       // Kwadratic interpolation vector flipped 
    vector<float> I3Flipped = I3;       // Cubic interpolation vector flipped

    // Virtual grid points:
    float uM1 = 0.f;                  // u(M+1)
    float wMin1 = 0.f;                // w(-1)
    float uPrevM1 = 0.f;              // u(M+1)^{n-1}
    float wPrevMin1 = 0.f;            // w(-1)^{n-1}

    // Connection force:
    float r1;                         // Coefficient 1
    float r2;                         // Coefficient 2
    float uMI;                        // Intermediate scheme u
    float w0I;                        // Intermediate scheme w
    float Fc;                         // Connection force 
    float omegaS = 1.;                // Frequency connection spring
    float sigmaS = 1.;                // Damping connection spring

    // Scheme weights:
    float D, C1, C2, C3, C4, C5; 

    float maxNChange = 1. / 10.;
    float maxChangeF0;

    // Output
    //float out;


    int index = 0;  // Keeping track of pointer switching

};

