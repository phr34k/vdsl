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

//Enemy2
Enemy2::Enemy2(Enemy2* p, Enemy2* n)
{
	this->previous = NULL;
	this->next = NULL;
	AnimationFrame = 0;
	radiant	= 0;
	speed = 0;
	state = 0;
	this->hp = 1;

	if( n != NULL && p != NULL)
	{
		//Segment
		this->previous = p;
		p->next = this;
		this->next = n;
		n->previous = this;
	}
	else if( n != NULL )
	{
		//tail
		this->next = n;
		n->previous = this;
	}
	else if( p != NULL )
	{
		//Head
		this->previous = p;
		p->next = this;
	}
}

void Enemy2::Update()
{

		if( tick % 21 == 20 )
		{

			if( IsExploding )
			{
				ExplosionFrame = (ExplosionFrame + 1) % 8;
				return;
			}

			if( ShieldLifetime > 0 )
				ShieldLifetime--;
		}

	
		if( this->previous == NULL )
		{

			if( tick % 32 == 31 )
			{

				Enemy2* m = getTail();
				while( m != NULL )
				{
					if( m->previous != NULL )
					{
						m->radiant = m->previous->radiant;
						m->speed =  m->previous->speed;
						m->AnimationFrame = m->previous->AnimationFrame;
						m->state = m->previous->state;
						m->IsActivated = m->previous->IsActivated;
					}
					else
					{
						if( state == 0 )
						{
							if( !player )
							{
								m->speed = 0;
								state = 0;
							}
							else if(  (player->position.y - position.y >= -5 && player->position.y - position.y <= 5 ) && 
								 (player->position.x - position.x >= -5 && player->position.x - position.x <= 5 ) )
							{
								m->speed = 0;
								state = 1;
							}
							else
							{
								double radiant = atan2( (float)(player->position.y-position.y), (float)(player->position.x-position.x) );
								m->radiant = radiant;
								m->speed = 1;
								m->AnimationFrame = (16 + (int)(radiant * 8 / pi)) % 16;
							}
						}
						else
						{
							m->speed = 1;

							if( m->position.x  < 0 || m->position.y < 0 || m->position.y > SCRHEIGHT || m->position.x > SCRWIDTH ) 
							{
								m->IsAlive = false;
								if( m->next != NULL )
								m->next->previous = NULL;
							}
						}
					}

					m = m->previous;
				}
			}

			else if(  tick % 3 == 2 )
			{
					Enemy2* m = getTail();
					while( m != NULL )
					{
						if( m->IsExploding == false ) 
						{
						m->position.x += (float)(cos(m->radiant) * m->speed);
						m->position.y += (float)(sin(m->radiant) * m->speed);
						}
						m = m->previous;
					}
					
			}

		}	
	
}



bool Enemy2::IsColliding(Bullet* bullet)
{
	int vx = 0;
	switch(bullet->type)
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
			vx = 20;
			break;
		case Bullet5:
			vx = 30;
			break;
		case Bullet6:
			vx = 30;
			break;
	}

	return  (bullet->position.x > this->position.x ||  bullet->position.x + vx > this->position.x ) && (bullet->position.x + vx < this->position.x + 40 || bullet->position.x < this->position.x + 40) &&
				bullet->position.y > this->position.y - 50 && bullet->position.y + 17 < this->position.y + 90;
}

Enemy2* Enemy2::getTail()
{
	if( !this->next )
		return this;
	else
		return next->getTail();
}

Enemy2* Enemy2::getHead()
{
	if( !this->previous )
		return this;
	else
		return previous->getTail();
}

void Enemy2::Draw(Surface *surface)
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
				//score += 5;

				if( this->next )
					this->next->previous = NULL;
				if( this->previous )
					this->previous->next = NULL;
				break;
		}
		return;
	}


	switch( AnimationFrame )
	{
		case 0:
			EnemyGraphics3->PartialCopyAreaTo(0,0,30,27, surface,(int)position.x ,(int)position.y);
			break;
		case 1:
			EnemyGraphics3->PartialCopyAreaTo(32,0,63,27, surface,(int)position.x ,(int)position.y);
			break;
		case 2:
			EnemyGraphics3->PartialCopyAreaTo(64,0,95,27, surface,(int)position.x ,(int)position.y);
			break;
		case 3:
			EnemyGraphics3->PartialCopyAreaTo(96,0,127,27, surface,(int)position.x ,(int)position.y);
			break;
		case 4:
			EnemyGraphics3->PartialCopyAreaTo(128,0,159,27, surface,(int)position.x ,(int)position.y);
			break;
		case 5:
			EnemyGraphics3->PartialCopyAreaTo(160,0,191,27, surface,(int)position.x ,(int)position.y);
			break;
		case 6:
			EnemyGraphics3->PartialCopyAreaTo(192,0,223,27, surface,(int)position.x ,(int)position.y);
			break;
		case 7:
			EnemyGraphics3->PartialCopyAreaTo(224,28,255,54, surface,(int)position.x ,(int)position.y);
			break;
		case 8:
			EnemyGraphics3->PartialCopyAreaTo(0,28,30,54, surface,(int)position.x ,(int)position.y);
			break;
		case 9:
			EnemyGraphics3->PartialCopyAreaTo(32,28,63,54, surface,(int)position.x ,(int)position.y);
			break;
		case 10:
			EnemyGraphics3->PartialCopyAreaTo(64,28,95,54, surface,(int)position.x ,(int)position.y);
			break;
		case 11:
			EnemyGraphics3->PartialCopyAreaTo(96,28,127,54, surface,(int)position.x ,(int)position.y);
			break;
		case 12:
			EnemyGraphics3->PartialCopyAreaTo(128,28,159,54, surface,(int)position.x ,(int)position.y);
			break;
		case 13:
			EnemyGraphics3->PartialCopyAreaTo(160,28,191,54, surface,(int)position.x ,(int)position.y);
			break;
		case 14:
			EnemyGraphics3->PartialCopyAreaTo(192,28,223,54, surface,(int)position.x ,(int)position.y);
			break;
		case 15:
			EnemyGraphics3->PartialCopyAreaTo(224,28,255,54, surface,(int)position.x ,(int)position.y);
			break;
	}
}