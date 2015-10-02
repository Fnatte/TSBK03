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

TextureData *dogFace, *blackFace;

float frand() {
 return rand() / ((float)RAND_MAX+1);
}

FPoint add(FPoint f, FPoint s) {
	return (FPoint){f.h + s.h, f.v + s.v};
}

FPoint division(FPoint p, float divisor) {
	return (FPoint){p.h / divisor, p.v / divisor};
}

FPoint scale(FPoint p, float factor) {
	return (FPoint){p.h * factor, p.v * factor};
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
	return sqrt(pow(first.h - second.h, 2) + pow(first.v - second.v, 2));
}

FPoint clamp(FPoint point, float min, float max) {
	float amp = amplitude(point);
	amp = (amp > max)? max : (amp < min)? min : amp;
	return  scale(normalize(point), amp);
}

FPoint squareClamp(FPoint point, float min, float max) {
	return (FPoint){(point.h > max)? max : (point.h < min)? min : point.h,
			(point.v > max)? max : (point.v < min)? min : point.v};
}

void SpriteBehavior(SpritePtr current) {
	float repelDistance = 70;
	float gravityDistance = 175;
	float alignDistance = 60;
	float dogDistance = 300;
	float gravityWeight = 0.06;
	float repelWeight = 1.0;
	float alignWeight = 0.25;
	float dogWeight = 1.0;

	SpritePtr other = gSpriteRoot;
	FPoint gravity = {0, 0};
	FPoint repelForce = {0, 0};
	FPoint meanDirection = {0, 0};
	FPoint dogForce = {0, 0};

	if (current->face == blackFace) {
		current->speed = add(current->speed, (FPoint){frand() - 0.5, frand() - 0.5});
	}
	if (current->face == dogFace) {
		current->speed = add(current->speed, scale((FPoint){frand() - 0.5, frand() - 0.5}, 0.05));
		current->speed = clamp(current->speed, 0.3, 0.7);
		return;
	}

	do {
		if (current == other) {
			other = other->next;
			continue;
		}
		float length = euclidDist(current->position, other->position);
		FPoint directionToOther = {other->position.h - current->position.h,
											 other->position.v - current->position.v};
		directionToOther = normalize(directionToOther);
		if (length < repelDistance) {
			repelForce.h -= directionToOther.h * ((repelDistance - length) / repelDistance);
			repelForce.v -= directionToOther.v * ((repelDistance - length) / repelDistance);
		}
		else if (length < gravityDistance) {
			gravity.h += directionToOther.h;
			gravity.v += directionToOther.v;
		}
		if (length < alignDistance) {
			FPoint normalizedSpeed = normalize(other->speed);
			meanDirection = add(normalizedSpeed, meanDirection);
		}
		if (other->face == dogFace) {
			if (length < dogDistance) {
			dogForce.h -= directionToOther.h * ((dogDistance - length) / dogDistance);
			dogForce.v -= directionToOther.v * ((dogDistance - length) / dogDistance);
			}
		}

		other = other->next;
	} while (other != NULL);
	meanDirection = add(normalize(current->speed), meanDirection);
	meanDirection = normalize(meanDirection);

	current->speed = add(current->speed, scale(dogForce, dogWeight));
	current->speed = add(current->speed, scale(repelForce, repelWeight));
	current->speed = add(current->speed, scale(meanDirection, alignWeight));
	current->speed = add(current->speed, scale(gravity, gravityWeight));
	current->speed = clamp(current->speed, 0.0, 5.0);

	if (0) {
		printf("repelForce: (%f, %f)\n", repelForce.h, repelForce.v);
		printf("meanDirection: (%f, %f)\n", meanDirection.h, meanDirection.v);
		printf("gravity: (%f, %f)\n", gravity.h, gravity.v);
	}
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
	TextureData *sheepFace, *foodFace;

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
	NewSprite(blackFace, 100, 200, 1, 1);
	NewSprite(dogFace, 100, 300, 1, 1);
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
