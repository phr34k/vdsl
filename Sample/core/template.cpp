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
#include "../stdafx.h"
#include "delegate.h"
#include "../vpl/generator.h"
#include "../vpl/generated.h"
#include <map>
#include <string>


/////////////////////////////////
// visual programming language
void* mainlevel;
////////////////////////////////

std::map<std::string, void*> classFlowMapping;
Delegate __onKeyDown[255];
Delegate __onKeyRelease[255];
bool __keystates[255];

extern Surface* surface;
Surface* backbuffer;
Surface* beamSurface;
Surface* gameOverGraphics;
Surface* gamePausedGraphics;
Surface* PlayerGraphics1;
Surface* EnemyGraphics1;
Surface* EnemyGraphics2;
Surface* EnemyGraphics3;
Surface* EnemyGraphics4;
Surface* EnemyGraphics5;
Surface* EnemyGraphics6;
Surface* EnemyGraphics7;
Surface* EnemyGraphics8;
Surface* EnemyGraphics9;
Surface* EnemyGraphics10;
Surface* gameMenuGraphics;
Surface* gameWonGraphics;
Font* font;
Charmap* charmap;
Player* player = NULL;

int tick = MAX_MOVEMENTTICK;
int score = 0;
int options = 0;
int current = 0;
int MenuState = 0;
int beamstrength = 0;


bool IsGamePaused = false;
bool CanUpdate = true;
list<cObject*> v;
list<cObject*> enemies;
char name[12];
char ScoreboardNameList[5][12];
int HighscoresScores[5];
void OnReleaseSpace();
void Reset();


int gfx_var_health;
int GameOver = false;
int GameWon = false;


int RandInt( int n1, int n2 ) 
{ 
	return n1 + (rand() * (n2 - n1)) / RAND_MAX; 
};

float Rand( float n1, float n2 ) 
{ 
	return n1 + (rand() * (n2 - n1)) / RAND_MAX; 
};

double Distance2D(double x, double y)
{
	return sqrt( (x * x ) + ( y * y ));
}

bool AddKeyHook( char key, DelegateCallback callback)
{
	return __onKeyDown[key].Add(callback);
}

bool RemoveKeyHook( char key, DelegateCallback callback)
{
	return __onKeyDown[key].Remove(callback);
}

bool IsKeyPressed( char key)
{
	return __keystates[key];
}










////<summary>
////A set of key input callbacks using the new input system
////</summary>
void KeyA() { if( current < 12 ){ name[current] = 'a'; current++; }}
void KeyB() { if( current < 12 ){ name[current] = 'b'; current++; }}
void KeyC() { if( current < 12 ){ name[current] = 'c'; current++; }}
void KeyD() { if( current < 12 ){ name[current] = 'd'; current++; }}
void KeyE() { if( current < 12 ){ name[current] = 'e'; current++; }}
void KeyF() { if( current < 12 ){ name[current] = 'f'; current++; }}
void KeyG() { if( current < 12 ){ name[current] = 'g'; current++; }}
void KeyH() { if( current < 12 ){ name[current] = 'h'; current++; }}
void KeyI() { if( current < 12 ){ name[current] = 'i'; current++; }}
void KeyJ() { if( current < 12 ){ name[current] = 'j'; current++; }}
void KeyK() { if( current < 12 ){ name[current] = 'k'; current++; }}
void KeyL() { if( current < 12 ){ name[current] = 'l'; current++; }}
void KeyM() { if( current < 12 ){ name[current] = 'm'; current++; }}
void KeyN() { if( current < 12 ){ name[current] = 'n'; current++; }}
void KeyO() { if( current < 12 ){ name[current] = 'o'; current++; }}
void KeyP() { if( current < 12 ){ name[current] = 'p'; current++; }}
void KeyQ() { if( current < 12 ){ name[current] = 'q'; current++; }}
void KeyR() { if( current < 12 ){ name[current] = 'r'; current++; }}
void KeyS() { if( current < 12 ){ name[current] = 's'; current++; }}
void KeyT() { if( current < 12 ){ name[current] = 't'; current++; }}
void KeyU() { if( current < 12 ){ name[current] = 'u'; current++; }}
void KeyV() { if( current < 12 ){ name[current] = 'v'; current++; }}
void KeyW() { if( current < 12 ){ name[current] = 'w'; current++; }}
void KeyX() { if( current < 12 ){ name[current] = 'x'; current++; }}
void KeyY() { if( current < 12 ){ name[current] = 'y'; current++; }}
void KeyZ() { if( current < 12 ){ name[current] = 'z'; current++; }}
void KeyBackspacce() { if( current > 0 ){ name[current] = '\0'; current--; }else{ name[current] = '\0'; current = 0;}}

void TogglePause()
{
	IsGamePaused ^= true; 
}


////<summary>
////A reset function to reset everything to default settings
////</summary>
void KeyPressed(int key)
{
	//Find all keypressed references
	{
		extern void* mainlevel;
		void* node = 0, *f = method_getfirst(mainlevel), *n = f;
		if( f != 0 ) 
		do
		{
			unsigned int id = method_getnodekind(f);		
			if( id == flow::KeyPressed::SymbolId )  
			{
				int a = method_getproperty<unsigned int>(f, 0);
				if( key == a ) {
					method_raise(f, 0);
				}					
			}			
		}
		while( n != (f = method_next(f)) );
	}

	if( player != NULL )
	{
		player->KeyPressed(key);
	}

	for ( list<cObject*>::iterator it=enemies.begin() ; it != enemies.end(); it++ )
	{
		cObject* obj = *it;
		obj->KeyPressed(key);
	}
}

void KeyReleased(int key)
{
	//Find all keypressed references
	{
		extern void* mainlevel;
		void* node = 0, *f = method_getfirst(mainlevel), *n = f;
		if( f != 0 ) 
		do
		{
			unsigned int id = method_getnodekind(f);		
			if( id == flow::KeyPressed::SymbolId )  
			{
				//method_raise(f, 1);
				int a = method_getproperty<unsigned int>(f, 0);
				if( key == a ) {
					method_raise(f, 1);
				}					
			}			
		}
		while( n != (f = method_next(f)) );
	}


	if( player != NULL )
	{
		player->KeyReleased(key);
	}

	for ( list<cObject*>::iterator it=enemies.begin() ; it != enemies.end(); it++ )
	{
		cObject* obj = *it;
		obj->KeyReleased(key);
	}
}

void Reset()
{
	if( player != NULL )
		delete player;
	//player = new Player();
	//player->position.y = SCRHEIGHT / 2;
	//player->flow = classFlowMapping["player"];

	for ( list<cObject*>::iterator it=enemies.begin() ; it != enemies.end(); it++ )
	{
		cObject* obj = *it;
		delete obj;
	}

	for ( list<cObject*>::iterator it=v.begin() ; it != v.end(); it++ )
	{
		cObject* obj = *it;
		delete obj;
	}

	v.clear();
	enemies.clear();


	void* node = 0;
	if( 0 == (node = method_getbyname(mainlevel, "Begin"))) {
		printf("Entry point was not found in the executable\r\n");
	} else {
		method_raise(node, 0);
	}
	
	charmap->x = 0;
	charmap->y = 0;
	CanUpdate = true;
	beamstrength = 0;
}







void Init()
{	
	/* initialize random seed: */
	srand (time(NULL));	

	/* pre-generate a bunch of numbers as a hack for creating numbers on consecutive application startups */
	for( int i = 0; i < 20; i++ ) rand();	

	extern void* loadvpl(void* global, int argc, char* argv);
	mainlevel = loadvpl(0, 2, "level.xml");
	classFlowMapping["player"] = loadvpl(0, 2, "player.xml");
	classFlowMapping["enemy_01"] = loadvpl(0, 2, "enemy_01.xml");
	classFlowMapping["enemy_02"] = loadvpl(0, 2, "enemy_02.xml");
	classFlowMapping["enemy_03"] = loadvpl(0, 2, "enemy_03.xml");
	classFlowMapping["enemy_04"] = loadvpl(0, 2, "enemy_04.xml");
	classFlowMapping["enemy_05"] = loadvpl(0, 2, "enemy_05.xml");
	classFlowMapping["enemy_06"] = loadvpl(0, 2, "enemy_06.xml");
	classFlowMapping["enemy_07"] = loadvpl(0, 2, "enemy_07.xml");
	classFlowMapping["enemy_08"] = loadvpl(0, 2, "enemy_08.xml");
	classFlowMapping["enemy_09"] = loadvpl(0, 2, "enemy_09.xml");
	classFlowMapping["enemy_10"] = loadvpl(0, 2, "enemy_10.xml");

	
	backbuffer = new Surface( surface->GetWidth(), surface->GetHeight() );
	backbuffer->Clear( 0 );
	backbuffer->InitCharset();

	tick = MAX_MOVEMENTTICK;

	beamSurface = new Surface("Images/ui/beam.tga");
	gameOverGraphics = new Surface("Images/ui/gameover.tga");
	gamePausedGraphics = new Surface("Images/ui/gamepaused.tga"); 
	PlayerGraphics1 = new Surface("Images/playersheet.tga");
	EnemyGraphics1 = new Surface("Images/enemysheet.tga");
	EnemyGraphics2 = new Surface("Images/enemysheet1.tga");
	EnemyGraphics3 =  new Surface("Images/enemysheet2.tga");
	EnemyGraphics4 = new Surface("Images/enemysheet3.tga");
	EnemyGraphics5 = new Surface("Images/enemysheet4.tga");
	EnemyGraphics6 = new Surface("Images/enemysheet5.tga");
	EnemyGraphics7 = new Surface("Images/enemysheet6.tga");
	EnemyGraphics8 = new Surface("Images/enemysheet7.tga");
	EnemyGraphics9 = new Surface("Images/enemysheet8.tga");
	EnemyGraphics10 = new Surface("Images/enemysheet9.tga");
	gameMenuGraphics = new Surface("Images/ui/gamemenu.tga");
	gameWonGraphics = new Surface("Images/ui/gamewon.tga");
	charmap = new Charmap("level.uav");
	font = new Font("Images/ui/font.tga", "abcdefghijklmnopqrstuvwxyz0123456789");


	//Add key hooks for menu display
	//AddKeyHook( key_down, &ToggleMenuDown);
	//AddKeyHook( key_up, &ToggleMenuUp);
	//AddKeyHook( key_space, &PressEnter);


	//Open the file
	errno_t err;
	FILE* pFile;
	if( (err  = fopen_s( &pFile, "highscores.cni", "rb" )) == 0 )
	{
		fread(  (char*)(&ScoreboardNameList),  sizeof(char), 60, pFile);
		fread(  HighscoresScores,  sizeof(int), 5, pFile);

		//Close the file
		fclose(pFile);
	}
}



////<summary>
////Helpers functions to process the AI
////</summary>
void ProcessBullets()
{
	//Do bullets
	list<cObject*>::iterator it;
	list<cObject*> removeList;
	for ( it=v.begin() ; it != v.end(); it++ )
	{
		cObject* obj = *it;
		if( obj->IsActivated == false  && CanUpdate == true )
		{
			obj->position.x-= ScrollingSpeed;
			if( obj->position.x < SCRWIDTH + 10 )
				obj->IsActivated = true;
		}

		if( obj->IsActivated == true )
		{
			
			if(  CanUpdate == true )
			obj->Update();

			if( obj->IsAlive )
				obj->Draw(backbuffer);
			else
			{
				removeList.push_back(obj);
			}
		}
	}

	for ( it=removeList.begin() ; it != removeList.end(); it++ )
	{
		cObject* obj = *it;
		v.remove(obj);
		delete obj;
	}
}


void ProcessEnemies2()
{
	list<cObject*>::iterator it;
	for ( it=enemies.begin() ; it != enemies.end(); it++ )
	{
		cObject* obj = *it;
		obj->position.x -= ScrollingSpeed;

		if( obj->position.x < SCRWIDTH + 200 )
			obj->IsActivated = true; 	
	}
}

void ProcessEnemies()
{
	//Do bullets
	list<cObject*>::iterator it;
	list<cObject*> removeList;
	for ( it=enemies.begin() ; it != enemies.end(); it++ )
	{
		cObject* obj = *it;
		if( obj->IsActivated == true && CanUpdate == true)
			obj->Update();
		if( obj->IsAlive )
			obj->Draw(backbuffer);
		else
		{
			removeList.push_back(obj);
		}
	}

	for ( it=removeList.begin() ; it != removeList.end(); it++ )
	{
		cObject* obj = *it;
		enemies.remove(obj);
		delete obj;
	}
}





////<summary>
////Render functions
////</summary>
void DrawGauge(int x, int i, int str, Pixel color)
{
	int w = (int)(((float)str / (float)100) * 182);
	backbuffer->Bar(x + 12,i + 4,x + w,i + 15, color);
	beamSurface->AddAreaTo(0,0,193,18,backbuffer,x,i);
}



void RenderStartMenu()
{
	backbuffer->Clear( 0 );

	int __y = 0;
	int __y2 = 0;
	for( int i = 0; i < 2; i++)
	{
		if( options != i )
		{
			gameMenuGraphics->AddAreaTo(0,__y,282, __y + 45, backbuffer, (SCRWIDTH - gameMenuGraphics->GetWidth()  )/ 2,(SCRHEIGHT - 45)/ 2 + __y2);
		}
		else
		{
			gameMenuGraphics->AddAreaTo(0,__y + 45 ,282, __y + 90, backbuffer, (SCRWIDTH - gameMenuGraphics->GetWidth()  )/ 2,(SCRHEIGHT - 45)/ 2 + __y2);
		}

		__y += 90;
		__y2 += 45;
	}

	backbuffer->CopyTo(surface,0,0);
}

void RenderGame()
{
	bool wasInvisible = player ? player->position.x < -50 : true;
	CanUpdate = GameOver == false && IsGamePaused == false;
	backbuffer->Clear( 0 );

	//Draw the background
	charmap->DrawCopy(backbuffer);


	//Update all entities and the background (horizontal scrolling)
	if( tick % 24 == 23 && charmap->x > -3228 && CanUpdate)
	{
		charmap->x-= ScrollingSpeed;

		if( player )
		{
			Point p2 = player->position;
			p2.x += 30;
			Point p1 = p2;
			p1.y -= 5;
			p2.y += 5;
			if( charmap->IsCollediable(p1) || charmap->IsCollediable(p2) )
			{
				player->position.x-= ScrollingSpeed;
			}
		}		

		ProcessEnemies2();
	}

	//Update the player
	if( player && player->IsAlive )
	{
		if( CanUpdate )
			player->Update();
		player->Draw(backbuffer);
	}
	/*
	else
	{
		GameOver = true;
	}
	*/
	
	//Update enemies
	ProcessEnemies();
	//Update bullets
	ProcessBullets();

	//Draw beamer-strength bar
	DrawGauge(150,260, beamstrength, 0xFFFF);
	//Draw health bar
	DrawGauge(350,260, gfx_var_health, 0x0ADF);
	//Draw scores

	char buffer[30];
	sprintf_s(buffer,"%d", score);
	font->Print(backbuffer, buffer, 140 - font->Width(buffer),253);

	
	//If we get stuck behind a tile we're gameover
	if( wasInvisible && player && player->position.x < -50 ) 
	{
		player->LeaveViewport();
	}
	
	//Draw some fonts in special occasions
	if( GameOver == true )
	{
		gameOverGraphics->AddAreaTo(0,0,gameOverGraphics->GetWidth() - 1, gameOverGraphics->GetHeight() - 1, backbuffer, (SCRWIDTH - gameOverGraphics->GetWidth()  )/ 2,(SCRHEIGHT - gameOverGraphics->GetHeight() )/ 2);
	}
	//If you die while the your ship is leaving the stage you won't be winning so much afterall.
	else if( GameWon == true )
	{
		gameWonGraphics->AddAreaTo(0,0,gameWonGraphics->GetWidth() - 1, gameWonGraphics->GetHeight() - 1, backbuffer, (SCRWIDTH - gameWonGraphics->GetWidth()  )/ 2,(SCRHEIGHT - gameWonGraphics->GetHeight() )/ 2);
	}
	else if( IsGamePaused == true )
	{
		gamePausedGraphics->AddAreaTo(0,0,gamePausedGraphics->GetWidth() - 1, gamePausedGraphics->GetHeight() - 1, backbuffer, (SCRWIDTH - gamePausedGraphics->GetWidth()  )/ 2,(SCRHEIGHT - gamePausedGraphics->GetHeight() )/ 2);
	}

	backbuffer->CopyTo(surface,0,0);

}






void RenderScoretable()
{

	backbuffer->Clear( 0 );

	char* text = "lawrence\0\0\0";
	char buffer[10];
	char buffer2[10];
	

	int y = 75;
	int x = (SCRWIDTH - 500) / 2;
	for( int i = 0; i < 5; i++ )
	{
		font->Print(backbuffer,ScoreboardNameList[i],x + 50,y);
		sprintf_s(buffer, "%d", HighscoresScores[i] );
		font->Print(backbuffer,buffer,x + 500 - font->Width(buffer),y); 
		sprintf_s(buffer2, "%d", i + 1 );
		font->Print(backbuffer,buffer2,x,y); 

		y += 30;
	}

	char* b = "highscores";
	font->Print(backbuffer,b, (SCRWIDTH - font->Width(b)) / 2 ,30);
	backbuffer->CopyTo(surface,0,0);

}


void RenderSetScore()
{
	backbuffer->Clear( 0 );
	char* b = "enter  name";
	font->Print(backbuffer,b, (SCRWIDTH - font->Width(b)) / 2 ,30);
	font->Print(backbuffer,name, (SCRWIDTH - font->Width(b)) / 2 ,80);

	backbuffer->CopyTo(surface,0,0);

}





bool Tick()
{
	//Gametick
	tick--;

	
	if( MenuState == 0 )
	{
		RenderStartMenu();
	}
	else if( MenuState == 1)
	{
		RenderGame();
	}
	else if( MenuState == 2 )
	{
		RenderScoretable();
	}
	else if( MenuState == 3 )
	{
		RenderSetScore();
	}

	//Do gametick	
	if( tick <= 0 )
		tick = MAX_MOVEMENTTICK;

	return true;
}

