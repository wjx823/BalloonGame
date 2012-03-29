/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#include "Airship.h"

const float Airship::MAX_SHIFTSPEED=0.01f;
const float Airship::BULLETS_PER_SECOND=5.0;

Airship::Airship(float x, float y, float z)
{
	balloon = new MyMesh("airship");
	balloon->CompileList();

	gammaMax = 60000.0f;
	windResistance = 0.5f;
	velocity = upVelocity = shiftSpeed = yRotation = 0.0f;
	lastBulletCreationTime = 0;

	coordinates[0] = x;
	coordinates[1] = y;
	coordinates[2] = z;

	propeller = new Propeller(x, y, z);

	front[0] = 0.0f;
	front[1] = 0.0f;
	front[2] = -1.0f;

	up[0] = 0.0f;
	up[1] = 1.0f;
	up[2] = 0.0f;

	right[0] = 1.0f;
	right[1] = 0.0f;
	right[2] = 0.0f;

	gamma[0] = 0.0f;			//right - left speed
	gamma[1] = 0.0f;			//up - down speed
	gamma[2] = gammaMax;		//throttle (either gammaMax or 0)

	MAX_VELOCITY = 40.0f;

	//powerups
	speedUpTime = 0.0;
}

void Airship::draw(float tx, float ty, float tz) {
	float newc[3];
	newc[0] = tx - coordinates[0];
	newc[1] = ty - coordinates[1];
	newc[2] = tz - coordinates[2];
	float nmul = 1.0f / sqrtf(newc[0]*newc[0] + newc[1]*newc[1] +newc[2]*newc[2]);
	newc[0] *= nmul;
	newc[1] *= nmul;
	newc[2] *= nmul;

	glPushMatrix();
	glLineWidth(10);
	GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glMateriali(GL_FRONT, GL_SHININESS, 20);
	glColor3f(1.0f,0.0f,0.0f);          // Set The Color To Red
	glBegin(GL_LINES);
	glVertex3f(coordinates[0], coordinates[1], coordinates[2]);	 // origin of the line
	glVertex3f(coordinates[0] + 100.0*newc[0], coordinates[1] + 100.0*newc[1], coordinates[2] + 100.0*newc[2]);   // ending point of the line
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(coordinates[0], coordinates[1], coordinates[2]);
	glRotatef(yRotation, 0.0,1.0,0.0);
	glCallList(balloon->getDispList());
	glPopMatrix();

	propeller->draw();


	int size = bullets.size();
	for (int i = 0; i < size; i++) {
		bullets[i]->draw();
	}
}

void Airship::speedUp() {
	MAX_VELOCITY = 120.0f;
	speedUpTime += 20.0;
}

void Airship::app(float dt) { 
	
	//calculate new speed on each axis
	shiftSpeed  = (shiftSpeed + dt*gamma[0]) *  (1.0f - windResistance);
	velocity =  (velocity + gamma[2]*dt) *  (1.0f - windResistance);
	upVelocity =  (upVelocity + gamma[1]*dt) * (1.0f - windResistance);

	//Apply limits to linear and angular speed
	velocity = (velocity> MAX_VELOCITY )? MAX_VELOCITY: (velocity < 0 )? 0:velocity;
	shiftSpeed = (shiftSpeed> MAX_SHIFTSPEED )? MAX_SHIFTSPEED:(shiftSpeed < -MAX_SHIFTSPEED )? -MAX_SHIFTSPEED : shiftSpeed;

	//calculate new position
	coordinates[0] +=  dt*(front[0]*velocity + right[0]*shiftSpeed + up[0]*upVelocity);
	coordinates[1] +=  dt*(front[1]*velocity + right[1]*shiftSpeed + up[1]*upVelocity);
	coordinates[2] +=  dt*(front[2]*velocity + right[2]*shiftSpeed + up[2]*upVelocity);

	//front = front + shiftSpeed * right
	front[0] = (front[0] + shiftSpeed * right[0]);
	front[1] =  (front[1] + shiftSpeed * right[1]);
	front[2] =  (front[2] + shiftSpeed * right[2]);

	//normalize front vector
	float fmul = 1.0f / sqrtf(front[0]*front[0] + front[1]*front[1] +front[2]*front[2]);
	front[0] *= fmul;
	front[1] *= fmul;
	front[2] *= fmul;

	float parameter = - 1.0f*front[2];
	yRotation = (acos(parameter) * (180 / 3.14159265)); 

	if (front[0]>0) {
		if(front[2]>0)
			 yRotation = 180 +(180- (acos(parameter) * (180 / 3.14159265)));
		else
			yRotation = -(acos(parameter) * (180 / 3.14159265));
			 
	}else {
		yRotation =  (acos(parameter) * (180 / 3.14159265)); 
	}

	//right = front X up
	right[0] = (front[1] * up[2] - front[2] * up[1]);
	right[1] =  (front[2] * up[0] - front[0] * up[2]);
	right[2] =  (front[0] * up[1] - front[1] * up[0]);

	//normalize right vector
	float rmul = 1.0f / sqrtf(right[0]*right[0] + right[1]*right[1] + right[2]*right[2]);
	right[0] *= rmul;
	right[1] *= rmul;
	right[2] *= rmul;
	
	propeller->app(dt, coordinates[0], coordinates[1]-64.5, coordinates[2], yRotation, gamma[2]);

	removeOldBullets();

	int size = bullets.size();
	for (int i = 0; i < size; i++) {
		bullets[i]->app(dt);
	}

	//powerups
	if (speedUpTime >= 0) {
		speedUpTime -= dt;
	} else {
		MAX_VELOCITY = 40.0f;
	}
}

void Airship::addBullet() {
	long currentTime = clock();
	float dtBullet = (float) (currentTime - lastBulletCreationTime) / CLOCKS_PER_SEC;
	if( dtBullet > 1/BULLETS_PER_SECOND ){
		lastBulletCreationTime = currentTime;
		Bullet* bullet = new Bullet(coordinates[0] +(28.0* front[0]), coordinates[1] -58.2, coordinates[2]+ (28.0* front[2]), front,  false);
		bullets.push_back(bullet);
	}
}

void Airship::removeOldBullets() {
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

bool Airship::intersectsWithBullet(float x, float y, float z) {
	float dx = coordinates[0]-x, dy = coordinates[1]-y, dz = coordinates[2]-z;
	float difference = sqrtf(dx*dx + dy*dy + dz*dz);
	dy = coordinates[1]-57.0f -y;
	float difference2 = sqrtf(dx*dx + dy*dy + dz*dz);

	if(difference < 50 || difference2 < 40)
		return true;
	return false;
}

bool Airship::intersectsWith(float x, float y, float z){
	float dx = coordinates[0]-x, dy = coordinates[1]-y, dz = coordinates[2]-z;
	float difference = sqrtf(dx*dx + dy*dy + dz*dz);

	dy = coordinates[1]-57.0f -y;
	float difference2 = sqrtf(dx*dx + dy*dy + dz*dz);

	if(difference < 64 || difference2 < 44)
		return true;
	return false;
}