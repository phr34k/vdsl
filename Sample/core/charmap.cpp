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

#include "charmap.h"

Charmap::Charmap(char *a_File)
{
	//Initialize default values
	this->maps = NULL;
	this->tiles = NULL;
	x = 0;
	y = 0;

	//Open the file
	errno_t err;
	FILE* pFile;
	if( (err  = fopen_s( &pFile, a_File, "rb" )) == 0 )
	{
		//Helper variables
		unsigned int MaxTiles = 0;
		unsigned int MaxCharmap = 0;
		Tiledata* prev = NULL;
		Mapdata* prevM = NULL;

		//Read what the buffer size is of the pixel buffer
		fread(&MaxSizeTiles, 1, sizeof(MaxSizeTiles), pFile);
		//Read the maximum tilesize e.g. 16x16
		fread(&TileSize, 1, sizeof(TileSize), pFile);
		//Read the number of tiles
		fread(&MaxTiles, 1, sizeof(MaxTiles), pFile);
		
		//Populate all tile data
		for( unsigned int i = 0; i< MaxTiles; i++ )
		{

			//create a new surface class of 16 x 16 and assign the adjuster pointer 
			//of the master buffer
			Surface* b = new Surface(16, 16);
			b->Clear(0x00);

			//populate the tile with pixel data
			char IsEmpty; 
			fread( &IsEmpty, 1, sizeof(char), pFile);
			fread( b->GetBuffer(), sizeof(Pixel), MaxSizeTiles, pFile);
			
			//Create new tiledata information
			Tiledata* tile = new Tiledata(b, prev, IsEmpty);
			if( prev == NULL ){
				tiles = tile;
			}

			//used for looping
			prev = tile;
		}

		
		//Read the number of charmap entries
		fread(&MaxCharmap, 1, sizeof(MaxCharmap), pFile);

		//Populate with all charmap entries
		for( unsigned int i = 0; i< MaxCharmap; i++ )
		{
			int x, y, index;

			//Read the y value in absolute coordinates
			fread(&y, 1, sizeof(y), pFile);
			//Read the x value in absolute coordinates
			fread(&x, 1, sizeof(x), pFile);
			//Read the tile-index (zero-based)
			fread(&index, 1, sizeof(index), pFile);
			Tiledata* f = FindTiledata(index);
			//Create charmap entry
			Mapdata* d = new Mapdata( f, x,y, prevM);
			//If this is the first entry
			if( prevM == NULL )
			{
				maps = d;
			}

			prevM = d;
		}

		//Close the file
		fclose(pFile);
	}
}


void Charmap::DrawBlend(Surface* buffer)
{
	//Loop over each charmap entry
	Mapdata* current = this->maps;
	while( current != NULL )
	{
		//Only draw if the tile has data this entry is precompiled in the level
		if( current->data->isempty > 0 )
		{
			//Normalise x,y values
			int ax = current->x + x;
			int ay = current->y + y;
			//Used for optimalisations
			int mSize = TileSize - 1;
			int iSize = -mSize;

			//Only draw if the tile is visible
			if( ax >= iSize && ax < SCRWIDTH - iSize && ay >= 0 && ay < SCRHEIGHT - iSize )
			{
				current->data->tile->AddAreaTo(0, 0, mSize, mSize,buffer, ax, ay);
			}
		}
		current = current->next;
	}
}

void Charmap::DrawCopy(Surface* buffer)
{
	//Loop over each charmap entry
	Mapdata* current = this->maps;
	while( current != NULL )
	{
		//Only draw if the tile has data this entry is precompiled in the level
		if( current->data->isempty > 0 )
		{
			//Normalise x,y values
			int ax = current->x + x;
			int ay = current->y + y;
			//Used for optimalisations
			int mSize = TileSize - 1;
			int iSize = -mSize;

			//Only draw if the tile is visible
			if( ax >= iSize && ax < SCRWIDTH - iSize && ay >= 0 && ay < SCRHEIGHT - iSize )
			{
				current->data->tile->CopyAreaTo(0, 0, mSize, mSize,buffer, ax, ay);
			}
		}
		current = current->next;
	}
}

bool Charmap::IsCollediable(Point pos)
{
	//Loop over each charmap entry
	Mapdata* current = this->maps;
	while( current != NULL )
	{
		//Normalise x,y values
		int ax = current->x + x;
		int ay = current->y + y;
		//Used for optimalisations
		int mSize = TileSize - 1;
		//If point is inside the boundingbox of the tile than return true or false
		if( pos.x >= ax && pos.x <= (ax +  mSize) && pos.y >= ay && pos.y <= (ay + mSize) )
			return current->data->isempty > 0;
		current = current->next;
	}

	//Default value is false
	return false;
}

Tiledata* Charmap::FindTiledata(int index)
{
	//Find the tile data on the index
	Tiledata* f = this->tiles;
	//Container for the current tile
	int dTile = 0;
	//Loop over each tile to find it.
	while( f != NULL )
	{	
		if( dTile == index ) return f;
		f = f->next;
		dTile++;
	}

	//Default value is NULL
	return NULL;
}


Tiledata::Tiledata(Surface *surface, Tiledata* previous, char IsEmtpy)
{
	//Initialise default values
	tile = surface;
	isempty = IsEmtpy;
	next = NULL;

	//Auto-assign next iteration
	if( previous != NULL )
	{
		previous->next = this;
	}
}

Tiledata::~Tiledata()
{
	//Only destroy if memory is alive
	if( tile != NULL )
		delete tile;
	if( next != NULL )
		delete next;
}

Mapdata::Mapdata(Tiledata* mData, int rx, int ry, Mapdata* previous)
{
	//Initialise default values
	data = mData;
	x = rx;
	y = ry;
	next = NULL;
	
	//Auto-assign next iteration
	if( previous != NULL )
	{
		previous->next = this;
	}
}