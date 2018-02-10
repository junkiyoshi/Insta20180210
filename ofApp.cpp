#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	ofEnableDepthTest();

	this->note.load("note.png");
	this->sound.load("I_Did_That.mp3");
	this->sound.play();

	this->n_bands_to_get = 64;

	for (int j = 0; j < this->n_bands_to_get; j++) {
		this->fft_smoothed.push_back(0);
	}

	this->light.setPosition(ofVec3f(0, 0, 1024));
	this->light.enable();
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSoundUpdate();
	float* val = ofSoundGetSpectrum(this->n_bands_to_get);

	for (int i = 0; i < this->n_bands_to_get; i++) {

		this->fft_smoothed[i] *= 0.96f;
		if (this->fft_smoothed[i] < val[i]) {

			this->fft_smoothed[i] = val[i];
		}
	}

	log.push_back(this->fft_smoothed);

	if (log.size() > 10) {
			this->log.erase(this->log.begin());
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(200);
	this->note.resize(50, 50);
	this->note.draw(ofVec2f(-(this->note.getWidth() / 2), -(this->note.getHeight() / 2)));

	if (this->sound.getIsPlaying()) {

		float radius = 80;

		for (int l = this->log.size() - 1; l > -1 ; l--)
		{
			for (int i = 0; i < this->n_bands_to_get; i++) {

				float deg = i * (360.f / this->n_bands_to_get);
				float height = this->log[l][i] * 50 * i / 3;

				float x = radius * cos(deg * DEG_TO_RAD);
				float y = radius * sin(deg * DEG_TO_RAD);
				ofVec3f point(x, y, height / 2);

				ofColor line_color;
				line_color.setHsb(255 - deg / 360 * 255, 255, 255);
				ofSetColor(line_color);

				ofDrawBox(point, 5, 5, height);
			}

			radius += 8;
		}
	}

	this->cam.end();
}

//========================================================================
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}