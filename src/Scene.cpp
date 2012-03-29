/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#include "Scene.h"

using namespace std;

Scene::Scene(int argc, char *argv[])
{
	score = previousScore = 0;

	zoom = 80.0;
	fogDensity = 0.00;
	GLfloat linearAttValue = 0.0;
	quadraticAttvalue = 0.0;
	prev_x = 0, prev_y = 0, angle_x = 0, angle_y = 0;
	fogDensity = 0;
	rotY = 0, dispZ = 0;

	prevTime = (long) clock();

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

	glutInitWindowSize(1024,668);
	glutInitWindowPosition(50,50);
	glutCreateWindow("Balloon Game");

	setup();

	//fix stages
	stages.push_back(new string("stage1.txt"));
	stages.push_back(new string("stage2.txt"));
	current_stage = 0;
	loadStage(stages[0]->data());
}

void Scene::nextStage() {
	if (current_stage+1 >= stages.size()) {
		cout << "The game is over";
		//show a final message and exit
		exit(0);
	} else {
		rocks.clear();
		skeletons.clear();
		current_stage++;
		previousScore = score;
		loadStage(stages[current_stage]->data());
	}

}

void Scene::reloadStage() {
		rocks.clear();
		skeletons.clear();
		score = previousScore;
		loadStage(stages[current_stage]->data());
}

void Scene::loadStage(const char *fname) {
	//Create the sky
	sky = new Sky();
	//open file
	string line;
	ifstream stage(fname);
	if (stage.is_open()) {
		while (stage.good()){
			getline (stage,line);  //get next line
			if (line.substr(0,4).compare("BALL") == 0) {
				istringstream sparser(line.substr(5));
				string pos[3];
				sparser >> pos[0] >> pos[1] >> pos[2];
				balloon = new Airship(atof(pos[0].data()), atof(pos[1].data()), atof(pos[2].data()));
				balloon->app(0.0001f);
				balloon->clearAllMovement();

				camera = new Viewer();
				camera->app(balloon);
			}
			else if (line.substr(0,4).compare("TREA") == 0) {
				istringstream sparser(line.substr(5));
				string pos[3];
				sparser >> pos[0] >> pos[1] >> pos[2];
				treasure = new Treasure(atof(pos[0].data()), atof(pos[1].data()), atof(pos[2].data()));
			} else if (line.substr(0,4).compare("ROCK") == 0) {
				istringstream sparser(line.substr(5));
				string pos[3];
				sparser >> pos[0] >> pos[1] >> pos[2];
				addRock(atof(pos[0].data()), atof(pos[1].data()), atof(pos[2].data()));
			} else if (line.substr(0,4).compare("SKEL") == 0) {
				istringstream sparser(line.substr(5));
				string pos[3], sdir[3], dist;
				sparser >> pos[0] >> pos[1] >> pos[2] >> sdir[0] >> sdir[1] >> sdir[2] >> dist;
				float dir[3] = {atof(sdir[0].data()), atof(sdir[1].data()), atof(sdir[2].data())};
				addSkeleton(atof(pos[0].data()), atof(pos[1].data()), atof(pos[2].data()), dir, atof(dist.data()));
			} else if (line.substr(0,4).compare("POW1") == 0) {
				istringstream sparser(line.substr(5));
				string pos[3];
				sparser >> pos[0] >> pos[1] >> pos[2];
				powerups.push_back(new PowerUp(atof(pos[0].data()), atof(pos[1].data()), atof(pos[2].data())));
			} else if (line.substr(0,4).compare("POW2") == 0) {
				istringstream sparser(line.substr(5));
				string pos[3];
				sparser >> pos[0] >> pos[1] >> pos[2];
				powerups.push_back(new Bonus(atof(pos[0].data()), atof(pos[1].data()), atof(pos[2].data())));
			}
		}
		stage.close();
	}
}

void Scene::addRock(int x, int y, int z) {
	Rock* rock = new Rock(x, y , z);
	rocks.push_back(rock);
}

void Scene::addSkeleton(int x, int y, int z, float dir[], float dist) {
	Skeleton* skel = new Skeleton(x, y , z, dir, dist);
	skeletons.push_back(skel);
}

Scene::~Scene(void) {	
	rocks.clear();
	skeletons.clear();
}

void Scene::draw() {
	camera->draw(balloon);
	treasure->draw();
	sky->draw();
	float *coords = treasure->getPos();
	balloon->draw(coords[0], coords[1], coords[2]);
	int size = rocks.size();
	for (int i = 0; i < size; i++) {
		rocks[i]->draw();
	}
	size = skeletons.size();
	for (int i = 0; i < size; i++) {
		skeletons[i]->draw();
	}
	size = powerups.size();
	for (int i = 0; i < size; i++) {
		powerups[i]->draw();
	}
}

void Scene::app(float dt) {
	balloon->app(dt);

	GLfloat light_position1[] = {balloon->coordinates[0], balloon->coordinates[1] + 20.0f, balloon->coordinates[2]+30.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	treasure->app(dt);
	sky->app(balloon);
	camera->app(balloon);
	int size = rocks.size();
	for (int i = 0; i < size; i++) {
		rocks[i]->app(dt);
	}
	size = skeletons.size();
	for (int i = 0; i < size; i++) {
		skeletons[i]->app(dt);
	}
	size = powerups.size();
	for (int i = 0; i < size; i++) {
		powerups[i]->app(dt);
	}
	checkForCollisions();
}

void Scene::checkForCollisions() {
	checkForRockConfusion();
	checkForSkeletonDestruction();
	checkForTreasureDetection();
	checkForBalloonDestruction();
	checkForPowerUpCollision();
}

void Scene::checkForBalloonDestruction() {
	for (unsigned int i = 0; i < skeletons.size(); i++) {
		if(balloon->intersectsWith(skeletons[i]->x, skeletons[i]->y, skeletons[i]->z) ){
			cout<< "SKELETON KILLED ME!! DISPLAY SOMETHING! \n";
			reloadStage();
			break;		
		}
	}

	bool reloaded = false;
	for (unsigned int i = 0; i < rocks.size() && !reloaded; i++) {
		unsigned int bulletsSize = rocks[i]->bullets.size();
		if(balloon->intersectsWith(rocks[i]->x, rocks[i]->y, rocks[i]->z) ) {
			reloadStage();
			break;
		}
		for(unsigned int j =0; j<bulletsSize; j++) {
			if(balloon->intersectsWithBullet(rocks[i]->bullets[j]->x, rocks[i]->bullets[j]->y, rocks[i]->bullets[j]->z)){
				reloadStage();
				reloaded = true;
				break;		
			}
		}
	}
}

void Scene::checkForTreasureDetection() {
	if(balloon->intersectsWith(treasure->x, treasure->y, treasure->z) ){
		cout<< "TREASURE FOUND!! DISPLAY SOMETHING...! \n";
		nextStage();
	}
}

void Scene::checkForPowerUpCollision() {
	for (unsigned int i = 0; i < powerups.size(); i++) {
		if (powerups[i]->intersectsWithBalloon(balloon->coordinates[0], balloon->coordinates[1], balloon->coordinates[2])) {
			if (powerups[i]->type == 1)
				balloon->speedUp();
			if (powerups[i]->type == 2)
				score += 100;
			powerups.erase(powerups.begin()+(i--));
		}
		
	}
}

void Scene::checkForRockConfusion() {
	for (unsigned int i = 0; i < rocks.size(); i++) {
		unsigned int bulletsSize = balloon->bullets.size();
		rocks[i]->intersectsWithBalloon(balloon->coordinates[0], balloon->coordinates[1], balloon->coordinates[2]);
		for(unsigned int j =0; j<bulletsSize; j++) {
			if(rocks[i]->intersectsWithBullet(balloon->bullets[j]->x, balloon->bullets[j]->y, balloon->bullets[j]->z)){
				rocks[i]->confuse();
				break;		
			}
		}
	}
}

void Scene::checkForSkeletonDestruction() {
	for (unsigned int i = 0; i < skeletons.size(); i++) {
		unsigned int bulletsSize = balloon->bullets.size();
		for(unsigned int j =0; j<bulletsSize; j++) {
			if(skeletons[i]->intersectsWith(balloon->bullets[j]->x, balloon->bullets[j]->y, balloon->bullets[j]->z)){
				skeletons.erase(skeletons.begin()+(i--));
				score += 20; //update score
				break;		
			}
		}
	}
}

void Scene::setup()
{
	pos[0]=pos[1]=pos[2]=50.0;
	GLfloat light_position[] = { 0.0f, 50.0f, 100.0f, 1.0f };
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0);


	GLfloat light_position1[] = { 0.0f, 20.0f, 30.0f, 1.0f };
	GLfloat ambientLight1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight1[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	glLightfv( GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv( GL_LIGHT1, GL_AMBIENT, ambientLight1 );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuseLight1 );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT1);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glClearDepth(1.0);


	//Light Attenuation
	//there are three attenuation cooefficents:
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAttValue);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadraticAttvalue);
}

void Scene::render()
{
	//extern GLuint metalTextureID, targetTextureID;
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	draw();

	float dt = getDeltaTime();
	app(dt);

	char* str = new char[30];
	sprintf(str, "STAGE: %d SCORE: %d", (current_stage+1), score ); 
	text(str, 0.1f, 2, 10,550, 1, 1, 1);  //width, position in pixels (3rd/4th parameter)
	
	glutSwapBuffers();
}

void Scene::idle()
{
	glutPostRedisplay();
}

void Scene::key(unsigned char charIn, int x, int y)
{
	switch(charIn)
	{
		case 'q':case 'Q':
			balloon->moveDown();
			break;
		case 'e':case 'E':
			balloon->moveUp();
			break;
		case 'w':case 'W':
			pushedZ = true;
			balloon->moveFront();
			break;
		case 'd':case 'D':
			pushedX = true;
			balloon->moveRight();
			break;
		case 'a':case 'A':
			pushedX = true;
			balloon->moveLeft();
			break;
		case 's':case 'S':
			balloon->moveBack();
			break;
		case 'x':case 'X':
			linearAttValue -= 0.001;
			if (linearAttValue < 0.0) linearAttValue = 0.0;
			break;
		case ' ':
			balloon->addBullet();
			break;	
		case '\033':		//Esc key!		
			exit(0);
		break;
	}
	
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAttValue);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadraticAttvalue);
		
	glutPostRedisplay();
}

void Scene::keyUp(unsigned char charIn, int x, int y)
{
	switch(charIn)
	{
		case 'q':case 'Q':
		case 'e':case 'E':
			balloon->clearYMovement();
			break;
		case 'w':case 'W':
		case 's':case 'S':
			pushedZ = false;
			if(!pushedX)
				balloon->clearZMovement();
			break;
		case 'd':case 'D':
		case 'a':case 'A':
			balloon->clearXMovement();
			pushedX = false;
			if(!pushedZ)
				balloon->clearZMovement();
			break;
	}
}

void Scene::reshape(int width,int height)
{
	if (height==0) height=1;
	glViewport(0,0,(GLsizei)width,(GLsizei)height);

	// Setup viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)width/(float)height, 1.0, 800.0 );
}

void Scene::text(const char *str,float size,float width,float posx, float posy, float r, float g, float b)
{
	glColor3f(r,g,b);
	glEnable(GL_COLOR_MATERIAL);

	/// backup current model-view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();                     /// save current modelview matrix
	glLoadIdentity();                   /// resolutions::RESet modelview matrix

	/// set to 2D orthogonal projection
	glMatrixMode(GL_PROJECTION);		/// switch to projection matrix
	glPushMatrix();						/// save current projection matrix
	glLoadIdentity();					/// resolutions::RESet projection matrix
	glOrtho (0, 600, 0, 600, -1, 1);

	glPushMatrix();
	glTranslatef (posx,posy,0.0);
	glScalef(size,size,size);
	glLineWidth(width);
	for (int i=0;i<(int)strlen(str);i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
	glPopMatrix();	

	glMatrixMode(GL_PROJECTION);		/// switch to modelview matrix
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);			/// switch to modelview matrix
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
}

float Scene::getDeltaTime() {
	long currentTime = clock();
	float dt = (float)(currentTime - prevTime) / CLOCKS_PER_SEC;
	prevTime = currentTime;
	return (dt == 0) ? 0.0001 : dt;
}