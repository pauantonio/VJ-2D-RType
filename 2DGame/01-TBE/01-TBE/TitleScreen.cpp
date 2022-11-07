#include "TitleScreen.h"
#include <GL/glew.h>
#include <string.h>
#include <GL/freeglut_std.h>
TitleScreen::TitleScreen()
{
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::loadTitleScreen() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	glColor3f(1, 1, 1);
	glRasterPos2d(-0.15,0.5);
	char Title[] = "TYPE-R CONCEPT";
	for (int i = 0; i < strlen(Title); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, Title[i]);
	}

	glRasterPos2d(-0.2,0);
	char Play[] = "Press [P] to play";
	for (int i = 0; i < strlen(Play); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, Play[i]);
	}

	glRasterPos2d(-0.35, -0.1);
	char Instructions[] = "Press [I] to read instructions";
	for (int i = 0; i < strlen(Instructions); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, Instructions[i]);
	}

	glRasterPos2d(-0.3, -0.2);
	char Credit[] = "Press [C] for Credit scene";
	for (int i = 0; i < strlen(Credit); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, Credit[i]);
	}

	glutSwapBuffers();
}

void TitleScreen::loadCredits() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	glColor3f(1, 1, 1);
	glRasterPos2d(-0.15, 0.5);
	char Credit[] = "CREDITS";
	for (int i = 0; i < strlen(Credit); i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, Credit[i]);
	}
	glRasterPos2d(-0.3, 0);
	char MadeBy[] = "This game was made by:";
	for (int i = 0; i < strlen(MadeBy); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, MadeBy[i]);
	}
	glRasterPos2d(-0.3, -0.1);
	char LV[] = "Lluis Vacares Jaimez";
	for (int i = 0; i < strlen(LV); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, LV[i]);
	}
	glRasterPos2d(-0.3, -0.2);
	char PA[] = "Pau Antonio Soler";
	for (int i = 0; i < strlen(PA); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, PA[i]);
	}
	glRasterPos2d(-1, -0.5);
	char Play[] = "[P] to Play";
	for (int i = 0; i < strlen(Play); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, Play[i]);
	}

	glRasterPos2d(-1, -0.6);
	char Instructions[] = "[I] to Instructions";
	for (int i = 0; i < strlen(Instructions); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, Instructions[i]);
	}

	glRasterPos2d(-1, -0.7);
	char Menu[] = "[M] to Menu";
	for (int i = 0; i < strlen(Menu); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, Menu[i]);
	}


	glutSwapBuffers();

}

void TitleScreen::loadInstructions() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	glColor3f(1, 1, 1);
	glRasterPos2d(-0.15, 0.5);
	char Title[] = "INSTRUCTIONS";
	for (int i = 0; i < strlen(Title); i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, Title[i]);
	}

	glRasterPos2d(-0.95, 0.3);
	char ln1[] = "The objective of the game is to kill as many enemies as you can";
	for (int i = 0; i < strlen(ln1); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ln1[i]);
	}
	glRasterPos2d(-0.95, 0.2);
	char ln2[] = "without getting hurt from enemy attacks.";
	for (int i = 0; i < strlen(ln2); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ln2[i]);
	}

	glRasterPos2d(-0.95, 0.1);
	char lnx[] = "TRY TO SURVIVE AND DEFEAT THE F I N A L  B O S S ! ! !";
	for (int i = 0; i < strlen(lnx); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, lnx[i]);
	}


	glRasterPos2d(-0.1, 0.0);
	char ln3[] = "CONTROLS";
	for (int i = 0; i < strlen(ln3); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ln3[i]);
	}

	glRasterPos2d(-0.1, -0.1);
	char ln4[] = "[W] GO UP";
	for (int i = 0; i < strlen(ln4); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ln4[i]);
	}
	glRasterPos2d(-0.1, -0.2);
	char ln5[] = "[A] GO LEFT";
	for (int i = 0; i < strlen(ln5); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ln5[i]);
	}

	glRasterPos2d(-0.1, -0.3);
	char ln6[] = "[S] GO DOWN";
	for (int i = 0; i < strlen(ln6); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ln6[i]);
	}

	glRasterPos2d(-0.1, -0.4);
	char ln7[] = "[D] GO RIGHT";
	for (int i = 0; i < strlen(ln7); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ln7[i]);
	}

	glRasterPos2d(-0.1, -0.5);
	char ln8[] = "[MAINTAIN E AND RELEASE] CHARGED SHOT";
	for (int i = 0; i < strlen(ln8); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ln8[i]);
	}

	glRasterPos2d(-0.1, -0.6);
	char ln9[] = "[SPACE BAR] SHOOT";
	for (int i = 0; i < strlen(ln9); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ln9[i]);
	}
	glRasterPos2d(-0.1, -0.7);
	char ln10[] = "[F] ACTIVATE FORCE CHEATS";
	for (int i = 0; i < strlen(ln10); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ln10[i]);
	}
	glRasterPos2d(-0.1, -0.8);
	char ln11[] = "[G] ACTIVATE INVINCIBILITY CHEATS";
	for (int i = 0; i < strlen(ln11); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ln11[i]);
	}

	glRasterPos2d(-0.1, -0.9);
	char ln12[] = "[0-5] TELEPORT ACRROSS THE LEVEL";
	for (int i = 0; i < strlen(ln12); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ln12[i]);
	}


	glRasterPos2d(-1, -0.5);
	char Play[] = "[P] to Play";
	for (int i = 0; i < strlen(Play); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, Play[i]);
	}

	glRasterPos2d(-1, -0.6);
	char Credits[] = "[C] to Credits";
	for (int i = 0; i < strlen(Credits); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, Credits[i]);
	}

	glRasterPos2d(-1, -0.7);
	char Menu[] = "[M] to Menu";
	for (int i = 0; i < strlen(Menu); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, Menu[i]);
	}

	glutSwapBuffers();

}