// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include "MicroGlut.h"
// uses framework Cocoa
#else
#include <GL/gl.h>
#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"
#include <math.h>

// Lägg till egna globaler här efter behov.

FPoint division(FPoint p, float divisor) {
	return (FPoint){p.h / divisor, p.v / divisor};
}

FPoint multiplication(FPoint p, float factor) {
	return (FPoint){p.h * factor, p.v / factor};
}

float amplitude(FPoint p) {
	return sqrt(p.h * p.h + p.v * p.v);
}

FPoint normalize(FPoint p) {
	float length = sqrt(p.h * p.h + p.v * p.v);
	FPoint retVal;
	retVal.h = p.h / length;
	retVal.v = p.v / length;
	return retVal;
}

float euclidDist(FPoint first, FPoint second) {
	return sqrt(pow(first.h - second.h, 2) + sqrt(pow(first.v - second.v, 2)));
}

FPoint clamp(FPoint point, float max, float min) {
	return (FPoint){(point.h > max)? max : (point.h < min)? min : point.h,
			(point.v > max)? max : (point.v < min)? min : point.v};
}

void SpriteBehavior(SpritePtr current) {
	float minDistance = 15;
	// Lägg till din labbkod här. Det går bra att ändra var som helst i
	// koden i övrigt, men mycket kan samlas här. Du kan utgå från den
	// globala listroten, gSpriteRoot, för att kontrollera alla sprites
	// hastigheter och positioner, eller arbeta från egna globaler.
	SpritePtr other = gSpriteRoot;
	FPoint force = {0, 0};
	do {
		if (current == other) {
			other = other->next;
			continue;
		}
		float length = euclidDist(current->position, other->position);
		FPoint direction = {other->position.h - current->position.h,
											 other->position.v - current->position.v};
		direction = normalize(direction);
		if (length < minDistance) {
			force.h -= direction.h * (minDistance - length);
			force.v -= direction.v * (minDistance - length);
		}
		else if (length < 100) {
			force.h += direction.h;
			force.v += direction.v;
		}

		other = other->next;
	} while (other != NULL);

	force = division(force, 10);

	current->speed.h += force.h;
	current->speed.v += force.v;
	current->speed = clamp(current->speed, 5.0, -5.0);
}

// Drawing routine
void Display() {
	SpritePtr sp;

	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawBackground();

	// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do {
		SpriteBehavior(sp); // Din kod!
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);

	glutSwapBuffers();
}

void Reshape(int h, int v) {
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value) {
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
		{
    case '+':
    	someValue += 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case '-':
    	someValue -= 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case 0x1b:
      exit(0);
		}
}

void Init() {
	TextureData *sheepFace, *blackFace, *dogFace, *foodFace;

	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund

	sheepFace = GetFace("bilder/sheep.tga"); // Ett får
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart får
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat

	for(int n = 0; n < 10; n++) {
		NewSprite(sheepFace,
			rand() % 700 + 50,
			rand() % 500 + 50,
			((rand() % 200) - 100) / 100.0,
			((rand() % 200) - 100) / 100.0
		);
	}

	// NewSprite(sheepFace, 100, 200, 1, 1);
	// NewSprite(sheepFace, 200, 100, 1.5, -1);
	// NewSprite(sheepFace, 250, 200, -1, 1.5);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");

	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);

	InitSpriteLight();
	Init();

	glutMainLoop();
	return 0;
}
