#include "ParticleSystem.h"
#include "helpers.h"



//--------------------------------------------------------------
void ParticleSystem::setupUsingMesh(ofMesh inputMesh, ofPrimitiveMode displayMode) {
	// Clear any existing particles and mesh data
	myParticles.clear();

	// Copy the input mesh into myMesh
	myMesh = inputMesh;

	// Set the display mode
	myDisplayMode = displayMode;
	myMesh.setMode(myDisplayMode);

	// If the mesh doesn't have any normals, calculate them4
	if (myMesh.getNumNormals() != myMesh.getNumVertices()) {
		calcNormals(myMesh);
	}

	// Declare a particle for each vertex, using the normals
	// from the mesh as the direction to move each particle
	for (int i = 0; i < myMesh.getNumVertices(); i++) {
		vec3 p = myMesh.getVertex(i);
		vec3 n = myMesh.getNormal(i);
		Particle par;
		par.setup(p, n);
		myParticles.push_back(par);
	}

	// Assuming that the original .ply file was a set of triangles we've
	// already got the triangle list data.

	// If the display mode is lines we need to convert from the triangles
	// to lines

	if (myDisplayMode == OF_PRIMITIVE_LINES) {
		// Calculated index list for lines. For simplicity here we're just
		// going to declare every edge of every triangle as a line. This
		// means that a lot of lines are probably going to get drawn twice.
		// This could be cleaned up, but would require quite a lot more
		// processing for the general case to work with any mesh.

		vector<ofIndexType> lineIndexList;
		int numTriangles = myMesh.getNumIndices() / 3;
		for (int i = 0; i < numTriangles; i++) {
			// Declare three lines for each triangle
			ofIndexType v0 = myMesh.getIndex(3 * i);
			ofIndexType v1 = myMesh.getIndex(3 * i + 1);
			ofIndexType v2 = myMesh.getIndex(3 * i + 2);

			lineIndexList.push_back(v0);
			lineIndexList.push_back(v1);

			lineIndexList.push_back(v1);
			lineIndexList.push_back(v2);

			lineIndexList.push_back(v2);
			lineIndexList.push_back(v0);
		}

		// Replace the indices in the mesh with lineIndexList
		myMesh.clearIndices();
		myMesh.addIndices(lineIndexList);
	}

	// Set the correct display mode on the mesh
	myMesh.setMode(myDisplayMode);
}

//--------------------------------------------------------------
void ParticleSystem::setupUsingPointCloud(int gridSizeX, int gridSizeY, float planeRangeX, float planeRangeY, ofPrimitiveMode displayMode) {
    myParticles.clear();
    myMesh.clear();
    int w = planeRangeX;
    int h = planeRangeY;
    // Store grid size values into member variables
    myGridSizeX = gridSizeX;
    myGridSizeY = gridSizeY;
    myMesh.setMode(displayMode);
    int step = 2;
    
    //LOG if kinect is detected
    bool connection = kinect.isConnected();
    
    if(connection == false){
        cout<<"NO KINECT DETECTED"<<endl;
    } else if (connection == true){
        cout<<"Kinect Detected"<<endl;
    }
    
    
    if(kinect.isInitialized() == false){
        cout<<"IS KINECT CONNECTED?"<<endl;
    } else {
    
    for (int i = 0; i < myGridSizeX; i++){
           for (int j = 0; j < myGridSizeY; j++) {
               float x  = ofMap(i, 0, myGridSizeX - 1, 0, planeRangeX);
               float y = ofMap(j, 0, myGridSizeY - 1, 0, planeRangeY);
               float z = 1;
               // The point cloud needs reversing and repositioning
               float reverseY = ofMap(y, 0, 480, 480, 0);
               //Create a variable to reposition x value, this centres the mesh
               //To get connect point cloud the planeRangeX has to match Kinect resolution
               float xRePosition = -0.5*planeRangeX;
            if(kinect.getDistanceAt(x, y) > 0 && kinect.getDistanceAt(x, y) < 800) {
                //put the kinect depth values into the mesh, use a threshold for depth
                vec3 depthPt = vec3(kinect.getWorldCoordinateAt(x, y));
                float zOffset =  depthPt.z - 800;
                vec3 pos = vec3(x + xRePosition, reverseY, -zOffset);
                vec3 dir = vec3(0, 0, 1);
                Particle par;
                par.setup(pos, dir);
                myParticles.push_back(par);
            } else {
                vec3 pos = vec3(x + xRePosition, reverseY, -01);
                vec3 dir = vec3(0, 0, 1);
                Particle par;
                par.setup(pos, dir);
                myParticles.push_back(par);
            }
        }
    }
    // Create a vertex for each particle
    for (int i = 0; i < myParticles.size(); i++) {
        vec3 pos = myParticles[i].getPos();
        myMesh.addVertex(pos);
    }
    
    // Initialize the mesh
    if (myDisplayMode == OF_PRIMITIVE_POINTS) {
        // For points we don't need to declare anything more
    }
    else if (myDisplayMode == OF_PRIMITIVE_LINES) {
        // Declare lines connecting the vertices in a square grid using the vertex indices
        // We add lines by creating a list of pairs of vertex indices that should be
        // connected by lines
        for (int i = 0; i < myGridSizeX; i++) {
            for (int j = 0; j < myGridSizeY; j++) {
                if (i < myGridSizeX - 1) {
                    myMesh.addIndex(getParticleIndex(i, j));
                    myMesh.addIndex(getParticleIndex(i + 1, j));
                }

                if (j < myGridSizeY - 1) {
                    myMesh.addIndex(getParticleIndex(i, j));
                    myMesh.addIndex(getParticleIndex(i, j + 1));
                }
            }
        }
    }
    else if (myDisplayMode == OF_PRIMITIVE_TRIANGLES) {
        // Declare two triangles for each square in the grid
        for (int i = 0; i < myGridSizeX - 1; i++) {
            for (int j = 0; j < myGridSizeY - 1; j++) {
                myMesh.addTriangle(
                    getParticleIndex(i, j),
                    getParticleIndex(i + 1, j + 1),
                    getParticleIndex(i + 1, j));

                myMesh.addTriangle(
                    getParticleIndex(i, j),
                    getParticleIndex(i, j + 1),
                    getParticleIndex(i + 1, j + 1));
            }
        }

        // Add a normal for each vertex. This can be done by calling the
        // calcNormals helper function
        calcNormals(myMesh);
    }
    else {
        ofLogError("ParticleSystem::setup, displayMode set to invalid value");
    }


    }
}


//--------------------------------------------------------------
void ParticleSystem::update(float amplitude, float frequency, float scale) {
	for (int i = 0; i < myParticles.size(); i++) {
		myParticles[i].update(amplitude, frequency, scale);
		myMesh.setVertex(i, myParticles[i].getPos());
	}

	// If we've got a mesh of triangles we need to update the vertex normals
	if (myDisplayMode == OF_PRIMITIVE_TRIANGLES) {
		calcNormals(myMesh);
	}
}

//--------------------------------------------------------------
void ParticleSystem::draw() {
	myMesh.draw();
}

//--------------------------------------------------------------
int ParticleSystem::getParticleIndex(int x, int y) {
	x = ofWrap(x, 0, myGridSizeX);
	y = ofWrap(y, 0, myGridSizeY);
	return myGridSizeY * x + y;
}

//--------------------------------------------------------------
ofMesh ParticleSystem::getMesh() {
	return myMesh;
}
//--------------------------------------------------------------
void ParticleSystem::setupKinect(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // enable depth->video image calibration
    kinect.setRegistration(true);
    
//    kinect.init();
    //kinect.init(true); // shows infrared instead of RGB video image
    kinect.init(false, false); // disable video image (faster fps)
    
    kinect.open();        // opens first available kinect
    //kinect.open(1);    // open a kinect by id, starting with 0 (sorted by serial # lexicographically))
    //kinect.open("A00362A08602047A");    // open a kinect using it's unique serial #
    
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    

    
    ofSetFrameRate(60);
    
    // zero the tilt on startup
    angle = 0;
    kinect.setCameraTiltAngle(angle);

}
//--------------------------------------------------------------
void ParticleSystem::updateKinect(){
    
    kinect.update();
}
//--------------------------------------------------------------
