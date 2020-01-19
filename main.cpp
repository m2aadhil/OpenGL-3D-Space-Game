///w1673791 - M. M. Aadhil
//Coursework 2 - Space Game


#include <math.h>						
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	//Needed for console output (debugging)
#include <gl/freeglut.h>
#include <vector>
#include <iostream>
#include <thread>

#ifdef WIN32
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
//#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
//#include <gl\glu.h>			// OpenGL Utilities
//#include "glm.h"

#endif

//refernce files
#include "3DExample1\\camera.h"
#include "3DExample1\\vec.h"
#include "3DExample1\\assets.h"
#include <sstream>


#define OBJPATH = "3DExample1\\objects\\";
#define TEXTUREPATH = "3DExample1\\textures\\";

int screenW = 800;
int screenH = 600;

bool isLoading = true;
bool done = false;
asset orbit;
asset world;
asset sun;
asset diamond;
asset cylinder;
asset loop;

GLfloat  sunLightBright[] = { 253.0f, 184.0f, 19.0f, 1.0f };
GLfloat  redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  whiteLightLessBright[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat  greenLight[] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat	 lightPos[] = { 4000.0f, 4000.0f, -4000.0f, 100.0f };
GLfloat	 greenLightPos[] = { 0.0f, 1000.0f, -4000.0f, 100.0f };

//materials for the new lights
GLfloat mat_specular[] = { 4.0, 4.0, 4.0, 4.0 }; //material specular value
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //specular value for lights

vec3 viewPoint;
vec3 color = { 0.8, 0.8, 0.8 };
vec3 dcolor = { 0.8, 0.0, 0.0 };
GLuint hudTexture;
bool hudWarn = false;

int score = 0;

std::string loadingStage = "Loading";
float shipSpeed = 10.0f;
FLOAT initSpeed = 70;
float elapsedTime = 0.0f;

struct hoop
{
	vec3 position;
	vec3 color;
	float rotationY;
};

struct keypress
{
	bool W = false;
	bool S = false;
	bool A = false;
	bool D = false;
	bool Z = false;
	bool X = false;
	bool Q = false;
	bool E = false;
	bool UP = false;
	bool DOWN = false;
	bool LEFT = false;
	bool RIGHT = false;
	bool SHIFT = false;
};

keypress keysPressed;

std::vector<hoop> hoops;
bool isHud = false;
vec3 startPosition;

//get random number betweeen max min
int getRandomNumber(int max, int min)
{
	//not truly random prduce biased results
	return (rand() % (max + 1 - min)) + min;
}

//write texts
void drawText(std::string text, void* font, vec3 color, int x, int y)
{
	glColor3f(color.x, color.y, color.z);
	glPushMatrix();
	glRasterPos2i(x, y);
	for (std::string::iterator i = text.begin(); i != text.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
	glPopMatrix();
}

//reset world
void reset()
{

	score = 0;
	camera.position = startPosition;
	hoop hp;
	hoops.clear();
	for (size_t i = 8000; i > 5000; i -= 500)
	{
		hp.position = vec3{ (float)getRandomNumber(500, -500), (float)getRandomNumber(500, -500), (float)getRandomNumber(-i, -i + 500) };
		hp.color = vec3{ 1.0, 1.0, 0.0 };
		hp.rotationY = getRandomNumber(359, 0);
		hoops.push_back(hp);
	}

	hoops[hoops.size() - 1].color = vec3{ 0.0, 1.0, 0.0 };
}

//loading orbits. hoops models and textures
void loadAssets()
{
	loadingStage = "Loading World...";
	world.model = sun.model = orbit.model = load_obj(".\\objects\\spear.obj");
	orbit.position.x = orbit.position.y = 0.0; orbit.position.z = -20.0;
	orbit.rotation.y = orbit.rotation.z = 0.0; orbit.rotation.x = 20.0;
	orbit.color = {0.1, 0.1, 0.1};
	loop.color = {1.0, 0.0, 0.0};
	reset();
	world.position.x = world.position.y = world.position.z = 0.0;

	sun.position.x = sun.position.y = sun.position.z = 0.0;
	loadingStage = "Loading Hoops...";
	loop.model = load_obj(".\\objects\\hoop.obj");
	loadingStage = "Loading Diamonds...";
	diamond.model = load_obj(".\\objects\\crystal.obj");
	camera.position.z = 5000;
	loadingStage = "Loading Done...";
	done = true;

}

//camera porcessing
void processCamera()
{
	vec3 viewPoint = addVector(camera.position, camera.viewDirection);
	gluLookAt(camera.position.x, camera.position.y, camera.position.z,//eye
		viewPoint.x, viewPoint.y, viewPoint.z,//centre
		camera.upVector.x, camera.upVector.y, camera.upVector.z);
};

//draw on screen text
void drawScreenText()
{
	drawText(loadingStage, GLUT_BITMAP_9_BY_15, vec3{ 1.0, 1.0, 1.0 }, 100, screenH - 100);
	drawText("Crystals Collected : " + std::to_string(score), GLUT_BITMAP_TIMES_ROMAN_24, vec3{ 0.3, 0.6, 71.516 }, 50, 50);
	drawText("Ship Position X : " + std::to_string(camera.position.x), GLUT_BITMAP_HELVETICA_12, vec3{ 1.0, .0, 0.0 }, 50, 100);
	drawText("Ship Position Y : " + std::to_string(camera.position.y), GLUT_BITMAP_HELVETICA_12, vec3{ 1.0, 1.0, 0.0 }, 50, 125);
	drawText("Ship Position Z : " + std::to_string(camera.position.z), GLUT_BITMAP_HELVETICA_12, vec3{ 0.0, 1.0, 1.0 }, 50, 150);
	drawText("Rotation X : " + std::to_string(camera.rotation.x), GLUT_BITMAP_HELVETICA_12, vec3{ 1.0, 1.0, 1.0 }, 50, 200);
	drawText("Rotation Y : " + std::to_string(camera.rotation.y), GLUT_BITMAP_HELVETICA_12, vec3{ 1.0, 1.0, 1.0 }, 50, 225);
	drawText("Rotation Z : " + std::to_string(camera.rotation.z), GLUT_BITMAP_HELVETICA_12, vec3{ 1.0, 1.0, 1.0 }, 50, 250);
}

//draw hud
void Hud()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	glColor3f(0.8,0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, hudTexture);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(0.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(screenW, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(screenW, screenH);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(0.0, screenH);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);

};

//display method
void RenderScene(void)
{


	// Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	processCamera();

	if (isLoading && !done) {
		glPushMatrix();
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glScalef(100, 50000, 100);
		drawObj(cylinder.model, cylinder.texture, color);
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		//rotate the quad slightly
		glScalef(10000, 10000, 10000);
		drawObj(world.model, world.texture, color);
		glPopMatrix();


		glPushMatrix();
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glTranslatef(2000.0, 1000.0, -4000.0);
		glScalef(100, 100, 100);
		drawObj(sun.model, sun.texture, color);
		glPopMatrix();
		glPushMatrix();
		glPushMatrix();

		//rotate and then translate the quad 
		//moon
		glTranslatef(-2000.0, 0.0, -9000.0);
		glRotatef(orbit.rotation.y + elapsedTime*25, 0.0, 1.0, 0.0);
		glTranslatef(-250.0, 0.0, 0.0);
		glRotatef(orbit.rotation.x, 1.0, 0.0, 0.0);
		glRotatef(orbit.rotation.y, 0.0, 1.0, 0.0);
		glScalef(10, 10, 10);
		drawObj(orbit.model, sun.texture, orbit.color);
		glPopMatrix();

		//juoiter
		glTranslatef(-2000.0, 0.0, -9000.0);
		glRotatef(orbit.rotation.x, 1.0, 0.0, 0.0);
		glRotatef(elapsedTime*4, 0.0, 1.0, 0.0);
		glScalef(200, 200, 200);
		drawObj(orbit.model, orbit.texture, orbit.color);
		glPopMatrix();

		for each (hoop var in hoops)
		{

			glPushMatrix();
			glTranslatef(var.position.x, var.position.y, var.position.z);
			glRotatef(var.rotationY + (elapsedTime  * 10), 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glScalef(100, 100, 100);
			drawObj(loop.model, loop.texture, loop.color);
			glPopMatrix();

			glPushMatrix();
			glLightfv(GL_LIGHT1, GL_POSITION, greenLightPos);
			glTranslatef(var.position.x, var.position.y, var.position.z);
			glRotatef(orbit.rotation.y - (elapsedTime  * 20), 0.0, 1.0, 0.0);
			glScalef(50, 50, 50);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
			drawObj(diamond.model, diamond.texture, var.color);
			glDisable(GL_BLEND);
			glPopMatrix();
		}
	}


	//2d Projection - Start
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, screenW, screenH, 0.0, 0.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);

	drawScreenText();
	if (isHud) {
		Hud();
	}

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	//2d Projection - End
};

//check wether reached a crystal
void collisionOnCrystal(vec3 pos) {

	int activeHoop = hoops.size() - (score + 1);
	if (hoops.size() > activeHoop) {
		float distance = getVecDistance(hoops[activeHoop].position, pos);


		if (distance < 100)
		{
			hoops[activeHoop].color = vec3{ 1.0, 1.0, 0.0 };
			score++;
			activeHoop = hoops.size() - (score + 1);
			if (hoops.size() > activeHoop){
				hoops[activeHoop].color = vec3{ 0.0, 1.0, 0.0 };
			}
			else 
			{
				reset();
			}
		}
	}
}

//check reaching world end
bool restirictReachingEndofSpace(vec3 pos) {

	float distance = getVecDistance(world.position, pos);

	if (distance > 8500)
	{
		return true;
	}
	return false;
}

// This function does any needed initialization on the rendering
// context.
void setup()
{
	//textures
	vec3 position; position.x = position.y = 0.0; position.z = 40000.0;
	vec3 viewDirection; viewDirection.x = viewDirection.y = 0.0; viewDirection.z = -1.0;
	vec3 upVector; upVector.x = upVector.z = 0.0; upVector.y = 1.0;
	vec3 rightVector;  rightVector.z = rightVector.y = 0.0; rightVector.x = 1.0;
	cameraInit(position, viewDirection, upVector, rightVector);

	//char* modelPath = ;
	cylinder.model = load_obj(".\\objects\\cylinder.obj");
	cylinder.texture = world.texture = get_texture(".\\textures\\starField.tga");
	orbit.texture = get_texture(".\\textures\\jupiterC.tga");
	sun.texture = get_texture(".\\textures\\sun.tga");
	diamond.texture = get_texture(".\\textures\\diamond.tga");
	hudTexture = get_texture(".\\textures\\hud.tga");
	loop.texture = get_texture(".\\textures\\hoop.tga");
	//enable textures
	//glEnable(GL_TEXTURE_2D);


	glEnable(GL_DEPTH_TEST);	// Hidden surface removal    
	glFrontFace(GL_CCW);// Counter clock-wise polygons face out
	////glEnable(GL_CULL_FACE);		// Do not calculate inside

    glCullFace(GL_FRONT_AND_BACK);

	// Enable lighting
 	glEnable(GL_LIGHTING);
	//glEnable(GL_POINT_SMOOTH);
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, sunLightBright);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, redLight);
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);


	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, greenLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, greenLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT1);

	// Enable colour tracking
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	// Black blue background
	glClearColor(0.0f, 0.0f, 0.03f, 1.0f);

};

void reshape(int w, int h)
{
	GLfloat fAspect;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);
	screenW = w; screenH = h;
	// Calculate aspect ratio of the window
	fAspect = (GLfloat)w / (GLfloat)h;

	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// field of view of 45 degrees, near and far planes 1.0 and 1000
	//that znear and zfar should typically have a ratio of 1000:1 to make sorting out z depth easier for the GPU
	gluPerspective(55.0f, fAspect, 1.0, 100000.0); //may need to make larger depending on project
	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);
	//pull the eye position back to view the scene
	gluLookAt(0.00, 0.00, 0.0,//eye
		0.00, 0.00, -1.00,//centre
		0.00, 1.00, 0.00);//up
};


//processing key board inputs
void processKeys()
{
	vec3 prePos = camera.position;
	if (keysPressed.W)
	{
		cameraMoveForward(shipSpeed);
	};
	if (keysPressed.S)
	{
		cameraMoveBackward(shipSpeed);
	};
	if (keysPressed.A)
	{
		cameraMoveLeft(shipSpeed);
	};
	if (keysPressed.D)
	{
		cameraMoveRight(shipSpeed);
	};
	if (keysPressed.Z)
	{
		cameraRotateY(1.0);
	};
	if (keysPressed.X)
	{
		cameraRotateY(-1.0);
	};
	if (keysPressed.UP)
	{
		cameraMoveUp(shipSpeed);
	};
	if (keysPressed.DOWN)
	{
		cameraMoveDown(shipSpeed);
	};
	if (keysPressed.LEFT)
	{
		cameraRotateZ(1.0);
	};
	if (keysPressed.RIGHT)
	{
		cameraRotateZ(-1.0);
	};
	if (keysPressed.Q) {
		cameraRotateX(1.0);
	}
	if (keysPressed.E) {
		cameraRotateX(-1.0);
	}

	if (restirictReachingEndofSpace(camera.position)) {
		camera.position = prePos;
	}
	collisionOnCrystal(camera.position);
}

void keyUpSpecial(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: keysPressed.UP = false; break;
	case GLUT_KEY_DOWN: keysPressed.DOWN = false; break;
	case GLUT_KEY_LEFT: keysPressed.LEFT = false; break;
	case GLUT_KEY_RIGHT: keysPressed.RIGHT = false; break;
	case GLUT_KEY_SHIFT_L: keysPressed.SHIFT = false; break;
	}
};

void keyUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w': case 'W':
		keysPressed.W = false;
		break;
	case 's': case 'S':
		keysPressed.S = false;
		break;
	case 'a': case 'A':
		keysPressed.A = false;
		break;
	case 'd': case 'D':
		keysPressed.D = false;
		break;
	case 'z': case 'Z':
		keysPressed.Z = false;
		break;
	case 'x': case 'X':
		keysPressed.X = false;
		break;
	case 'q': case 'Q':
		keysPressed.Q = false;
		break;
	case 'e': case 'E':
		keysPressed.E = false;
		break;
	case 'h': case 'H':
		isHud = !isHud;
		break;
	case 'r': case 'R':
		reset();
		break;
	}
}

void keyboardSpecialKeyInput(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: keysPressed.UP = true; break;
	case GLUT_KEY_DOWN: keysPressed.DOWN = true; break;
	case GLUT_KEY_LEFT: keysPressed.LEFT = true; break;
	case GLUT_KEY_RIGHT: keysPressed.RIGHT = true; break;
	case GLUT_KEY_SHIFT_L: keysPressed.SHIFT = true; break;
	}
};

void keyboardInput(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w': case 'W':
		keysPressed.W = true;
		break;
	case 's': case 'S':
		keysPressed.S = true;
		break;
	case 'a': case 'A':
		keysPressed.A = true;
		break;
	case 'd': case 'D':
		keysPressed.D = true;
		break;
	case 'z': case 'Z':
		keysPressed.Z = true;
		break;
	case 'x': case 'X':
		keysPressed.X = true;
		break;
	case 'q': case 'Q':
		keysPressed.Q = true;
		break;
	case 'e': case 'E':
		keysPressed.E = true;
		break;
	}
	
};

//animations
void TimerFunc(int value)
{
	if (isLoading) //loading animation
	{
		camera.position.z -= initSpeed;

		if (camera.position.z < -35000)
		{
			camera.position.z = 50000;
		}

		if (done) {
			initSpeed -= 0.3;
			if (initSpeed < 1)
			{
				startPosition = camera.position;
				isLoading = false;
				loadingStage = "";
			}
		}
	}
	//timer function
	elapsedTime += 0.25;
	processKeys();
	glutSwapBuffers();
	glutPostRedisplay();
	glutTimerFunc(25, TimerFunc, 1);
};

void loadInstructions()
{
	std::cout << "--- WELCOME - 3D Space Game --- \n" << std::endl;
	std::cout << "When all hoops have been collceted game will reset \n" << std::endl;
	std::cout << "Controls : \n" << std::endl;
	std::cout << "Left Arrow = Roll the Ship Left" << std::endl;
	std::cout << "Right Arrow = Roll the Ship Right" << std::endl;
	std::cout << "Up Arrow = Move the Ship Up" << std::endl;
	std::cout << "Down Arrow = Move the Ship Down" << std::endl;
	std::cout << "Q = Rotate the Ship Backwards" << std::endl;
	std::cout << "E = Rotate the Ship Forward" << std::endl;
	std::cout << "W = Move the Ship Forward" << std::endl;
	std::cout << "S = Move the Ship Backward" << std::endl;
	std::cout << "A = Move the Ship Left" << std::endl;
	std::cout << "D = Move the Ship Right" << std::endl;
	std::cout << "Z = Rotate Ship Left" << std::endl;
	std::cout << "X = Rotate Ship Right" << std::endl;
	std::cout << "H = Turn Hud On/ Off" << std::endl;
	std::cout << "R = Reset Game" << std::endl;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	loadInstructions();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenW, screenH);// a 4:3 ratio
	glutCreateWindow("3D Space Game");
	glutSpecialFunc(keyboardSpecialKeyInput);
	glutSpecialUpFunc(keyUpSpecial);
	glutKeyboardFunc(keyboardInput);
	glutKeyboardUpFunc(keyUp);
	glutReshapeFunc(reshape);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(25, TimerFunc, 1);
	//glutTimerFunc(150, TimerFunc2, 1);
	setup();
	std::thread t1(loadAssets); //loading assets on a seperate thread
	glutMainLoop();
	return 0;
}





