/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#pragma once
#include "Node.h"
#include <math.h>
#include "Bullet.h"
#include "Propeller.h"
#include <time.h>

class Airship :
	public Node
{
private:
	const static float  MAX_SHIFTSPEED;

	MyMesh *balloon;
	Propeller *propeller;
public:
	//Bullet stuff
	const static float BULLETS_PER_SECOND;
	void removeOldBullets();
	const static int MAX_BULLET_LIFETIME = 5;
	vector<Bullet*> bullets;
	long lastBulletCreationTime;

	Airship(float x, float y, float z);
	~Airship(void) { bullets.clear(); }
	void draw(float, float, float);
	void app(float dt);
	void addBullet();
	bool intersectsWithBullet(float x, float y, float z);
	bool intersectsWith(float x, float y, float z);

	float front[3], right[3], up[3], coordinates[3], gamma[3];
	float gammaMax, windResistance, shiftSpeed, velocity;
	float upVelocity, yRotation;

	//movement methods
	inline void moveUp() { gamma[1] = 3700; }
	inline void moveDown() { gamma[1] = -3700; }
	inline void moveFront() { gamma[2] = gammaMax; }
	inline void moveBack() { gamma[2] = 0; }
	inline void moveLeft() { gamma[0] = -1; gamma[2] = gammaMax; }
	inline void moveRight() { gamma[0] = 1;	gamma[2] = gammaMax; }
	inline void clearXMovement() { gamma[0] = 0; }
	inline void clearYMovement() { gamma[1] = 0; }
	inline void clearZMovement() { gamma[2] = 0; }
	inline void clearAllMovement() { gamma[0] = gamma[1] = gamma[2] = 0; }

	float MAX_VELOCITY;

	//powerups
	void speedUp();
	float speedUpTime;
};


