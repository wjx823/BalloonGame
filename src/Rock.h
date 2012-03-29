/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#pragma once
#include "node.h"
#include "Bullet.h"

class Rock : public Node
{
public:
	Rock(float x, float y, float z);
	~Rock(void) { bullets.clear(); }
	void draw();
	void app(float dt);

	void confuse();

	//Bullet stuff
	const static int MAX_BULLET_LIFETIME = 5;
	const static float BULLETS_PER_SECOND;
	long lastBulletCreationTime;
	vector<Bullet*> bullets;
	void addBullet();
	void removeOldBullets();
	bool intersectsWithBullet(float x, float y, float z);
	void intersectsWithBalloon(float x, float y, float z);

	float x,y,z;
private:
	static const int STATE_IDLE = 0;
	static const int STATE_TARGETING = 1;
	static const int STATE_FIRING = 2;
	static const int STATE_DEACTIVATED = 3;
	MyMesh *rock;

	float yRotation;
	
	float direction[3];
	int state, omega;
};

