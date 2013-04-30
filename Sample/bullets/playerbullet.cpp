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


#include "../core/object.h"

Bullet::Bullet(BulletType mType)
{
	//Initial position
	this->position.x =0;
	this->position.y = 0;
	this->IsAlive = true;
	this->type = mType;
	this->AnimationFrame = 0;
	this->AnimationType = 0;
}

Bullet::~Bullet()
{
}


void Bullet::Update()
{
	if( tick % 7 == 6 )
	{
		AnimationFrame = (AnimationFrame + 1) % 2;
	}
	
	position.x += 1;
	list<cObject*>::iterator it;
	for ( it=enemies.begin() ; it != enemies.end(); it++ )
	{
		cObject* obj = *it;
		if( obj->IsExploding == true )
			continue;
		if(obj->IsColliding(this) && obj->ShieldLifetime == 0)
		{

			switch(this->type)
			{
				case Bullet1:
					obj->hp -= 1;
					break;
				case Bullet2:
					obj->hp -= 2;
					break;
				case Bullet3:
					obj->hp -= 3;
					break;
				case Bullet4:
					obj->hp -= 4;
					break;
				case Bullet5:
					obj->hp -= 5;
					break;
				case Bullet6:
					obj->hp -= 6;
					break;
			}

			if( obj->hp <= 0 )
			{
				obj->Death();
			}

			obj->ShieldLifetime = 5;
		}
	}

	int vx = 0;
	switch(this->type)
	{
		case Bullet1:
			vx = 3;
			break;
		case Bullet2:
			vx = 4;
			break;
		case Bullet3:
			vx = 20;
			break;
		case Bullet4:
			vx = 40;
			break;
		case Bullet5:
			vx = 60;
			break;
		case Bullet6:
			vx = 70;
			break;
	}

	Point p = position;
	p.x += vx;

	if( charmap->IsCollediable(p) )
	{
		this->IsAlive = false; 
		return;
	}


	if( position.x > SCRWIDTH )
			IsAlive = false;
}

void Bullet::Draw(Surface *surface)
{
	if( AnimationType == 0 ) 
	{
		PlayerGraphics1->AddAreaTo(244,81,264,96, surface,(int)position.x ,(int)position.y);
		if( tick % 7 == 6 )
		{
			AnimationType = 1;
		}
	}
	else
	{
		switch(type)
		{
			case Bullet1:
				if( AnimationFrame == 0 )
					PlayerGraphics1->AddAreaTo(230,81,243,96, surface,(int)position.x ,(int)position.y);
				else
					PlayerGraphics1->AddAreaTo(213,81,229,96, surface,(int)position.x ,(int)position.y);
				break;
			case Bullet2:
				if( AnimationFrame == 0 )
					PlayerGraphics1->AddAreaTo(248,97,264,112, surface,(int)position.x ,(int)position.y);
				else
					PlayerGraphics1->AddAreaTo(230,97,247,112, surface,(int)position.x ,(int)position.y);
				break;
			case Bullet3:
				if( AnimationFrame == 0 )
					PlayerGraphics1->AddAreaTo(232,113,264,128, surface,(int)position.x ,(int)position.y);
				else
					PlayerGraphics1->AddAreaTo(198,113,231,128, surface,(int)position.x ,(int)position.y);
				break;
			case Bullet4:
				if( AnimationFrame == 0 )
					PlayerGraphics1->AddAreaTo(216,131,264,149, surface,(int)position.x ,(int)position.y);
				else
					PlayerGraphics1->AddAreaTo(166,131,215,149, surface,(int)position.x ,(int)position.y);
				break;
			case Bullet5:
				if( AnimationFrame == 0 )
					PlayerGraphics1->AddAreaTo(200,149,264,167, surface,(int)position.x ,(int)position.y);
				else
					PlayerGraphics1->AddAreaTo(134,149,199,167, surface,(int)position.x ,(int)position.y);
				break;
			case Bullet6:
				if( AnimationFrame == 0 )
					PlayerGraphics1->AddAreaTo(184,167,264,186, surface,(int)position.x ,(int)position.y);
				else
					PlayerGraphics1->AddAreaTo(102,167,183,186, surface,(int)position.x ,(int)position.y);
				break;
		}
	}
}






