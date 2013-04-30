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

EnemyBullet3::EnemyBullet3()
{
	AnimationFrame = 0;
}


void EnemyBullet3::Draw(Surface* surface)
{

	switch(AnimationFrame)
	{
		case 1:
			EnemyGraphics10->AddAreaTo(588,354,611,373, surface,(int)(position.x - 20),(int)(position.y + 12));
			break;
		case 2:
			EnemyGraphics10->AddAreaTo(565,354,588,373, surface,(int)(position.x - 20),(int)(position.y + 12));
			break;
		case 3:
			EnemyGraphics10->AddAreaTo(542,354,565,373, surface,(int)(position.x - 20),(int)(position.y + 12));
			break;
		case 4:
			EnemyGraphics10->AddAreaTo(519,354,542,373, surface,(int)(position.x - 20),(int)(position.y + 12));
			break;
	}
}

void EnemyBullet3::Update()
{
	if( tick % 7 == 6 )
	{
		position.x += vx * 2;
		position.y += vy * 2;

		if( position.x < 0 )
			IsAlive = false;

		if( charmap->IsCollediable(position) )
		{
			this->IsAlive = false; 
			return;
		}
	}

	if( tick % 21 == 20 )
	{
	
		AnimationFrame = (AnimationFrame++) % 4;
		if( player->position.x + 100 < position.x )
		{
			this->vx = player->position.x - this->position.x;
			this->vy = player->position.y - this->position.y;
			double d = Distance2D(this->vx, this->vy);
			this->vx /= (float)d;
			this->vy /= (float)d;
		}

	}
	
	if( position.x > player->position.x - 30 && position.x < player->position.x + 30 && 
		position.y > player->position.y - 30 && position.y < player->position.y + 30)
	{
		IsAlive = false;

		if( ShieldLifetime == 0 )
		{
			player->hp -= 5;
			ShieldLifetime = 20;			
			player->ModifyHealth();			
			if( player->hp == 0 )
				player->Death();
		}
	}
}





