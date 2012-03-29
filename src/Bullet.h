/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#pragma once
#include <time.h>
#include "node.h"

class Bullet : public Node
{
public:
	Bullet(float x, float y, float z, float direction[], bool redcolor);
	~Bullet(void);
	void draw();
	void app(float dt);
	long creationTime;

	float x,y,z;
private:
	float direction[3];
	bool redColor;
	const static GLfloat bulletColor[4];
	const static GLfloat redBulletColor[4];
	int uspeed;	//velocity
};

