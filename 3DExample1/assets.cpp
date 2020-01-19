#include "vec.h"
#include <iostream>
#include <fstream> // file read
#include <sstream> //getkine
#include <gl/freeglut.h>
#include <vector>
#include "assets.h"
#include "../gltools.h"

//we need these for the texture loader
//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;

// this is a pointer to memory where the image bytes will be held 
GLbyte* pBytes0;

GLuint get_texture(const char* texturePath)
{
	GLuint texture;
	// allocate a texture name
	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Bind to next texture object
	glBindTexture(GL_TEXTURE_2D, texture);

	// Load texture data, set filter and wrap modes
	//note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
	pBytes0 = gltLoadTGA(texturePath, &iWidth, &iHeight,
		&iComponents, &eFormat);

	glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);

	//set up texture parameters

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//try these too
   // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
	// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	free(pBytes0);

	return texture;
};

//load obj
model load_obj(const char* objPath)
{
	FILE* file = fopen(objPath, "r");

	model object;

	while (1)
	{

		if (file == NULL) {
			std::cout << ("Impossible to open the file !\n");
			return object;
		}

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0)
		{
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			object.objVertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			vec2 textCo;
			fscanf(file, "%f %f\n", &textCo.x, &textCo.y);
			object.objTextCord.push_back(textCo);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			object.objNormals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			triFace triangleFace;
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &triangleFace.vIndex[0], &triangleFace.vtIndex[0], &triangleFace.vnIndex[0], &triangleFace.vIndex[1], &triangleFace.vtIndex[1], &triangleFace.vnIndex[1], &triangleFace.vIndex[2], &triangleFace.vtIndex[2], &triangleFace.vnIndex[2]);
			triangleFace.vIndex[0]--; triangleFace.vtIndex[0]--; triangleFace.vnIndex[0]--, triangleFace.vIndex[1]--, triangleFace.vtIndex[1]--, triangleFace.vnIndex[1]--, triangleFace.vIndex[2]--, triangleFace.vtIndex[2]--, triangleFace.vnIndex[2]--;
			object.triFaces.push_back(triangleFace);
		}
	}
	return object;
}

void drawObj(model object, GLuint texture, vec3 color)
{

	//GLfloat color[3] = { 1.0, 1.0, 1.0 };
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glEnable(GL_TEXTURE_2D);

	//bind the texture 
	glColor3f(color.x, color.y, color.z);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLES);
	for each (triFace face in object.triFaces)
	{
		glNormal3f(object.objNormals[face.vnIndex[0]].x, object.objNormals[face.vnIndex[0]].y, object.objNormals[face.vnIndex[0]].z);
		glTexCoord2f(object.objTextCord[face.vtIndex[0]].x, object.objTextCord[face.vtIndex[0]].y);
		glVertex3f(object.objVertices[face.vIndex[0]].x, object.objVertices[face.vIndex[0]].y, object.objVertices[face.vIndex[0]].z);

		glNormal3f(object.objNormals[face.vnIndex[1]].x, object.objNormals[face.vnIndex[1]].y, object.objNormals[face.vnIndex[1]].z);
		glTexCoord2f(object.objTextCord[face.vtIndex[1]].x, object.objTextCord[face.vtIndex[1]].y);
		glVertex3f(object.objVertices[face.vIndex[1]].x, object.objVertices[face.vIndex[1]].y, object.objVertices[face.vIndex[1]].z);

		glNormal3f(object.objNormals[face.vnIndex[2]].x, object.objNormals[face.vnIndex[2]].y, object.objNormals[face.vnIndex[2]].z);
		glTexCoord2f(object.objTextCord[face.vtIndex[2]].x, object.objTextCord[face.vtIndex[2]].y);
		glVertex3f(object.objVertices[face.vIndex[2]].x, object.objVertices[face.vIndex[2]].y, object.objVertices[face.vIndex[2]].z);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}