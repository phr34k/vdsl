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

Point TailObject0[] = 
{
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },

	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
	{ 40, 170 },
};

Point TailObject1[] = 
{
	{ 30, 167 },
	{ 30, 167 },
	{ 31, 166 },
	{ 31, 166 },
	{ 31, 165 },
	{ 31, 165 },
	{ 31, 164 },
	{ 31, 163 },
	{ 31, 162 },
	{ 31, 161 },
	{ 31, 160 },
	{ 31, 159 },

	{ 31, 158 },
	{ 31, 160 },
	{ 31, 161 },
	{ 31, 162 },
	{ 31, 163 },
	{ 31, 164 },
	{ 31, 165 },
	{ 31, 165 },
	{ 31, 166 },
	{ 31, 166 },
	{ 30, 167 },
	{ 30, 167 },
};




//Enemy9
Enemy9::Enemy9()
{
	this->AnimationFrame = 0;
	this->hp = 1;
	this->IsActivated = false;
	this->TailFrame = 0;
	Direction = 0;
	HeadFrame = 0;
	WaitTime = 20;
	bulletLifetime = 0;
	hp = 30;
}

void Enemy9::Draw(Surface* surface)
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
				//score += 500;
				player->state = StageLeave;
				//GameWon = true;
				break;
		}
		return;
	}


	switch( AnimationFrame )
	{
		case 0:
			EnemyGraphics10->CopyAreaTo(0,0,160,210, surface,(int)position.x ,(int)position.y);
			break;
		case 1:
			EnemyGraphics10->CopyAreaTo(162,0,321,210, surface,(int)position.x ,(int)position.y);
			break;
		case 2:
			EnemyGraphics10->CopyAreaTo(324,0,481,210, surface,(int)position.x ,(int)position.y);
			break;
	}



	switch( HeadFrame )
	{
		case 0:
			EnemyGraphics10->PartialCopyAreaTo(578,218,611,251, surface,(int)(position.x + 71),(int)(position.y + 93));
			break;
		case 1:
			EnemyGraphics10->PartialCopyAreaTo(545,218,577,251, surface,(int)(position.x + 71),(int)(position.y + 93));
			break;
		case 2:
			EnemyGraphics10->PartialCopyAreaTo(512,218,544,251, surface,(int)(position.x + 71),(int)(position.y + 93));
			break;
		case 3:
			EnemyGraphics10->PartialCopyAreaTo(479,218,511,251, surface,(int)(position.x + 71),(int)(position.y + 93));
			break;
		case 4:
			EnemyGraphics10->PartialCopyAreaTo(446,218,478,251, surface,(int)(position.x + 71),(int)(position.y + 93));
			break;
		case 5:
			EnemyGraphics10->PartialCopyAreaTo(413,218,445,251, surface,(int)(position.x + 71),(int)(position.y + 93));
			break;
		case 6:
			EnemyGraphics10->PartialCopyAreaTo(578,254,611,287, surface,(int)(position.x + 71),(int)(position.y + 93));
			break;
		case 7:
			EnemyGraphics10->PartialCopyAreaTo(545,254,577,287, surface,(int)(position.x + 71),(int)(position.y + 93));
			break;
		case 8:
			EnemyGraphics10->PartialCopyAreaTo(512,254,544,287, surface,(int)(position.x + 71),(int)(position.y + 93));
			break;
	}


	EnemyGraphics10->PartialCopyAreaTo(593,287,611,306, surface,(int)(position.x + TailObject0[TailFrame].x) ,(int)(position.y + TailObject0[TailFrame].y));
	EnemyGraphics10->PartialCopyAreaTo(593,287,611,306, surface,(int)(position.x + TailObject1[TailFrame].x) ,(int)(position.y + TailObject1[TailFrame].y));


	
	
}

void Enemy9::Update()
{

	if( tick % 21 == 20 )
		{

			if( IsExploding )
			{
				ExplosionFrame = (ExplosionFrame + 1) % 8;
				return;
			}

			if( bulletLifetime > 0 )
				bulletLifetime--;


			if( ShieldLifetime > 0 )
				ShieldLifetime--;
		}


	if( tick % 64 == 63 )
	{
		AnimationFrame = (AnimationFrame + 1 ) % 3;
		TailFrame = (TailFrame + 1 ) % 24;

		if( HeadFrame > 0 ) 
		{
			if( WaitTime > 0 ) 
			{
				WaitTime--;

				if( HeadFrame == 8 && bulletLifetime == 0 )
					Shoot();
			}
			else if( Direction == 0 ) 
			{
				if( HeadFrame < 8 )
					HeadFrame++;
				if( HeadFrame == 8 )
				{
					Direction = 1;
					WaitTime = 10;
				}
			}
			else
			{
				if( HeadFrame > 1 )
					HeadFrame--;
				if( HeadFrame == 1 )
				{
					Direction = 0;
					WaitTime = 30;
				}
			}
		}

	}
}

void Enemy9::Shoot()
{
	bulletLifetime = 3;

	EnemyBullet3* bullet = new EnemyBullet3();
	bullet->position = position;
	bullet->position.x += 91;
	bullet->position.y += 91;

	bullet->vx = player->position.x - bullet->position.x;
	bullet->vy = player->position.y - bullet->position.y;
	double d = Distance2D(bullet->vx, bullet->vy);
	bullet->vx /= (float)d;
	bullet->vy /= (float)d;

	v.push_back(bullet);
}
bool Enemy9::IsColliding(Bullet* bullet)
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
			vx = 40;
			break;
		case Bullet5:
			vx = 60;
			break;
		case Bullet6:
			vx = 70;
			break;
	}

	bool IsColliding = bullet->position.x + vx > this->position.x + 71 && bullet->position.x + vx < this->position.x + 103 &&
				bullet->position.y + 6 > this->position.y + 93 && bullet->position.y - 6 < this->position.y + 126;
 
	if( IsColliding && HeadFrame == 0 )
		HeadFrame = 1;
	else if( IsColliding && HeadFrame > 1 )
		return true;
	return  false;
}

