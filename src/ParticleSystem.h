#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

using namespace glm;

class ParticleSystem {
public:
	void setupPlane(int gridSizeX, int gridSizeY, float planeRangeX, float planeRangeY, ofPrimitiveMode displayMode);
	void setupSphere(int gridSizeX, int gridSizeY, float sphereRadius, ofPrimitiveMode displayMode);
	void setupUsingMesh(ofMesh inputMesh, ofPrimitiveMode displayMode);
	void update(float amplitude, float frequency, float scale);
	void draw();
	ofMesh getMesh();
    void setupKinect();
    void updateKinect();
    void setupUsingPointCloud(int gridSizeX, int gridSizeY, float planeRangeX, float planeRangeY, ofPrimitiveMode displayMode);
    float p;

private:
	int getParticleIndex(int x, int y);
    int angle;// kinect start angle
    
	vector<Particle> myParticles;
	ofMesh myMesh, mesh;
	ofPrimitiveMode myDisplayMode;
	int myGridSizeX;
	int myGridSizeY;
    // used for viewing the point cloud
    ofEasyCam easyCam;
    ofxKinect kinect;
    
    
    
};
