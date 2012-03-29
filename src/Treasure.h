/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#pragma once
#include "node.h"

class Treasure : public Node
{
public:
	Treasure(float x, float y, float z);
	~Treasure(void) { }
	void draw();
	void app(float dt);
	float *getPos();
	float x,y,z;
private:
	MyMesh *treasure;
	float omega, yRotation; //velocity
};

