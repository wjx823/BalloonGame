/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#include "bonus.h"

Bonus::Bonus(float x, float y, float z) : PowerUp(x, y, z)
{
	type = 2;
}

void Bonus::draw(void)
{
	float greenColor[] = {0.0, 1.0, 0.0, 1.0};
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(yRotation, 1.0,0.0,1.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, greenColor);
	glutSolidTorus(10, 25, 100, 100);
	glPopMatrix();
}

void Bonus::app(float dt) {
	float df = omega * dt;
	yRotation += df;  //rotate the rock
}

bool Bonus::intersectsWithBalloon(float x, float y, float z){
	float dx = this->x-x, dy = this->y-y, dz = this->z-z;
	float difference = sqrtf(dx*dx + dy*dy + dz*dz);
	return (difference < 80);
}
