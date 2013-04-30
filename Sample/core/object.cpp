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
#include "../vpl/generator.h"
#include "../vpl/generated.h"

void cObject::Death()
{
	if( flow != 0x0 )
	{
		void* node = 0, *f = method_getfirst(flow), *n = f;
		if( f != 0 ) 
		do
		{
			unsigned int id = method_getnodekind(f);		
			if( id == flow::Signal::SymbolId )  
			{
				if( stricmp(method_getnodename(f), "Death") == 0x0 )
				{				
					method_set_global(f, this);
					method_raise(f, 0);				
				}		
			}			
		}
		while( n != (f = method_next(f)) );
	}		
}


void cObject::ModifyHealth()
{
	if( flow != 0x0 )
	{
		void* node = 0, *f = method_getfirst(flow), *n = f;
		if( f != 0 ) 
		do
		{
			unsigned int id = method_getnodekind(f);		
			if( id == flow::Signal::SymbolId )  
			{
				if( stricmp(method_getnodename(f), "HealthChanged") == 0x0 )
				{
					method_set_global(f, this);
					method_raise(f, 0);				
				}		
			}			
		}
		while( n != (f = method_next(f)) );
	}
}

void cObject::LeaveViewport()
{
	if( flow != 0x0 )
	{
		void* node = 0, *f = method_getfirst(flow), *n = f;
		if( f != 0 ) 
		do
		{
			unsigned int id = method_getnodekind(f);		
			if( id == flow::Signal::SymbolId )  
			{
				if( stricmp(method_getnodename(f), "LeaveViewport") == 0x0 )
				{
					method_set_global(f, this);
					method_raise(f, 0);				
				}		
			}			
		}
		while( n != (f = method_next(f)) );
	}
}




void cObject::KeyPressed(int key)
{
	if( flow != 0x0 )
	{
		void* node = 0, *f = method_getfirst(flow), *n = f;
		if( f != 0 ) 
		do
		{
			unsigned int id = method_getnodekind(f);		
			if( id == flow::KeyPressed::SymbolId )  
			{
				//method_raise(f, 1);
				int a = method_getproperty<unsigned int>(f, 0);
				if( key == a ) {
					method_set_global(f, this);
					method_raise(f, 0);
				}					
			}			
		}
		while( n != (f = method_next(f)) );	
	}
}

void cObject::KeyReleased(int key)
{
	if( flow != 0x0 )
	{
		void* node = 0, *f = method_getfirst(flow), *n = f;
		if( f != 0 ) 
		do
		{
			unsigned int id = method_getnodekind(f);		
			if( id == flow::KeyPressed::SymbolId )  
			{
				//method_raise(f, 1);
				int a = method_getproperty<unsigned int>(f, 0);
				if( key == a ) {
					method_set_global(f, this);
					method_raise(f, 1);
				}					
			}			
		}
		while( n != (f = method_next(f)) );
	}
}

