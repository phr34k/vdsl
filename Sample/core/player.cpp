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

#include "object.h"


using namespace std;
extern Player* player;
extern list<cObject*> v;

#define key_up 38
#define key_left 37
#define key_down 40
#define key_right 39
extern bool IsKeyPressed(char key);

//Player
Player::Player()
{
	//Initialize the player
	this->graphics = NULL;
	this->fuelgraphics = NULL;
	this->chargegraphics = NULL;

	//Initialize graphics of the player
	this->graphics = new Surface("Images/sprites1/sprite_01_01.tga");
	this->fuelgraphics = new Surface("Images/sprites1/sprite_01_02.tga");
	this->chargegraphics = new Surface("Images/sprites1/sprite_01_03.tga");
	this->hp = 100;

	//Initialize states of the player
	this->state = StageEnter;
	this->position.x =0;
	this->position.y = 0;
	this->fuelframe = 0;
	this->chargeframe = 0;
	this->playerframe = 3;
	this->showBeamCollection = false;
	this->isfireing = false;
}

Player::~Player()
{
	//Dtor of the player
	if( this->graphics != NULL )
		delete this->graphics;
}

void Player::Update()
{

	//showBeamCollection = IsKeyPressed(key_space);

	//Update the player
	if( this->state == StageEnter)
	{
		if( tick % 21 == 20 )
		{
			this->position.x += 20;
			if( this->position.x > 130 )
				this->state = StageInProgress;
		}
		else if( tick % 7 == 6 )
		{
			fuelframe = (fuelframe + 1 ) % 4;
		}
	}
	else if( this->state == StageLeave)
	{
		if( tick % 21 == 20 )
		{
			if( this->position.x < SCRWIDTH + 100 )
				this->position.x += 20;
		}
		else if( tick % 7 == 6 )
		{
			fuelframe = (fuelframe + 1 ) % 4;
		}
	}
	else if( this->state == StageInProgress)
	{

		if( tick % 10 == 9 )
		{
			int fx = 0;
			int fy = 0;
			if( IsKeyPressed(key_left) && position.x >= 5)
			{
				fx -= 5;
			}
			if(  IsKeyPressed(key_right)  && position.x < SCRWIDTH - 45 )
			{
				fx += 5;
			}
			if( IsKeyPressed(key_down) && position.y < SCRHEIGHT - 75  )
			{
				fy += 5;
			}
			if( IsKeyPressed(key_up) && position.y >= 5)
			{
				fy -= 5;
			}

			Point p = position;
			p.x += fx;
			p.y += fy;
			
			Point p1 = p;
			Point p2 = p;
			Point p3 = p;
			Point p4 = p;

			p1.x += 30;
			p2.x += 30;
			p3.x += 10;
			p4.x += 10;

			p1.y += 10;
			p2.y -= 5;
			p3.y += 10;
			p4.y -= 5;

			if( !( charmap->IsCollediable(p1) || charmap->IsCollediable(p2) || 
				   charmap->IsCollediable(p3) || charmap->IsCollediable(p4) ) )
			{
				position.x += fx;
				position.y += fy;
			}

			if( fy < 0 && playerframe < 4)
			{
				playerframe++;
			}
			else if( fy > 0 && playerframe > 0 )
			{
				playerframe--;
			}
			else
			{
				if( playerframe < 2 )
					playerframe++;
				else if( playerframe > 3 )
					playerframe--;
			}
		}

		if( tick % 20 == 19) 
		{

			if( IsExploding )
			{
				ExplosionFrame = (ExplosionFrame + 1) % 8;
				return;
			}

			if( isfireing == true )
			{
				chargeframe = (chargeframe + 1 ) % 8;
				if( beamstrength < 100 )
					beamstrength+= 4;

				if( ShieldLifetime > 0 )
					ShieldLifetime--;
			}
			else
			{
				chargeframe = 0;
			}
		}
	}

	if( hp <= 0 ) 
	{
		IsExploding = true;
	}
}

void Player::Draw(Surface* surface)
{

	if( IsExploding )
	{
		switch( ExplosionFrame )
		{
			case 0:
				PlayerGraphics1->AddAreaTo(265,289,283,330, surface,(int)position.x ,(int)position.y);
				break;
			case 1:
				PlayerGraphics1->AddAreaTo(246,289,264,330, surface,(int)position.x ,(int)position.y);
				break;
			case 2:
				PlayerGraphics1->AddAreaTo(214,289,245,330, surface,(int)position.x ,(int)position.y);
				break;
			case 3:
				PlayerGraphics1->AddAreaTo(179,289,213,330, surface,(int)position.x ,(int)position.y);
				break;
			case 4:
				PlayerGraphics1->AddAreaTo(142,289,178,330, surface,(int)position.x ,(int)position.y);
				break;
			case 5:
				PlayerGraphics1->AddAreaTo(106,289,142,330, surface,(int)position.x ,(int)position.y);
				break;
			case 6:
				PlayerGraphics1->AddAreaTo(68,289,105,330, surface,(int)position.x ,(int)position.y);
				break;
			default:
				this->IsAlive = false;
				break;
		}
		return;
	}


	//Draw the sprite
	switch( playerframe )
	{
		case 0:
			graphics->AddAreaTo(0,0,33,15, surface, (int)position.x,(int)position.y);
			break;
		case 1:
			graphics->AddAreaTo(33,0,66,15, surface, (int)position.x,(int)position.y);
			break;
		case 2:
			graphics->AddAreaTo(66,0,99,15, surface, (int)position.x,(int)position.y);
			break;
		case 3: 
			graphics->AddAreaTo(99,0,132,15, surface, (int)position.x,(int)position.y);
			break;
		case 4: 
			graphics->AddAreaTo(132,0,165,15, surface, (int)position.x,(int)position.y);
			break;
	}

	if( this->state == StageEnter || this->state == StageLeave)
	{
		switch( fuelframe )
		{
			case 0:
				fuelgraphics->AddAreaTo(0,0,33,31, surface, (int)(position.x - 33),(int)(position.y - 9));
				break;
			case 1:
				fuelgraphics->AddAreaTo(33,0,66,31, surface, (int)(position.x - 33),(int)(position.y - 9));
				break;
			case 2: 
				fuelgraphics->AddAreaTo(66,0,99,31, surface, (int)(position.x - 33),(int)(position.y - 9));
				break;
			case 3: 
				fuelgraphics->AddAreaTo(99,0,131,31, surface, (int)(position.x - 33),(int)(position.y - 9));
				break;
		}
	}
	else
	{
		//Show the charge graphics
		if( showBeamCollection ) 
		{
			switch( chargeframe )
			{
				case 0:
					chargegraphics->AddAreaTo(0,0,33,33, surface, (int)(position.x + 33),(int)(position.y - 9));
					break;
				case 1:
					chargegraphics->AddAreaTo(33,0,66,33, surface, (int)(position.x + 33),(int)(position.y - 9));
					break;
				case 2: 
					chargegraphics->AddAreaTo(66,0,99,33, surface, (int)(position.x + 33),(int)(position.y - 9));
					break;
				case 3: 
					chargegraphics->AddAreaTo(99,0,132,33, surface, (int)(position.x + 33),(int)(position.y - 9));
					break;
				case 4: 
					chargegraphics->AddAreaTo(132,0,165,33, surface, (int)(position.x + 33),(int)(position.y - 9));
					break;
				case 5: 
					chargegraphics->AddAreaTo(165,0,198,33, surface, (int)(position.x + 33),(int)(position.y - 9));
					break;
				case 6: 
					chargegraphics->AddAreaTo(198,0,231,33, surface, (int)(position.x + 33),(int)(position.y - 9));
					break;
				case 7: 
					chargegraphics->AddAreaTo(231,0,264,33, surface, (int)(position.x + 33),(int)(position.y - 9));
					break;
			}
		}
	}

}









