#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "TitleScreen.h"
#include <iostream>


//Remove console (only works in Visual Studio)
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


#define TIME_PER_FRAME 1000.f / 60.f // Approx. 60 fps


static int prevTime;
TitleScreen title;
bool GameStarted = false;
bool godMode = false;

static void drawCallback()
{
	Game::instance().render();
	glutSwapBuffers();
}

static void titol() {
	title.loadTitleScreen();
}


static void credits() {
	title.loadCredits();
}

static void instructions() {
	title.loadInstructions();
}


// If a key is pressed this callback is called

static void keyboardDownCallback(unsigned char key, int x, int y)
{
	if (key == 'p') {
		GameStarted = true;
		glFlush();
		Game::instance().init();
		glutDisplayFunc(drawCallback);
		glutPostRedisplay();
	}

	if (key == 'm' && !GameStarted) {
		glFlush();
		glutDisplayFunc(titol);
		glutPostRedisplay();
	}

	if (key == 'c' && !GameStarted) {
		glFlush();
		glutDisplayFunc(credits);
		glutPostRedisplay();
	}
	
	if (key == 'i' && !GameStarted) {
		glFlush();
		glutDisplayFunc(instructions);
		glutPostRedisplay();
	}

	if (key == 'g' && GameStarted) {
		godMode = !godMode;
		Game::instance().godMode(godMode);
		glutDisplayFunc(drawCallback);
		glutPostRedisplay();
	}

	if (key == 'f' && GameStarted) {
		Game::instance().forceMode();
		glutDisplayFunc(drawCallback);
		glutPostRedisplay();
	}

	if ((key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5') && GameStarted)
	{
		int keyNumber = key - '0';
		Game::instance().teleport(keyNumber);
		glutDisplayFunc(drawCallback);
		glutPostRedisplay();
	}

	Game::instance().keyPressed(key);
}

// If a key is released this callback is called

static void keyboardUpCallback(unsigned char key, int x, int y)
{
	Game::instance().keyReleased(key);
}

// If a special key is pressed this callback is called

static void specialDownCallback(int key, int x, int y)
{
	Game::instance().specialKeyPressed(key);
}

// If a special key is released this callback is called

static void specialUpCallback(int key, int x, int y)
{
	Game::instance().specialKeyReleased(key);
}

// Same for changes in mouse cursor position

static void motionCallback(int x, int y)
{
	Game::instance().mouseMove(x, y);
}

// Same for mouse button presses or releases

static void mouseCallback(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
		Game::instance().mousePress(button);
	else if(state == GLUT_UP)
		Game::instance().mouseRelease(button);
}







static void idleCallback()
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - prevTime;
	
	if(deltaTime > TIME_PER_FRAME)
	{
		// Every time we enter here is equivalent to a game loop execution
		if(!Game::instance().update(deltaTime))
			exit(0);
		prevTime = currentTime;
		glutPostRedisplay();
	}
}


int main(int argc, char **argv)
{
	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE  | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	
	glutCreateWindow(argv[0]);
	glutDisplayFunc(/*drawCallback*/titol);
	glutIdleFunc(idleCallback);
	glutKeyboardFunc(keyboardDownCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutSpecialFunc(specialDownCallback);
	glutSpecialUpFunc(specialUpCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);

	

	// GLEW will take care of OpenGL extension functions
	
	glewExperimental = GL_TRUE;
	glewInit();
	glutPostRedisplay();
	

	// Game instance initialization
	Game::instance().init();
	prevTime = glutGet(GLUT_ELAPSED_TIME);
	// GLUT gains control of the application
	glutMainLoop();

	return 0;
}



