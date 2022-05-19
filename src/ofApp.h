#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ParticleSystem.h"

using namespace glm;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void setupParticleSystem();
		void gridSizeChanged(int &v);
		void displayModeChanged(bool &v);
		void saveMeshButtonPressed();
    void saveImage();

		ParticleSystem myParticleSystem;

		ofxLabel myFpsLabel;
		ofParameter<float> paramAmplitude;
		ofParameter<float> paramFrequency;
		ofParameter<float> paramScale;
		ofParameter<int> paramGridSizeX;
		ofParameter<int> paramGridSizeY;
		ofParameter<bool> paramShowLines;
		ofParameter<bool> paramShowTriangles;
        ofParameter<bool> paramShader;
        ofxButton buttonRestart;
		ofParameter<string> paramFileName;
		ofxButton buttonSaveMesh;
		ofxPanel myGui;

		ofEasyCam myCamera;
		ofMaterial myMeshMaterial;
		ofLight myLight1;
		ofLight myLight2;

		float myPlaneRangeX;
		float myPlaneRangeY;
		float mySphereRadius;
		int mySetupMode; // 0 = plane, 1 = sphere, 2 = custom mesh
		ofMesh myInitialMesh;
    
    //Shader setup
    ofShader myReflectionShader;
    ofImage myEnvironmentMap;
    
    ofFbo fbo;
};
