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

//Enemy1
Enemy1::Enemy1()
{
	this->AnimationFrame = 0;
	rotator = (unsigned short)RandInt(0, 14000);
	this->hp = 1;
}

void Enemy1::Draw(Surface* surface)
{
	 switch( AnimationFrame )
	 {
		 case 0:
			 EnemyGraphics2->PartialCopyAreaTo(229,0,262,23, surface,(int)(position.x - 20),(int)(position.y + 12));
			 break;
 		 case 1:
			 EnemyGraphics2->PartialCopyAreaTo(196,0,228,23, surface,(int)(position.x - 20),(int)(position.y + 12));
			 break;
 		 case 2:
			 EnemyGraphics2->PartialCopyAreaTo(163,0,195,23, surface,(int)(position.x - 20),(int)(position.y + 12));
			 break;
  		 case 3:
			 EnemyGraphics2->PartialCopyAreaTo(131,0,162,23, surface,(int)(position.x - 20),(int)(position.y + 12));
			 break;
  		 case 4:
			 EnemyGraphics2->PartialCopyAreaTo(98,0,130,23, surface,(int)(position.x - 20),(int)(position.y + 12));
			 break;
   		 case 5:
			 EnemyGraphics2->PartialCopyAreaTo(65,0,97,23, surface,(int)(position.x - 20),(int)(position.y + 12));
			 break;
		 case 6:
			 EnemyGraphics2->PartialCopyAreaTo(33,0,63,23, surface,(int)(position.x - 20),(int)(position.y + 12));
			 break;
		  case 7:
			 EnemyGraphics2->PartialCopyAreaTo(0,0,32,23, surface,(int)(position.x - 20),(int)(position.y + 12));
			 break;
	 }
}

void Enemy1::Update()
{
	if( tick % 21 == 20 )
	{
		position.x -= 2;
		AnimationFrame = (AnimationFrame + 1) % 7;
		rotator = (rotator + 1200) % 65535;
		position.y -= (float)(sin( rotator * (pi / 32768)) * 4);

		if( ShieldLifetime > 0 )
			ShieldLifetime--;
	}

	if( tick % 7 == 6 )
	{
		if( bulletLifetime > 0 )
			bulletLifetime--;
		else if( player && this->position.x > player->position.x )
		{
			int l = (int)(player->position.x - position.x);
			if( RandInt(0,999) > 990 && l > -300 && l < 300 )
			{
				Shoot();
			}
		}
	}

}

void Enemy1::Shoot()
{
	bulletLifetime = 30;

	EnemyBullet2* bullet = new EnemyBullet2();
	bullet->position = position;
	bullet->vx = player->position.x - bullet->position.x;
	bullet->vy = player->position.y - bullet->position.y;
	double d = Distance2D(bullet->vx, bullet->vy);
	bullet->vx /= (float)d;
	bullet->vy /= (float)d;

	v.push_back(bullet);
}
