/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#include "powerup.h"


PowerUp::PowerUp(float x, float y, float z)
{
	//set coordinates
	this->x = x;
	this->y = y;
	this->z = z;
	//init vars
	omega = 50;
	yRotation = 175;
	//type
	type = 1;
}

void PowerUp::draw(void)
{
	float redColor[] = {1.0, 0.0, 0.0, 1.0};
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(yRotation, 0.0,1.0,1.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, redColor);
	glutSolidTorus(10, 25, 100, 100);
	glPopMatrix();
}

void PowerUp::app(float dt) {
	float df = omega * dt;
	yRotation += df;  //rotate the rock
}

bool PowerUp::intersectsWithBalloon(float x, float y, float z){
	float dx = this->x-x, dy = this->y-y, dz = this->z-z;
	float difference = sqrtf(dx*dx + dy*dy + dz*dz);
	return (difference < 80);
}
