/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#include "Bullet.h"

const GLfloat Bullet::bulletColor[] = {0.0, 1.0, 0.0, 1.0};
const GLfloat Bullet::redBulletColor[] = {1.0, 0.0, 0.0, 1.0};

Bullet::Bullet(float x, float y, float z, float direction[], bool redColor)
{
	//set coordinates
	this->x = x;
	this->y = y;
	this->z = z;
	
	this->redColor = redColor;

	for(int i=0; i< 3; i++){
		this->direction[i] = direction[i];
	}
	
	//initialize variables
	uspeed = 200;
	creationTime = clock();
}


Bullet::~Bullet(void)
{
}

void Bullet::draw(void)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	
	if(redColor) 
		glMaterialfv(GL_FRONT, GL_DIFFUSE, redBulletColor);
	else
		glMaterialfv(GL_FRONT, GL_DIFFUSE, bulletColor);
	glMateriali(GL_FRONT, GL_SHININESS, 20);
	glutSolidSphere (2.4, 20, 16);

	glPopMatrix();
}

void Bullet::app(float dt) {
	float du = uspeed * dt;
	x += du * direction[0];
	y += du * direction[1];
	z += du * direction[2];
}