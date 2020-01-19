#include <math.h>
#include "vec.h"

//#define NULL_VECTOR vec3{0.0f,0.0f,0.0f}

vec3 addVector(vec3 vector1, vec3 vector2)
{
	vec3 res;
	res.x = vector1.x + vector2.x;
	res.y = vector1.y + vector2.y;
	res.z = vector1.z + vector2.z;

	return res;
};

vec3 subVector(vec3 vector1, vec3 vector2)
{
	vec3 res;
	res.x = vector1.x - vector2.x;
	res.y = vector1.y - vector2.y;
	res.z = vector1.z - vector2.z;

	return res;
};

vec3 multiplyVector(vec3 vector1, vec3 vector2)
{
	vec3 res;
	res.x = vector1.x * vector2.x;
	res.y = vector1.y * vector2.y;
	res.z = vector1.z * vector2.z;
	return res;
};

vec3 scaleVector(vec3 vector, float size)
{
	vec3 res;
	res.x = vector.x * size;
	res.y = vector.y * size;
	res.z = vector.z * size;
	return res;
};

float getVecDistance(vec3 v1, vec3 v2)
{
	return (float)(sqrt((v2.x - v1.x)*(v2.x - v1.x) + (v2.y - v1.y)*(v2.y - v1.y) + (v2.z - v1.z)*(v2.z - v1.z)));
};

float getVec3Length(vec3 v)
{
	return (float)(sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)));
};

vec3 normalizeVec3(vec3 v)
{
	vec3 res;
	float l = getVec3Length(v);
	if (l == 0.0f) return vec3{ 0.0f,0.0f,0.0f };
	res.x = v.x / l;
	res.y = v.y / l;
	res.z = v.z / l;
	return res;
};

vec3 crossVec3(vec3 u, vec3 v)
{
	vec3 resVector;
	resVector.x = u.y * v.z - u.z * v.y;
	resVector.y = u.z * v.x - u.x * v.z;
	resVector.z = u.x * v.y - u.y * v.x;
	return resVector;
};
