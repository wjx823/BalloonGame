/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#include "Propeller.h"

Propeller::Propeller(float x, float y, float z)
{
	//load model
	propeller = new MyMesh("propeller");
	propeller->CompileList();

	//set coordinates
	this->x = x;
	this->y = y;
	this->z = z;

	//initialize variables
	yRotation = zRotation = 0;
}

void Propeller::draw(void)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(yRotation, 0.0, 1.0, 0.0);
	glRotatef(zRotation, 0.0, 0.0, 1.0);
	glCallList(propeller->getDispList());
	glPopMatrix();
}

void Propeller::app(float dt, float x, float y, float z, float yRotation, float speed) {
		float df = speed * dt;
		//set coordinates
		this->x = x;
		this->y = y;
		this->z = z;
		this->zRotation -= df ;
		this->yRotation = yRotation;
}