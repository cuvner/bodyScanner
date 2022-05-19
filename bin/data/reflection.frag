#version 150

#define PI 3.1415926538

out vec4 fragColor;

in vec2 v_texcoord;
in vec3 v_viewSpaceNormal;
in vec3 v_viewSpacePosition;

uniform sampler2D environmentMap;

// These are passed in from OF programmable renderer
uniform mat4 modelViewMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

vec4 calcReflection(in vec3 normal, in vec3 surfacePosition) {
	// Compute direction from eye to the surface position
	// Note: eye is at (0,0,0) in view space
	vec3 eyeDir = normalize(surfacePosition);

	// Compute reflection direction
	vec3 reflDir = reflect(eyeDir, normal);

	// Calculate matrix to convert from view space to world space
	mat4 viewToWorldMatrix = inverse(viewMatrix);

	// Convert refl dir from view space to world space
	vec4 temp = viewToWorldMatrix * vec4(reflDir, 0);
	vec3 worldSpaceReflDir = normalize(temp.xyz);

	// Calculate direction of reflection in polar coordinates
	float theta = atan(worldSpaceReflDir.z, worldSpaceReflDir.x);
	float phi = asin(worldSpaceReflDir.y);

	// Use polar coordinates to calculate the texture co-ordinates to lookup from the reflection map
	vec2 envLookupCoords = vec2(0.5 * theta / PI + 0.5, 0.5 - phi / PI);

	// Lookup the color from the reflection map
	return texture(environmentMap, envLookupCoords);
}

void main (void) {
	// Re-normalize v_viewSpaceNormal to avoid interpollation artefacts
	vec3 normal = normalize(v_viewSpaceNormal);

	vec4 reflectionColor = calcReflection(normal, v_viewSpacePosition);
	fragColor = reflectionColor;
}
