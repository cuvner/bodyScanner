#include "helpers.h"

//--------------------------------------------------------------
float fbm(float x, int numOctaves) {
	float result = 0.0;

	float freq = 1.0;
	float amp = 1.0;
	for (int i = 0; i < numOctaves; i++) {
		result += amp * ofSignedNoise(freq * x);
		freq *= 2.0;
		amp *= 0.5;
	}

	return result;
}

//--------------------------------------------------------------
float fbm(vec2 v2, int numOctaves) {
	float result = 0.0;

	float freq = 1.0;
	float amp = 1.0;
	for (int i = 0; i < numOctaves; i++) {
		result += amp * ofSignedNoise(freq * v2);
		freq *= 2.0;
		amp *= 0.5;
	}

	return result;
}
//--------------------------------------------------------------
float fbm(vec3 v3, int numOctaves) {
	float result = 0.0;

	float freq = 1.0;
	float amp = 1.0;
	for (int i = 0; i < numOctaves; i++) {
		result += amp * ofSignedNoise(freq * v3);
		freq *= 2.0;
		amp *= 0.5;
	}

	return result;
}
//--------------------------------------------------------------
float fbm(vec4 v4, int numOctaves) {
	float result = 0.0;

	float freq = 1.0;
	float amp = 1.0;
	for (int i = 0; i < numOctaves; i++) {
		result += amp * ofSignedNoise(freq * v4);
		freq *= 2.0;
		amp *= 0.5;
	}

	return result;
}

//--------------------------------------------------------------
// Random values used as offsets

const float off0 = 90.44;
const float off1 = 66.38;
const float off2 = 56.27;
const float off3 = 54.31;
const float off4 = 23.22;
const float off5 = 80.44;
const float off6 = 88.73;
const float off7 = 34.16;
const float off8 = 78.05;
const float off9 = 43.40;
const float off10 = 76.67;
const float off11 = 80.67;

//--------------------------------------------------------------
// Noise functions with vec2 results

vec2 fbm_vec2(float x, int numOctaves) {
	return vec2(
		fbm(x, numOctaves),
		fbm(x + off0, numOctaves));
}

vec2 fbm_vec2(vec2 v2, int numOctaves) {
	return vec2(
		fbm(v2, numOctaves), 
		fbm(v2 + vec2(off0, off1), numOctaves));
}

vec2 fbm_vec2(vec3 v3, int numOctaves) {
	return vec2(
		fbm(v3, numOctaves), 
		fbm(v3 + vec3(off0, off1, off2), numOctaves));
}

vec2 fbm_vec2(vec4 v4, int numOctaves) {
	return vec2(
		fbm(v4, numOctaves), 
		fbm(v4 + vec4(off0, off1, off2, off3), numOctaves));
}

//--------------------------------------------------------------
// Noise functions with vec3 results

vec3 fbm_vec3(float x, int numOctaves) {
	return vec3(
		fbm(x, numOctaves),
		fbm(x + off0, numOctaves),
		fbm(x + off1, numOctaves));
}

vec3 fbm_vec3(vec2 v2, int numOctaves) {
	return vec3(
		fbm(v2, numOctaves),
		fbm(v2 + vec2(off0, off1), numOctaves),
		fbm(v2 + vec2(off2, off3), numOctaves));
}

vec3 fbm_vec3(vec3 v3, int numOctaves) {
	return vec3(
		fbm(v3, numOctaves),
		fbm(v3 + vec3(off0, off1, off2), numOctaves),
		fbm(v3 + vec3(off3, off4, off5), numOctaves));
}

vec3 fbm_vec3(vec4 v4, int numOctaves) {
	return vec3(
		fbm(v4, numOctaves),
		fbm(v4 + vec4(off0, off1, off2, off3), numOctaves),
		fbm(v4 + vec4(off4, off5, off6, off7), numOctaves));
}

//--------------------------------------------------------------
// Noise functions with vec3 results

vec4 fbm_vec4(float x, int numOctaves) {
	return vec4(
		fbm(x, numOctaves),
		fbm(x + off0, numOctaves),
		fbm(x + off1, numOctaves),
		fbm(x + off2, numOctaves));
}

vec4 fbm_vec4(vec2 v2, int numOctaves) {
	return vec4(
		fbm(v2, numOctaves),
		fbm(v2 + vec2(off0, off1), numOctaves),
		fbm(v2 + vec2(off2, off3), numOctaves),
		fbm(v2 + vec2(off4, off5), numOctaves));
}

vec4 fbm_vec4(vec3 v3, int numOctaves) {
	return vec4(
		fbm(v3, numOctaves),
		fbm(v3 + vec3(off0, off1, off2), numOctaves),
		fbm(v3 + vec3(off3, off4, off5), numOctaves),
		fbm(v3 + vec3(off6, off7, off8), numOctaves));
}

vec4 fbm_vec4(vec4 v4, int numOctaves) {
	return vec4(
		fbm(v4, numOctaves),
		fbm(v4 + vec4(off0, off1, off2, off3), numOctaves),
		fbm(v4 + vec4(off4, off5, off6, off7), numOctaves),
		fbm(v4 + vec4(off8, off9, off10, off11), numOctaves));
}

//--------------------------------------------------------------
float bias(float x, float b) {
	return x / (((1.0 / b - 2.0)*(1.0 - x)) + 1.0);
}

//--------------------------------------------------------------
float gain(float x, float g) {
	if (x < 0.5)
		return bias(x * 2.0, g) / 2.0;
	else
		return bias(x * 2.0 - 1.0, 1.0 - g) / 2.0 + 0.5;
}

//--------------------------------------------------------------
void calcNormals(ofMesh &curMesh) {
	// Calculates normals for triangle meshes with indices
	// Mesh has to be of type OF_PRIMITIVE_TRIANGLES

	// Quick exit if this isn't a triangle mesh
	if (curMesh.getMode() != OF_PRIMITIVE_TRIANGLES) {
		return;
	}

	// Quick exit if we're not using indices
	if (!curMesh.hasIndices()) {
		return;
	}

	// Since we're using it frequently, store the number of
	// vertices in a local variable
	int numVertices = curMesh.getNumVertices();

	// If we don't already have a normal for each vertex, add them to the mesh
	if (curMesh.getNumNormals() != curMesh.getNumVertices()) {
		curMesh.clearNormals();
		for (int i = 0; i < numVertices; i++) {
			curMesh.addNormal(vec3(0, 0, 0));
		}
	}

	// Get vertex position arrays
	const vector<vec3> vertices = curMesh.getVertices();

	// Initialize an array for an accumulated sum for each normal
	vector<vec3> sumNormals;
	sumNormals.resize(numVertices, vec3(0, 0, 0));

	// Each set of 3 consecutive indices indicates one triangle
	const vector<ofIndexType> indices = curMesh.getIndices();
	int numTriangles = indices.size() / 3;

	// Loop through each triangle
	for (int i = 0; i < numTriangles; i++) {
		// Get indices of the the verts for this triangle
		int v0 = indices[3 * i];
		int v1 = indices[3 * i + 1];
		int v2 = indices[3 * i + 2];

		// Calculate weighted normal due to this triangle
		vec3 d0 = vertices[v1] - vertices[v0];
		vec3 d1 = vertices[v2] - vertices[v0];
		vec3 w = cross(d1, d0);

		// Add weighted value to the normal to the sum at each vertex
		sumNormals[v0] += w;
		sumNormals[v1] += w;
		sumNormals[v2] += w;
	}

	// Loop through each vertex and re-normalize the accumulated sum
	// to calculate the final value for each normal
	for (int i = 0; i < numVertices; i++) {
		curMesh.setNormal(i, normalize(sumNormals[i]));
	}
}

//--------------------------------------------------------------
void removeDuplicateVertices(ofMesh &curMesh, float threshold) {
	// Get lists for old and new vert data
	std::vector<vec3>& oldVerts = curMesh.getVertices();
	std::vector<vec3> newVerts;

	std::vector<ofFloatColor>& oldColors = curMesh.getColors();
	std::vector<ofFloatColor> newColors;

	std::vector<vec2>& oldTCoords = curMesh.getTexCoords();
	std::vector<vec2> newTCoords;

	std::vector<vec3>& oldNormals = curMesh.getNormals();
	std::vector<vec3> newNormals;

	std::vector<ofIndexType>& oldIndices = curMesh.getIndices();
	std::vector<ofIndexType> newIndices;

	// Find duplicate vertices, and create new list of vertices
	// together with map from old vertex number to new ones
	std::vector<ofIndexType> indexMap;

	for (ofIndexType i = 0; i < oldVerts.size(); i++) {
		// Get the next vert from the old list to test
		const vec3 v0 = oldVerts[i];
		bool foundDuplicate = false;

		// Compare with the positions of verts already in the new list
		for (ofIndexType j = 0; j < newVerts.size(); j++) {
			const vec3 v1 = newVerts[j];
			if (distance(v0, v1) < threshold) {
				// Map old vert i to the new vert within the threshold distance
				indexMap.push_back(j);
				foundDuplicate = true;
				break;
			}
		}

		// If we haven't found a duplicate, put this vert into the new list
		if (!foundDuplicate) {
			indexMap.push_back(newVerts.size());
			newVerts.push_back(v0);

			// Also add data from this vertex to the lists of
			// newColors, newTCoords and newNormals
			if (curMesh.hasColors()) {
				newColors.push_back(oldColors[i]);
			}

			if (curMesh.hasTexCoords()) {
				newTCoords.push_back(oldTCoords[i]);
			}

			if (curMesh.hasNormals()) {
				newNormals.push_back(oldNormals[i]);
			}
		}
	}

	// Use the map from old to new vertices to create a
	// new set of indices. Depending on the mesh mode
	// we need to do different processing to detect
	// degenerate lines or triangles

	std::cout << "Generating new index list\n";
	if (curMesh.getMode() == OF_PRIMITIVE_LINES) {
		// Case for line mesh
		std::cout << "curMesh.getMode() == OF_PRIMITIVE_LINES\n";
		int numLines = oldIndices.size() / 2;
		for (int i = 0; i < numLines; i++) {
			ofIndexType v0 = indexMap[oldIndices[2 * i]];
			ofIndexType v1 = indexMap[oldIndices[2 * i + 1]];
			if (v0 != v1) {
				newIndices.push_back(v0);
				newIndices.push_back(v1);
			}
		}
	}
	else if (curMesh.getMode() == OF_PRIMITIVE_TRIANGLES) {
		// case for triangle mesh
		std::cout << "curMesh.getMode() == OF_PRIMITIVE_TRIANGLES\n";
		int numTriangles = oldIndices.size() / 3;
		for (int i = 0; i < numTriangles; i++) {
			ofIndexType v0 = indexMap[oldIndices[3 * i]];
			ofIndexType v1 = indexMap[oldIndices[3 * i + 1]];
			ofIndexType v2 = indexMap[oldIndices[3 * i + 2]];
			if (v0 != v1 && v0 != v2 && v1 != v2) {
				newIndices.push_back(v0);
				newIndices.push_back(v1);
				newIndices.push_back(v2);
			}
		}
	}
	else {
		// Default case, just re-map all indices from original list
		std::cout << "curMesh.getMode() == OF_PRIMITIVE_POINTS\n";
		for (int i = 0; i < oldIndices.size(); i++) {
			newIndices.push_back(indexMap[oldIndices[i]]);
		}
	}

	// Use these to construct new lists for the colors, texture coordinates, normals
	// and indexes of lines or triangles

	std::cout << "Setting new vertex data\n";
	curMesh.clearVertices();
	curMesh.addVertices(newVerts);

	curMesh.clearIndices();
	curMesh.addIndices(newIndices);


	if (curMesh.hasColors()) {
		curMesh.clearColors();
		curMesh.addColors(newColors);
	}

	if (curMesh.hasTexCoords()) {
		curMesh.clearTexCoords();
		curMesh.addTexCoords(newTCoords);
	}

	if (curMesh.hasNormals()) {
		curMesh.clearNormals();
		curMesh.addNormals(newNormals);
	}

	std::cout << "Finished removeDuplicateVertices()\n";
}
