/**
 * Domain Specific Visual Programming Language
 * Copyright (c) 2013 NHTV UNIVERSITY OF APPLIED SCIENCES
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Lawrence Kok
 *
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "../stdafx.h"

extern int tick;
extern int beamstrength;
extern Surface* PlayerGraphics1;
extern Surface* EnemyGraphics1;
extern Surface* EnemyGraphics2;
extern Surface* EnemyGraphics3;
extern Surface* EnemyGraphics4;
extern Surface* EnemyGraphics5;
extern Surface* EnemyGraphics6;
extern Surface* EnemyGraphics7;
extern Surface* EnemyGraphics8;
extern Surface* EnemyGraphics9;
extern Surface* EnemyGraphics10;
using namespace std;

class Bullet;
class cObject;
class Charmap;
class Player;
extern list<cObject*> enemies;
extern list<cObject*> v;
extern Charmap* charmap;
extern Player* player;

struct Vector
{
	unsigned short angle;
	float speed;
};

struct Point
{
	float x;
	float y;
};

enum PlayerActionState
{
	StageEnter,
	StageInProgress,
	StageLeave
};

enum BulletType
{
	Bullet1,
	Bullet2,
	Bullet3,
	Bullet4,
	Bullet5,
	Bullet6
};

enum Enemy3Type
{
	UpperRight,
	UpperLeft,
	BottomRight,
	BottomLeft
};

class cObject
{
public:

	cObject(){ 
		IsAlive = true; 
		IsExploding = false;
		ExplosionFrame = 0;
		IsActivated = false;
		hp = 6;
		ShieldLifetime = 0;
		flow = 0;
	};

	int hp;
	int ShieldLifetime;
	bool IsAlive;
	bool IsActivated;
	bool IsExploding;
	int ExplosionFrame;
	Point position;	
	void* flow;

    virtual void Draw(Surface* surface) = 0;
    virtual void Update() = 0;
	virtual bool IsColliding(Bullet* bullet){ return false; };
	virtual void LeaveViewport();
	virtual void Death();
	virtual void ModifyHealth();
	virtual void KeyPressed(int key);
	virtual void KeyReleased(int key);
};

class Player : public cObject
{
private:
	Surface* graphics;
	Surface* fuelgraphics;
	Surface* chargegraphics;
	int fuelframe;
	int chargeframe;
	int playerframe;	
public:
	bool showBeamCollection;
	bool isfireing;
	PlayerActionState state;
	Player();
	~Player();
    void Draw(Surface* surface) ;
    void Update();
};

class Enemy : public cObject
{
private:	
	int frame;
	int bulletframe;
	bool showbullet;
	int bulletLifetime;
public:
	Enemy();
	void Draw(Surface* surface);
	void Update();
	void Shoot();
	bool IsColliding(Bullet* bullet);
};

class Enemy1 : public cObject
{
private:
	int AnimationFrame;
	short rotator;
	int bulletLifetime;
public:
	Enemy1();
	void Draw(Surface* surface);
	void Update();
	void Shoot();
};

class Enemy2 : public cObject
{
private:
	int AnimationFrame;
	double radiant;
	int speed;
	int state;
public:
	Enemy2(Enemy2* next, Enemy2* previous);
	void Draw(Surface* surface);
	void Update();
	bool IsColliding(Bullet* bullet);
	Enemy2* next;
	Enemy2* previous;
	Enemy2* getTail();
	Enemy2* getHead();
};


class Enemy3 : public cObject
{
public:
	int Frame;
	int bulletLifetime;
	Enemy3Type mType;
	Enemy3();
	void Draw(Surface* surface);
	void Update();
	bool IsColliding(Bullet* bullet);
	void Shoot();
};

class Enemy4 : public cObject 
{
public:
	int Frame;
	int bulletLifetime;
	Enemy3Type mType;	
	Enemy4();
	void Draw(Surface* surface);
	void Update();
	void Shoot();
	bool IsColliding(Bullet* bullet);
};

class Enemy5 : public cObject 
{
public:
	int bulletLifetime;
	int state;
	int direction;
	int Frame;
	int AimFrame;
	Enemy5();
	void Draw(Surface* surface);
	void Update();
	void Shoot();
	bool IsColliding(Bullet* bullet);;
};

class Enemy6 : public cObject
{
private:
	int frame;
	int bulletframe;
	bool showbullet;
	int bulletLifetime;

public:
	Enemy6();
	void Draw(Surface* surface);
	void Update();
	void Shoot();
	bool IsColliding(Bullet* bullet);
};

class Enemy7 : public cObject 
{
public:
	int Frame;
	int bulletLifetime;
	Enemy3Type mType;
	Enemy7();
	void Draw(Surface* surface);
	void Update();
	bool IsColliding(Bullet* bullet);
	void Shoot();
};

class Enemy8 : public cObject
{
private:
	int AnimationFrame;
	double radiant;
	int speed;
	int state;
	unsigned short rotator;
public:
	Enemy8(Enemy8* next, Enemy8* previous);
	void Draw(Surface* surface);
	void Update();
	bool IsColliding(Bullet* bullet);
	Enemy8* next;
	Enemy8* previous;
	Enemy8* getTail();
	Enemy8* getHead();
};

class Enemy9 : public cObject
{
private:
	int AnimationFrame;
	int TailFrame;
	int HeadFrame;
	int WaitTime;
	int Direction;
	int bulletLifetime;
public:
	Enemy9();
	void Draw(Surface* surface);
	void Update();
	void Shoot();
	bool IsColliding(Bullet* bullet);
};

class Bullet : public cObject
{
private:
	int AnimationFrame;
	int AnimationType;
public:
	Bullet(BulletType mType);
	~Bullet();
	BulletType type;
    void Draw(Surface* surface);
    void Update();
};

class EnemyBullet1 : public cObject
{
private:
	int AnimationFrame;
public:
    void Draw(Surface* surface);
    void Update();
};

class EnemyBullet2 : public cObject
{
private:
	int AnimationFrame;
public:
	float vx;
	float vy;
	EnemyBullet2();
    void Draw(Surface* surface);
	void Update();
};

class EnemyBullet3 : public cObject
{
private:
	int AnimationFrame;
public:
	float vx;
	float vy;
	EnemyBullet3();
    void Draw(Surface* surface);
    void Update();
};


#endif