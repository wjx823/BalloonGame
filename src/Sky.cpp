/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#include "Sky.h"

Sky::Sky(void)
{
	globe = new MyMesh("globe");
	globe->CompileList();
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
}

void Sky::draw() {
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glScalef(7.0, 7.0, 7.0);
	glCallList(globe->getDispList());
	glPopMatrix();
}

void Sky::app(Airship *airship) {
	pos[0] = airship->coordinates[0];
	pos[1] = airship->coordinates[1];
	pos[2] = airship->coordinates[2];
}