/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#include "Rock.h"

const float Rock::BULLETS_PER_SECOND=2.0;

Rock::Rock(float x, float y, float z)
{
	//load model
	rock = new MyMesh("rock");
	rock->CompileList();
	//set coordinates
	this->x = x;
	this->y = y;
	this->z = z;

	direction[0] = 0.0f;
	direction[1] = 0.0f;
	direction[2] = 0.0f;
	
	//initialize variables
	omega = 50;
	yRotation = 175;
	lastBulletCreationTime = 0;
	state = STATE_IDLE;
}

void Rock::draw(void)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(yRotation, 0.0,1.0,0.0);
	glCallList(rock->getDispList());
	glPopMatrix();

	int size = bullets.size();
	for (int i = 0; i < size; i++) {
		bullets[i]->draw();
	}
}

void Rock::app(float dt) {
	if (state == STATE_IDLE || state == STATE_FIRING) {
		float df = omega * dt;
		yRotation += df;  //rotate the rock

		//change the direction vector of the rock according to the new angle 
		float angle = (yRotation*3.1415926f )/180;
		direction[0] = -sin(angle+0.18);
		direction[2] = -cos(angle+0.18);
	}
	if( state == STATE_FIRING ) {
		addBullet();
	}
	removeOldBullets();

	int size = bullets.size();
	for (int i = 0; i < size; i++) {
		bullets[i]->app(dt);
	}
}

void Rock::addBullet() {
	long currentTime = clock();
	float dtBullet = (float) (currentTime - lastBulletCreationTime) / CLOCKS_PER_SEC;
	if( dtBullet > 1/BULLETS_PER_SECOND ){
		lastBulletCreationTime = currentTime;
		Bullet* bullet = new Bullet(x, y-17 , z, direction,  true);
		bullets.push_back(bullet);
	}
}

void Rock::removeOldBullets() {
	int size = bullets.size();
	long currentTime = clock();
	float dt;
	for (int i = 0; i < size; i++) {
		dt = (float)(currentTime - bullets[i]->creationTime) / CLOCKS_PER_SEC;
		if (dt > MAX_BULLET_LIFETIME ) {
			bullets.erase(bullets.begin()+(i--));
			size--;
		}
	}
}

bool Rock::intersectsWithBullet(float x, float y, float z){
	float dx = this->x-x, dy = this->y-y, dz = this->z-z;
	float difference = sqrtf(dx*dx + dy*dy + dz*dz);
	if(difference < 39){
		return true;
	}
	return false;
}

void Rock::intersectsWithBalloon(float x, float y, float z){
	float dx = this->x-x, dy = this->y-y, dz = this->z-z;
	float difference = sqrtf(dx*dx + dy*dy + dz*dz);
	if(difference < 220){
		state = STATE_FIRING;
	} else
		state = STATE_IDLE;
}

void Rock::confuse(){
	state = STATE_DEACTIVATED;
}