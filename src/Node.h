/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#pragma once
#include <iostream>
#include <vector>
#include "OBJLoader.h"
#include "BMPLoader.h"

using namespace std;

class Node
{
public:
	Node(void);
	~Node(void);
	virtual void draw();
	virtual void app(float dt);
};