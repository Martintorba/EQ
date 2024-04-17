#include "ofApp.h"

void ofApp::setup() {
	
	plotHeight = 128;
	bufferSize = 512;

	gui.setup();
	gui.setPosition(20, 450);
	gui.add(frequencySlider.setup("Frequency", 10000, 20, 20000));
	gui.add(gainSlider.setup("Gain", 0.0, -10, 10.0));

	fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_BARTLETT); 


	audioInput = new float[bufferSize];
	fftOutput = new float[fft->getBinSize()];
	eqFunction = new float[fft->getBinSize()];
	eqOutput = new float[fft->getBinSize()];
	ifftOutput = new float[bufferSize];

	//Output settingsi
	
	ofSoundStreamSettings settings;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 2;
	settings.sampleRate = 44100;
	settings.bufferSize = bufferSize;
	settings.numBuffers = 4;


	settings.setInListener(this);
	settings.setOutListener(this);

	ofSoundStreamSetup(settings);


	/*
	const vector<ofSoundDevice>& devices = ofSoundStreamListDevices();
	for (const auto& device : devices) {
		if (device.name == "Realtek ASIO") {
			settings.setOutDevice(device);
			break;
		}
	}
	*/

	ofLog() << "blabla";
	//ofSoundStreamListDevices();

	
	
	appWidth = ofGetWidth();
	appHeight = ofGetHeight();

	//ofSoundStreamSetup(1, 2, this, 44100, bufferSize, 4);

	
	ofBackground(100, 0, 0);
}


void ofApp::update() {
	
	audioReceived(audioInput , bufferSize, 4);
	
	
}

void ofApp::draw() {
	ofSetHexColor(0xffffff);
	ofPushMatrix();

	glTranslatef(16, 16, 0);
	ofDrawBitmapString("Audio Input", 0, 0);
	plot(audioInput, bufferSize, plotHeight / 2, 0);

	glTranslatef(0, plotHeight + 16, 0);
	ofDrawBitmapString("FFT Output", 0, 0);
	plot(fftOutput, fft->getBinSize(), -plotHeight, plotHeight / 2);

	ofPushMatrix();
	glTranslatef(fft->getBinSize(), 0, 0);
	ofDrawBitmapString("EQd FFT Output", 0, 0);
	plot(eqOutput, fft->getBinSize(), -plotHeight, plotHeight / 2);
	ofPopMatrix();

	glTranslatef(0, plotHeight + 16, 0);
	ofDrawBitmapString("IFFT Output", 0, 0);
	plot(ifftOutput, fft->getSignalSize(), plotHeight / 2, 0);

	ofPopMatrix();
	string msg = ofToString((int)ofGetFrameRate()) + " fps";
	ofDrawBitmapString(msg, appWidth - 80, appHeight - 20);

	gui.draw();
}

void ofApp::plot(float* array, int length, float scale, float offset) {
	ofNoFill();
	ofDrawRectangle(0, 0, length, plotHeight);
	glPushMatrix();
	glTranslatef(0, plotHeight / 2 + offset, 0);
	ofBeginShape();
	for (int i = 0; i < length; i++)
		ofVertex(i, array[i] * scale);
	ofEndShape();
	glPopMatrix();
}

void ofApp::audioReceived(float* input, int bufferSize, int nChannels) {

	mode == MIC;
	if (mode == MIC) {
		memcpy(audioInput, input, sizeof(float) * bufferSize);
	}
	else if (mode == NOISE) {
		for (int i = 0; i < bufferSize; i++)
			audioInput[i] = ofRandom(-1,1);
	}
	



	//__________________________________________________________________________________________	
	for (int i = 0; i < bufferSize; i++) {
		if (audioInput[i] > 1) {
			
			//ofLog() << "Input[" << i << "]: " << audioInput[i];

		}
	}	
	
	
	
	fft->setSignal(audioInput);
	memcpy(fftOutput, fft->getAmplitude(), sizeof(float) * fft->getBinSize());


	ofLog() << "Bin size fft: "<< fft->getBinSize();
	for (int i = 0; i < fft->getBinSize(); i++) {
		if (fftOutput[i] > 1) {
			float hz = frequencyAtIndex(i, 44100, fft->getBinSize());
			//ofLog() <<"ffOutput["<<i<<"]" << fftOutput[i] << " Frekvenca: " << hz;
		}
	}
	//_________________________________________________________________________________________



	
	//////////////////////////////////////////////////////////////////////////////////// EQ



	float freq = frequencySlider.getParameter().cast<float>();
	float gain = gainSlider.getParameter().cast<float>();
	

	//ofLog() << "Bin size: " << fft->getBinSize();

	for (int i = 0; i < fft->getBinSize(); i++) {
		//eqFunction[i] = (float)(fft->getBinSize() - i) * (float)fft->getBinSize();
		
	}


	int j = indexAtFrequency(freq, 44100, fft->getBinSize());

	ofLog() << "j: " << j;

	for (int i = 0; i < fft->getBinSize(); i++) {

		if(i==j || i==j+1)
			eqOutput[i] = fftOutput[i]*gain;
		else eqOutput[i] = fftOutput[i];
	}

	
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////


	fft->setPolar(eqOutput, fft->getPhase());
	fft->clampSignal();
	memcpy(ifftOutput, fft->getSignal(), sizeof(float) * fft->getSignalSize());
}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'm':
		mode = MIC;
		break;
	case 'n':
		mode = NOISE;
		break;
	}
}

float ofApp::frequencyAtIndex(int index, float sampleRate, int fftSize) {

	return ((float)index / (float)fftSize) * (sampleRate / 2.0);


}

int ofApp::indexAtFrequency(int hz, float sampleRate, int fftSize) {

	return ((int)hz * (int)fftSize) / (sampleRate / 2.0);


}

void ofApp :: audioOut(ofSoundBuffer& buffer) {

	for (size_t i = 0; i < buffer.getNumFrames(); ++i) {

		buffer[i * buffer.getNumChannels()] = ifftOutput[i];
		buffer[i * buffer.getNumChannels() + 1] = ifftOutput[i];
	}

}
