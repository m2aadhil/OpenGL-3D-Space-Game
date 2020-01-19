#include "vec.h"
#include <math.h>
#include "camera.h"

#define PI 3.14159265


cameraStruct camera;

float resetRotation(float rotation)
{
	if (rotation > 359) {
		return 0;
	}
	else if (rotation < 0) {
		return 359;
	}
	return rotation;
}

void cameraInit(vec3 position, vec3 direction, vec3 upVetor, vec3 rightVector)
{
	camera.position = position;
	camera.viewDirection = direction;
	camera.rightVector = rightVector;
	camera.upVector = upVetor;
	camera.rotation.x = camera.rotation.y = camera.rotation.z = 0.0;
};

void cameraMove(vec3 direction)
{
	camera.position = addVector(camera.position, direction);
};

void cameraMoveForward(float speed)
{
	camera.position = addVector(camera.position, scaleVector(camera.viewDirection, speed));
};

void cameraMoveBackward(float speed)
{
	camera.position = addVector(camera.position, scaleVector(camera.viewDirection, -speed));
};

void cameraMoveRight(float speed)
{
	camera.position = addVector(camera.position, scaleVector(camera.rightVector, speed));
};

void cameraMoveLeft(float speed)
{
	camera.position = addVector(camera.position, scaleVector(camera.rightVector, -speed));
};

void cameraMoveUp(float speed)
{
	camera.position = addVector(camera.position, scaleVector(camera.upVector, speed));
};

void cameraMoveDown(float speed)
{
	camera.position = addVector(camera.position, scaleVector(camera.upVector, -speed));
};

void cameraRotateX(float angle)
{;

	camera.rotation.x = resetRotation(camera.rotation.x += angle);

	//Rotate viewdir around the right vector:
	camera.viewDirection = normalizeVec3(addVector(scaleVector(camera.viewDirection , cos(angle * (PI/180))) , scaleVector(camera.upVector , sin(angle * (PI / 180)))));

	//now compute the new UpVector (by cross product)
	camera.upVector= scaleVector(crossVec3(camera.viewDirection, camera.rightVector) , -1);
};

void cameraRotateY(float angle)
{
	camera.rotation.y  = resetRotation(camera.rotation.y += angle);

	//Rotate viewdir around the up vector:
	camera.viewDirection = normalizeVec3(subVector(scaleVector(camera.viewDirection , cos(angle * (PI / 180))) , scaleVector(camera.rightVector , sin(angle * (PI / 180)))));

	//now compute the new RightVector (by cross product)
	camera.rightVector = crossVec3(camera.viewDirection, camera.upVector);

};

void cameraRotateZ(float angle)
{
	camera.rotation.z  = resetRotation(camera.rotation.z += angle);

	//Rotate viewdir around the right vector:
	camera.rightVector = normalizeVec3(addVector(scaleVector(camera.rightVector , cos(angle * (PI / 180))) , scaleVector(camera.upVector , sin(angle * (PI / 180)))));

	//now compute the new UpVector (by cross product)
	camera.upVector = scaleVector(crossVec3(camera.viewDirection, camera.rightVector) , -1);
};