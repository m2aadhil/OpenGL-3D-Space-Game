#pragma once

struct vec3
{
	float x;
	float y;
	float z;
};

struct vec2
{
	float x;
	float y;
};

vec3 addVector(vec3 vector1, vec3 vector2);

vec3 subVector(vec3 vector1, vec3 vector2);

vec3 scaleVector(vec3 vector, float size);

vec3 normalizeVec3(vec3 v);

vec3 crossVec3(vec3 u, vec3 v);

float getVec3Length(vec3 v);

float getVecDistance(vec3 v1, vec3 v2);