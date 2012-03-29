/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#include <stdio.h>
#include <GL/glut.h>
#include "OBJLoader.h"
#include "Scene.h"

Scene *s;

inline void reshape(int width,int height) { s->reshape(width, height); }

inline void render() { s->render(); }

inline void key(unsigned char in, int x, int y) { s->key(in, x, y); }

inline void keyUp(unsigned char in, int x, int y) { s->keyUp(in, x, y); }

inline void idle() { s->idle(); }

int main(int argc, char* argv[]) { 
	s = new Scene(argc, argv);

	glutDisplayFunc (render);
	glutReshapeFunc (reshape);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc (idle);

	glutMainLoop();
	return 0;	
}