#include "ofApp.h"
#include "helpers.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// Set background colour to black
	ofSetBackgroundColor(0);

	// Set variable values
	myPlaneRangeX = 640;
	myPlaneRangeY = 480;

	// Setup mode:  2=custom mesh, 3=pointcloud
	mySetupMode = 2;
    
    //load shader
    myReflectionShader.load("reflection");
    ofDisableArbTex();
    ofEnableNormalizedTexCoords();
    myEnvironmentMap.load("hdri_hub_environmentMap.jpg");



	// If we're using a custom mesh, load it from file
	if (mySetupMode == 2) {
		myInitialMesh.load("stacks.ply");
		removeDuplicateVertices(myInitialMesh, 0.0001);
	}

	// Setup GUI
	myGui.setup();
	myGui.add(myFpsLabel.setup("FPS", ofToString(ofGetFrameRate(), 2)));
	myGui.add(paramAmplitude.set("Amplitude", 5.0, 0.0, 20.0));
	myGui.add(paramFrequency.set("Frequency", 1.0, 0.0, 10.0));
	myGui.add(paramScale.set("Scale", 1.0, 0.0, 10.0));
	myGui.add(paramGridSizeX.set("Grid size X", 200, 0, 500));
	myGui.add(paramGridSizeY.set("Grid size Y", 200, 0, 500));
	myGui.add(paramShowLines.set("Show lines", false));
	myGui.add(paramShowTriangles.set("Show triangles", true));
    myGui.add(paramShader.set("Show reflection", false));
	myGui.add(buttonRestart.setup("Restart"));
	myGui.add(paramFileName.set("File name", "outFile"));
	myGui.add(buttonSaveMesh.setup("Save mesh"));

	// Setup listeners for parameters
	paramGridSizeX.addListener(this, &ofApp::gridSizeChanged);
	paramGridSizeY.addListener(this, &ofApp::gridSizeChanged);
	paramShowLines.addListener(this, &ofApp::displayModeChanged);
	paramShowTriangles.addListener(this, &ofApp::displayModeChanged);
    paramShader.addListener(this, &ofApp::displayModeChanged);
	buttonRestart.addListener(this, &ofApp::setupParticleSystem);
	buttonSaveMesh.addListener(this, &ofApp::saveMeshButtonPressed);

	// Setup the particle system
	setupParticleSystem();

	// Setup EasyCam
	myCamera.setAutoDistance(false);
	myCamera.setPosition(vec3(138.26, 267.874, 608.552));
//    myCamera.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, 2000));
    myCamera.setTarget(vec3(0.0, 200, 0.0));
    

	// Setup material to use on the particle mesh
	myMeshMaterial.setDiffuseColor(ofFloatColor(0.2, 0.5, 0.7));
	myMeshMaterial.setSpecularColor(ofFloatColor(0.7, 0.7, 0.7));
	myMeshMaterial.setAmbientColor(ofFloatColor(0.1, 0.1, 0.2));
	myMeshMaterial.setShininess(50.0);

	// Setup lights (settings for discrete GPU)
	myLight1.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
	myLight1.setSpecularColor(myLight1.getDiffuseColor());
	myLight1.setPosition(vec3(500, 0, 500));

	myLight2.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
	myLight2.setSpecularColor(myLight2.getDiffuseColor());
	myLight2.setPosition(vec3(-500, 0, -500));
    
    myParticleSystem.setupKinect();
    
    fbo.allocate(1280, 720, GL_RGBA); // with alpha, 8 bits red, 8 bits green, 8 bits blue, 8 bits alpha, from 0 to 255 in 256 steps

}

//--------------------------------------------------------------
void ofApp::update(){
	// Update the particles
	myParticleSystem.update(paramAmplitude, paramFrequency, paramScale);

	// Update the frames per second label in the GUI
	myFpsLabel = ofToString(ofGetFrameRate(), 2);
    
    myParticleSystem.updateKinect();
    
    
    ofPrimitiveMode curDisplayMode;
    if (paramShowTriangles) {
        curDisplayMode = OF_PRIMITIVE_TRIANGLES;
    }
    else if (paramShowLines) {
        curDisplayMode = OF_PRIMITIVE_LINES;
    }
    else {
        curDisplayMode = OF_PRIMITIVE_POINTS;
    }

    if (mySetupMode == 3) {
        myParticleSystem.setupUsingPointCloud(paramGridSizeX, paramGridSizeY, myPlaneRangeX, myPlaneRangeY, curDisplayMode);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (paramShader == false){
	// Start drawing objects in 3D space
	ofEnableDepthTest();
	myCamera.begin();

	// Switch on the lights
	ofEnableLighting();
	myLight1.enable();
	myLight2.enable();

	// Draw the particles
	myMeshMaterial.begin();
	myParticleSystem.draw();
	myMeshMaterial.end();

	// Switch off lighting
	ofDisableLighting();

	// Finish drawing objects in 3D space
	myCamera.end();
	ofDisableDepthTest();
    
}  else {

    // Switch on the lights
    ofEnableLighting();
    myLight1.enable();
    myLight2.enable();

        // Setup rendering in 3D
        ofEnableDepthTest();
        myCamera.begin();
    


        // Start the reflection shader
        myReflectionShader.begin();

        // Pass the parameters needed by the shader as uniforms
        myReflectionShader.setUniformTexture("environmentMap", myEnvironmentMap.getTexture(), 1);

        // Draw the 3D mesh
        myParticleSystem.draw();

        // Stop the reflection shader
        myReflectionShader.end();

     
        // Finish drawing objects in 3D space
        myCamera.end();
    
       // Switch off lighting
       ofDisableLighting();
        ofDisableDepthTest();
    }
    

	// Draw the GUI elements
	myGui.draw();
    ofDrawBitmapString("press:KEY 2 for .PLY :Key 3 Kinect render", 230, 20);
    cout<<myParticleSystem.p<<endl;
   
}

//--------------------------------------------------------------
void ofApp::setupParticleSystem() {
	ofPrimitiveMode curDisplayMode;
	if (paramShowTriangles) {
		curDisplayMode = OF_PRIMITIVE_TRIANGLES;
	}
	else if (paramShowLines) {
		curDisplayMode = OF_PRIMITIVE_LINES;
	}
	else {
		curDisplayMode = OF_PRIMITIVE_POINTS;
	}

	// Call the appropriate setup function depending whether we're
	// displaying as a plane or a sphere

	 if (mySetupMode == 2) {
		myParticleSystem.setupUsingMesh(myInitialMesh, curDisplayMode);
	}
    else if (mySetupMode == 3) {
        myParticleSystem.setupUsingPointCloud(paramGridSizeX, paramGridSizeY, myPlaneRangeX, myPlaneRangeY, curDisplayMode);
    }

}

//--------------------------------------------------------------
void ofApp::gridSizeChanged(int &v) {
	// Simply call setupParticleSystem
	setupParticleSystem();
}

//--------------------------------------------------------------
void ofApp::displayModeChanged(bool &v) {
	// Simply call setupParticleSystem
	setupParticleSystem();
}

//--------------------------------------------------------------
void ofApp::saveMeshButtonPressed() {
	// Get the file fileName to save the file
	string fileName = paramFileName.toString();

	// Check whether fileName ends with .ply, if not add it
	if (fileName.compare(fileName.length() - 4, 4, ".ply") != 0) {
		fileName += ".ply";
	}

	// Call particleMesh to save the file
	myParticleSystem.getMesh().save(fileName);
}





//--------------------------------------------------------------
void ofApp::saveImage() {
//fbo is the main drawing canvas displayed in the second window
int w = fbo.getWidth();
int h = fbo.getHeight();

unsigned char* pixels = new unsigned char[w*h*3];  ;
ofImage screenGrab;
screenGrab.allocate(w,h,OF_IMAGE_COLOR);
screenGrab.setUseTexture(false);
  
//copy the pixels from FBO to the pixel array; then set the normal ofImage from those pixels; and use the save method of ofImage
  
fbo.begin();
glPixelStorei(GL_PACK_ALIGNMENT, 1);
glReadPixels(0, 0, fbo.getWidth(), fbo.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, pixels);
screenGrab.setFromPixels(pixels, fbo.getWidth(), fbo.getHeight(), OF_IMAGE_COLOR);
screenGrab.saveImage("output_" + ofToString(ofGetFrameNum()) + ".jpg", OF_IMAGE_QUALITY_MEDIUM);
fbo.end();
ofLog(OF_LOG_VERBOSE, "[DiskOut]  saved frame " + ofToString(ofGetFrameNum()) );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
      
        case'0':
            cout<<myCamera.getPosition()<<endl;
            break;
//        case'1':
//           mySetupMode = 1;
//            setupParticleSystem();
//            break;
            
        case'2':
            mySetupMode = 2;
            setupParticleSystem();
            break;
        case'3':
            mySetupMode = 3;
            setupParticleSystem();
            break;
        case'5':
            saveImage();
            break;
            
       
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
