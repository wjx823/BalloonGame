/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#pragma once
#include "node.h"

class Skeleton : public Node
{
public:
	Skeleton(float x, float y, float z,float direction[], float d);
	~Skeleton(void) { }
	void draw();
	void app(float dt);
	bool intersectsWith(float x, float y, float z);
	float x,y,z;
private:
	MyMesh *skeleton;
	float yRotation;
	
	float uspeed; //velocity
	float dir[3]; //normalized vector of direction
	bool direction;
	float dist;   //distance covered till now
	float max_dist;
};

