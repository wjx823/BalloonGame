/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#include "Treasure.h"


Treasure::Treasure(float x, float y, float z)
{
	//load model
	treasure = new MyMesh("treasure");
	treasure->CompileList();

	//set coordinates
	this->x = x;
	this->y = y;
	this->z = z;

	//initialize variables
	omega = 50;
}

void Treasure::draw(void)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(yRotation, 0.0,1.0,0.0);
	glCallList(treasure->getDispList());
	glPopMatrix();
}

float* Treasure::getPos() {
	float coords[3] = {x, y, z};
	return coords;
}

void Treasure::app(float dt) {
		float df = omega * dt;
		yRotation += df;
}