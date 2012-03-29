/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#pragma once
#include "node.h"
#include "Airship.h"
#include <math.h>

class Sky :
	public Node
{
private:
	MyMesh *globe;
public:
	Sky(void);
	~Sky(void) { }
	void draw();
	void app(Airship *air);
	float pos[3];
};

