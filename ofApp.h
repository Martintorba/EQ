#pragma once

#include "ofMain.h"
#include "ofxFft.h"
#include "ofxGui.h"

#define MIC 0
#define NOISE 1


class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void plot(float* array, int length, float scale, float offset);
	void audioReceived(float* input, int bufferSize, int nChannels);
	void draw();
	void keyPressed(int key);
	void audioOut(ofSoundBuffer& buffer);
	float frequencyAtIndex(int index, float sampleRate, int fftSize);
	int indexAtFrequency(int hz, float sampleRate, int fftSize);

	int plotHeight, bufferSize;

	ofxFft* fft;
	ofxPanel gui;

	ofxFloatSlider frequencySlider;
	ofxFloatSlider gainSlider;



	float* audioInput;
	float* fftOutput;
	float* eqFunction;
	float* eqOutput;
	float* ifftOutput;

	float appWidth;
	float appHeight;

	int mode;
};
