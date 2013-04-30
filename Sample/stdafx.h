#ifndef STDAFX_H
#define STDAFX_H

//Tweakable modifiers
#define MAX_MOVEMENTTICK 128
#define pi 3.14159265
#define ScrollingSpeed 1

//#define key_space 32
//#define key_enter 13
#define key_pause 19
#define key_backspace 8

//Frequently used function
extern float Rand( float n1, float n2 );
extern int RandInt( int n1, int n2 );
extern double Distance2D(double x, double y);

//Includes standards
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Default includes
#include "math.h"
#include "core/surface.h"
#include "core/charmap.h"
#include "core/object.h"

#endif