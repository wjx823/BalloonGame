/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#pragma once
#include "node.h"
#include "Airship.h"

class Viewer : public Node
{
public:
	Viewer();
	~Viewer(void) { }
	void draw(Airship*);
	void app(Airship*);
private:
	float position[3];
};

