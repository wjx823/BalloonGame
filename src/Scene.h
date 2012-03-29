/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#pragma once
#include "Node.h"
#include "Sky.h"
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include "OBJLoader.h"
#include "Scene.h"
#include "Airship.h"
#include "Rock.h"
#include "Skeleton.h"
#include "Viewer.h"
#include "Treasure.h"
#include "powerup.h"
#include "bonus.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

class Scene : public Node
{
public:
	Scene(int argc, char* argv[]);
	~Scene(void);
	void draw();
	void app(float dt);
	void setup();
	
	//Glut handling functions
	void reshape(int width, int height);
	void render();
	void idle();
	void motion(int x, int y);
	void mouse(int button, int state, int x, int y);
	void key(unsigned char in, int x, int y);
	void keyUp(unsigned char in, int x, int y);
	void text(const char *str,float size,float width,float posx, float posy, float r, float g, float b);
	void addRock(int x, int y, int z);
	void addSkeleton(int x, int y, int z,float dir[], float dist);
private:
	float fps;
	int score, previousScore;
	bool pushedZ, pushedX;

	long prevTime;
	float getDeltaTime();

	Sky *sky;
	Viewer *camera;
	Airship *balloon;
	Treasure *treasure;
	vector<Rock*> rocks;
	vector<Skeleton*> skeletons;
	vector<PowerUp*> powerups;
	
	float pos[3], zoom, pos_x, pos_y, prev_x, prev_y, angle_x, angle_y;
	float fogDensity;
	float rotY, dispZ;

	GLfloat linearAttValue, quadraticAttvalue;

	//Collision stuff
	void checkForCollisions();
	void checkForRockConfusion();
	void checkForSkeletonDestruction();
	void checkForTreasureDetection();
	void checkForBalloonDestruction();
	void checkForPowerUpCollision();

	//Stage stuff
	void loadStage(const char *);
	vector<string*> stages;
	int current_stage;
	void nextStage();
	void reloadStage();
};

