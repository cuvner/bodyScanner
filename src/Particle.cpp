#include "Particle.h"
#include "helpers.h"

//--------------------------------------------------------------
void Particle::setup(vec3 pos, vec3 dir) {
	myPos = pos;
	myOrigPos = pos;
	myDir = dir;
	mySize = 0.1;
}

//--------------------------------------------------------------
void Particle::update(float amplitude, float frequency, float scale) {
	float phase = frequency * ofGetElapsedTimef();
	float sinVal = sinf(myOrigPos.x / scale + phase) * sinf(myOrigPos.y / scale + phase) * sinf(myOrigPos.z / scale + phase);
	float noiseVal = ofNoise(myOrigPos / scale);

	myPos = myOrigPos + amplitude * sinVal * noiseVal * myDir;
}

//--------------------------------------------------------------
void Particle::draw() {
	ofDrawSphere(myPos, mySize);
}

//--------------------------------------------------------------
vec3 Particle::getPos() {
	return myPos;
}
