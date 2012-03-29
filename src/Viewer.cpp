/* Copyright (C) 2011 uberspot
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3 as published
* by the Free Software Foundation (http://www.gnu.org/licenses/gpl.txt).
*/

#include "Viewer.h"

Viewer::Viewer()
{
}

void Viewer::draw(Airship* balloon)
{
	gluLookAt(position[0], position[1], position[2], balloon->coordinates[0] , balloon->coordinates[1]-40.0f, balloon->coordinates[2], 0.0,1.0,0.0);
}

void Viewer::app(Airship* balloon) {
	   float viewer[3];
       viewer[0] = position[0] - balloon->coordinates[0];
       viewer[1] = position[1] - balloon->coordinates[1];
       viewer[2] = position[2] - balloon->coordinates[2];

       float mul = 1.0f / sqrtf(viewer[0]*viewer[0]+viewer[1]*viewer[1]+viewer[2]*viewer[2]);
       viewer[0]*= mul;
       viewer[1]*= mul;
       viewer[2]*= mul;

       float viewer_expected[3];
       viewer_expected[0] =     - balloon->front[0];
       viewer_expected[1] =  0.2 - balloon->front[1];
       viewer_expected[2] =     - balloon->front[2];

       float viewer_new[3];
       viewer_new[0] = viewer_expected[0] + viewer[0];
       viewer_new[1] = viewer_expected[1] + viewer[1];
       viewer_new[2] = viewer_expected[2] + viewer[2];

       mul = 1.0f / sqrtf( viewer_new[0]*viewer_new[0] + viewer_new[1]*viewer_new[1] + viewer_new[2]* viewer_new[2] );
       viewer_expected[0]*= mul;
       viewer_expected[1]*= mul;
       viewer_expected[2]*= mul;

       position[0] = balloon->coordinates[0] + 90 * viewer_expected[0];
       position[1] = balloon->coordinates[1] + 120 * viewer_expected[1];
       position[2] = balloon->coordinates[2] + 220 * viewer_expected[2]; 
}