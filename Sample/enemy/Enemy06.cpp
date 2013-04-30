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

//enemy 5
Enemy5::Enemy5()
{
	direction = 0;
	AimFrame = 0;
}

void Enemy5::Update()
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

		if( tick % 31 == 30 )
		{
			if( !player )
			{

			}
			else if( player->position.x + 130 < position.x )
			{
				if( AimFrame == 0 )
				{
					Point p = position;
					p.x--;
					p.y += 28;
					if( !charmap->IsCollediable(p))
					position.x--;
				}
				state = 0;
			}
			else if( player->position.x - 130 > position.x )
			{
				if( AimFrame == 0 )
				{
					Point p = position;
					p.x++;
					p.y += 28;
					if( !charmap->IsCollediable(p))
					position.x++;
				}
				state = 0;
			}
			else
			{
				state = 1;
			}
		}

		direction = player ? (player->position.x < position.x) : 0;

		if( state == 0 && tick % 32 == 31 )
		{
			Frame = (Frame + 1 ) % 6;
			if( AimFrame > 0 )
				AimFrame--;
		} 
		else if( state == 1 && tick % 32 == 31 )
		{
			if( bulletLifetime > 0 )
				bulletLifetime--;
			else 
			{
				if( AimFrame == 1)
				{
					Shoot();
					AimFrame++;
				}
				else if( AimFrame > 1 )
				{
					AimFrame++;
					if( AimFrame == 4 )
						AimFrame = 1;
				}
				else
				{
					AimFrame++;
				}
			}

		}

}

void Enemy5::Shoot()
{
	bulletLifetime = 10;

	EnemyBullet2* bullet = new EnemyBullet2();
	bullet->position = position;
	bullet->position.x += 20;
	bullet->position.y -= 10;
	bullet->vx = player->position.x - bullet->position.x;
	bullet->vy = player->position.y - bullet->position.y;
	double d = Distance2D(bullet->vx, bullet->vy);
	bullet->vx /= (float)d;
	bullet->vy /= (float)d;

	v.push_back(bullet);
}

bool Enemy5::IsColliding(Bullet* bullet)
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


void Enemy5::Draw(Surface *surface)
{
    if( IsExploding )
	{
		switch( ExplosionFrame )
		{
			case 0:
				PlayerGraphics1->PartialCopyAreaTo(265,289,283,330, surface,(int)position.x ,(int)position.y);
				break;
			case 1:
				PlayerGraphics1->PartialCopyAreaTo(246,289,264,330, surface,(int)position.x ,(int)position.y);
				break;
			case 2:
				PlayerGraphics1->PartialCopyAreaTo(214,289,245,330, surface,(int)position.x ,(int)position.y);
				break;
			case 3:
				PlayerGraphics1->PartialCopyAreaTo(179,289,213,330, surface,(int)position.x ,(int)position.y);
				break;
			case 4:
				PlayerGraphics1->PartialCopyAreaTo(142,289,178,330, surface,(int)position.x ,(int)position.y);
				break;
			case 5:
				PlayerGraphics1->PartialCopyAreaTo(106,289,142,330, surface,(int)position.x ,(int)position.y);
				break;
			case 6:
				PlayerGraphics1->PartialCopyAreaTo(68,289,105,330, surface,(int)position.x ,(int)position.y);
				break;
			default:
				this->IsAlive = false;
				//score += 45;
				break;
		}
		return;
	}





	if( direction == 0 )
	{
		if( state == 0 )
		{
			switch( Frame )
			{
				case 0:
					EnemyGraphics6->PartialCopyAreaTo(363,0,397,27, surface, (int)position.x,(int)position.y);
					break;
				case 1:
					EnemyGraphics6->PartialCopyAreaTo(330,0,363,27, surface, (int)position.x,(int)position.y);
					break;
				case 2:
					EnemyGraphics6->PartialCopyAreaTo(297,0,330,27, surface, (int)position.x,(int)position.y);
					break;
				case 3:
					EnemyGraphics6->PartialCopyAreaTo(264,0,297,27, surface, (int)position.x,(int)position.y);
					break;
				case 4:
					EnemyGraphics6->PartialCopyAreaTo(231,0,264,27, surface, (int)position.x,(int)position.y);
					break;
				case 5:
					EnemyGraphics6->PartialCopyAreaTo(198,0,231,27, surface, (int)position.x,(int)position.y);
					break;
			}
		}
		else
		{
			switch( AimFrame )
			{
				case 0:
					EnemyGraphics6->PartialCopyAreaTo(198,28,231,60, surface, (int)position.x,(int)position.y);
					break;
				case 1:
					EnemyGraphics6->PartialCopyAreaTo(231,28,264,60, surface, (int)position.x,(int)position.y);
					break;
				case 2:
					EnemyGraphics6->PartialCopyAreaTo(264,28,297,60, surface, (int)position.x,(int)position.y);
					break;
				case 3:
					EnemyGraphics6->PartialCopyAreaTo(297,28,330,60, surface, (int)position.x,(int)position.y);
					break;
			}
		}

	}
	else
	{
		if( state == 0 )
		{
			switch( Frame )
			{
				case 0:
					EnemyGraphics6->PartialCopyAreaTo(165,0,198,27, surface, (int)position.x,(int)position.y);
					break;
				case 1:
					EnemyGraphics6->PartialCopyAreaTo(132,0,165,27, surface, (int)position.x,(int)position.y);
					break;
				case 2:
					EnemyGraphics6->PartialCopyAreaTo(99,0,132,27, surface, (int)position.x,(int)position.y);
					break;
				case 3:
					EnemyGraphics6->PartialCopyAreaTo(66,0,99,27, surface, (int)position.x,(int)position.y);
					break;
				case 4:
					EnemyGraphics6->PartialCopyAreaTo(33,0,66,27, surface, (int)position.x,(int)position.y);
					break;
				case 5:
					EnemyGraphics6->PartialCopyAreaTo(0,0,33,27, surface, (int)position.x,(int)position.y);
					break;
			}
		}
		else
		{
			switch(AimFrame)
			{
				case 0:
					EnemyGraphics6->PartialCopyAreaTo(165,28,198,60, surface, (int)position.x,(int)position.y);
					break;
				case 1:
					EnemyGraphics6->PartialCopyAreaTo(132,28,165,60, surface, (int)position.x,(int)position.y);
					break;
				case 2:
					EnemyGraphics6->PartialCopyAreaTo(99,28,132,60, surface, (int)position.x,(int)position.y);
					break;
				case 3:
					EnemyGraphics6->PartialCopyAreaTo(66,28,99,60, surface, (int)position.x,(int)position.y);
					break;
			}
		}
	}
}