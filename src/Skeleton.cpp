/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/ 

#include "Skeleton.h"

Skeleton::Skeleton(float x, float y, float z, float direction[], float d)
{
	//load model
	skeleton = new MyMesh("skeleton");
	skeleton->CompileList();

	//set coordinates
	this->x = x;
	this->y = y;
	this->z = z;

	//set direction
	this->dir[0] = direction[0];
	this->dir[1] = direction[1];
	this->dir[2] = direction[2];

	//initialize variables
	uspeed = 50;
	yRotation = 180;
	this->direction = true;
	dist = 0;
	max_dist = d;
}

void Skeleton::draw(void)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(yRotation, 0.0,1.0,0.0);
	glCallList(skeleton->getDispList());
	glPopMatrix();
}

void Skeleton::app(float dt) {
		float du = uspeed * dt;

		float dx = du * dir[0];
		float dy = du * dir[1];
		float dz = du * dir[2];

		if (direction && dist < max_dist) {
			x += dx;
			y += dy;
			z += dz;
			dist += du;
		} else if (!direction && dist > 0) {
			x -= dx;
			y -= dy;
			z -= dz;
			dist -= du;
		}

		if (dist >= max_dist || dist <= 0){
			direction = !direction;
		}

		//set Rotation
		int sign = (direction) ? 1 : -1;
		float parameter = - 1.0f * dir[2] * sign;
		yRotation = (acos(parameter) * (180 / 3.14159265)); 

		if (dir[0] * sign > 0){
			if(dir[2]* sign > 0)
				 yRotation = 180 +(180- (acos(parameter) * (180 / 3.14159265)));
			else
				yRotation = -(acos(parameter) * (180 / 3.14159265));
			 
		}else {
			yRotation =  (acos(parameter) * (180 / 3.14159265)); 
		}
}

bool Skeleton::intersectsWith(float x, float y, float z){
	float dx = this->x-x, dy = this->y-y, dz = this->z-z;
	float difference = sqrtf(dx*dx + dy*dy + dz*dz);
	if(difference < 65)
		return true;
	return false;
}