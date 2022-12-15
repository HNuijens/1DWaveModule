#pragma once

#include <unordered_map>
#include <string>

static float limit(float min, float max, float x)
{
	if(x < min) return min;
	if(x > max) return max; 
	else return x;
}

static unordered_map<string, float> defaultStringParameters =
{
		{"f0", 220.0},
	    {"L", 1.0},
        {"rho", 7850.0},
        {"A", 0.0005 * 0.0005 * M_PI},
		{"r", 0.0005},
        {"E", 2e11},
        {"I", pow (0.0005, 4) * M_PI * 0.25},
        {"sig0", 1.0},
        {"sig1", 0.005}
};

static unordered_map<string, float> defaultBarParameters =
{
		{"f0", 220.0},
		{"L", 0.5},
        {"rho", 1190},
        {"A", 0.004 * 0.004 * M_PI},
		{"r", 0.004},
        {"E", 3.2e9},
        {"I", pow (0.004, 4) * M_PI * 0.25},
        {"sig0", 1},
        {"sig1", 0.05}
};


