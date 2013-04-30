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

//Enemy6
Enemy6::Enemy6()
{
	showbullet = false;
	bulletLifetime = 0;
	IsExploding = false;
	ExplosionFrame = 0;
	IsAlive = true;
	hp = 20;
}

void Enemy6::Draw(Surface *surface)
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
				//score += 45;
				break;
		}
		return;
	}


	

	if( showbullet )
	{
		EnemyGraphics7->AddAreaTo(111,0,164,53, surface,(int)position.x ,(int)position.y);
		switch( bulletframe )
		{
			case 0:
				EnemyGraphics7->PartialCopyAreaTo(134,77,164,88, surface,(int)(position.x - 20),(int)(position.y + 12));
				break;
			case 1:
				EnemyGraphics7->PartialCopyAreaTo(99,77,133,88, surface,(int)(position.x - 20),(int)(position.y + 12));
				break;
			case 2:
				EnemyGraphics7->PartialCopyAreaTo(66,77,98,88, surface,(int)(position.x - 20),(int)(position.y + 12));
				break;
		}
	}
	else
	{
		switch( frame )
		{
			case 0:
				EnemyGraphics7->PartialCopyAreaTo(0,0,50,53, surface,(int)position.x ,(int)position.y);
				break;
			case 1:
				EnemyGraphics7->PartialCopyAreaTo(50,0,111,53, surface,(int)position.x ,(int)position.y);
				break;
			case 2:
				EnemyGraphics7->PartialCopyAreaTo(111,0,164,53, surface,(int)position.x ,(int)position.y);
				break;
		}
	}
}

void Enemy6::Update()
{
	if( tick % 21 == 20 )
	{

		if( IsExploding )
		{
			ExplosionFrame = (ExplosionFrame + 1) % 8;
			return;
		}


		frame = 0;
		if( player->position.y < position.y )
		{
			position.y -= 5;
			frame = 1;
		}
		else if( player->position.y > position.y + 40 )
		{
			position.y += 5;
			frame = 1;
		}
		if( player->position.x > position.x - 40 )
		{
			position.x += 4;
			frame = 2;
		}
		else if(  player->position.x < position.x - 120 )
		{
			position.x -= 2;
			frame = 1;
		}

		if( showbullet )
		{
			bulletframe = (bulletframe + 1 ) % 3;
			if( bulletframe == 0 ) showbullet = false;
		}
		else
		{
			if( player->position.y > position.y && player->position.y < position.y + 40 &&
				player->position.x < position.x )
			{
				int l = (int)(player->position.x - position.x);
				if( bulletLifetime == 0 && l > -400 && l < 400  )
				{
					Shoot();
				}
			}
		}

		if( bulletLifetime > 0 )
			bulletLifetime--;


		if( ShieldLifetime > 0 )
			ShieldLifetime--;
	}
}


void Enemy6::Shoot()
{
	frame = 2;
	bulletLifetime = 30;
	showbullet = true;


	EnemyBullet1* bullet = new EnemyBullet1();
	bullet->position = position;
	v.push_back(bullet);

}


bool Enemy6::IsColliding(Bullet* bullet)
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

	bool IsColliding = bullet->position.x + vx > this->position.x && bullet->position.x + vx < this->position.x + 100 &&
				bullet->position.y + 6 > this->position.y && bullet->position.y - 6 < this->position.y + 300;

	return  IsColliding;
}