/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#pragma once
#include "powerup.h"

class Bonus : public PowerUp
{
public:
	Bonus(float x, float y, float z);
	void draw();
	void app(float dt);
	bool intersectsWithBalloon(float x, float y, float z);
};

