#pragma once
#include <gl/freeglut.h>

struct triFace
{
	int vIndex[3];
	int vtIndex[3];
	int vnIndex[3];
};

struct model
{
	std::vector<triFace> triFaces;
	std::vector<vec3> objVertices;
	std::vector<vec3> objNormals;
	std::vector<vec2> objTextCord;
};

struct asset
{
	char* name;
	vec3 position;
	vec3 rotation;
	model model;
	GLuint texture;
	vec3 color;
};

model load_obj(const char* objPath);

GLuint get_texture(const char* texturePath);

void drawObj(model object, GLuint texture, vec3 color);