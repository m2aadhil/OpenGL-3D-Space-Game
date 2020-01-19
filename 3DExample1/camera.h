#pragma once
#include "vec.h"

struct cameraStruct
{
	vec3 position;
	vec3 viewDirection;
	vec3 upVector;
	vec3 rightVector;
	vec3 rotation;
};

extern cameraStruct camera;

void cameraInit(vec3 position, vec3 direction, vec3 upVetor, vec3 rightVector);

void cameraMove(vec3 direction);

void cameraMoveForward(float speed);

void cameraMoveBackward(float speed);

void cameraMoveRight(float speed);

void cameraMoveLeft(float speed);

void cameraMoveUp(float speed);

void cameraMoveDown(float speed);


void cameraRotateX(float angle);

void cameraRotateY(float angle);

void cameraRotateZ(float angle);