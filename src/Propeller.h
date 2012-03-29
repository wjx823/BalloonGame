/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#pragma once
#include "node.h"

class Propeller : public Node
{
public:
	Propeller(float x, float y, float z);
	~Propeller(void) { }
	void draw();
	void app(float dt, float x, float y, float z, float yRotation, float speed);
private:
	MyMesh *propeller;
	float yRotation, zRotation;
	float x,y,z;
};

