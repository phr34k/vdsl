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

#include <stdio.h>
#include <malloc.h>
#include <string>
#include <map>
#include <math.h>
#include <time.h>
#include <windows.h>
#include "Generator.h"
#include "Generated.h"
#include "../stdafx.h"
#include "../core/delegate.h"

extern std::map<std::string, void*> classFlowMapping;

namespace flow
{
	template<typename T, int N> class Vector
	{
	public:
		union
		{
			T V[N];
		};

		Vector& operator = (const Vector& Source) {
			for( int i = 0; i < N; i++ ) 
				V[i] = Source.V[i];
			return *this;
		}
	};
}


void __cdecl method_releasetoken(void* a, unsigned int token)
{
	//Releasing token	
	unsigned int type = method_gettokentype( a, token );
	if( type == 1 ) 
	{		
		static int l = 0;
		////printf("Delete string %d\r\n", ++l);
		delete (char*)method_gettoken(a, token);
	}
}

//Method reciptents
void __cdecl method_preexecute_random_real32(void* a, int signal)
{		
	float min = method_getproperty<float>(a, 1), max = method_getproperty<float>(a, 2);
	float v = (float)(((double)rand() / (double)RAND_MAX) * (max - min)) + min;
	method_setproperty<float>(a, 0) = v;
	//printf("RFLOAT:%f\r\n", v);
}

void __cdecl method_preexecute_random_int32(void* a, int signal)
{	
	int min = method_getproperty<int>(a, 1), max = method_getproperty<int>(a, 2);
	int v = (int)(((double)rand() / (double)RAND_MAX) * (max - min)) + min;
	method_setproperty<int>(a, 0) = v;
	//printf("RINT:%d\r\n", v);
}

void __cdecl method_missing(void* a, int signal)
{
	printf("method_missing %d\r\n", method_getnodekind(a));
}

void __cdecl method_comparison_bool(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<bool>(a, 0) = method_getproperty<bool>(a, 0);
	method_setproperty<bool>(a, 1) = method_getproperty<bool>(a, 1);
	//printf("CBOOL:A %s\r\n", method_setproperty<bool>(a, 0) ? "True" : "False");
	//printf("CBOOL:B %s\r\n", method_setproperty<bool>(a, 1) ? "True" : "False");
	if( method_setproperty<bool>(a, 0) == method_setproperty<bool>(a, 1) ) {
		method_raise(a, 1);
	} else {
		method_raise(a, 0);		
	}	
}

void __cdecl method_comparison_int32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
	method_setproperty<int>(a, 1) = method_getproperty<int>(a, 1);
	//printf("CINT32:A %d\r\n", method_setproperty<int>(a, 0));
	//printf("CINT32:B %d\r\n", method_setproperty<int>(a, 1));
	if( method_setproperty<int>(a, 0) <= method_setproperty<int>(a, 1) ) {
		//printf("Cint: A <= B\r\n");
		method_raise(a, 0);
	} 
	
	if( method_setproperty<int>(a, 0) > method_setproperty<int>(a, 1) ) {
		//printf("Cint: A > B\r\n");
		method_raise(a, 1);		
	} 
	
	if( method_setproperty<int>(a, 0) == method_setproperty<int>(a, 1) ) {
		//printf("Cint: A == B\r\n");
		method_raise(a, 2);		
	} 

	if( method_setproperty<int>(a, 0) != method_setproperty<int>(a, 1) ) {
		//printf("Cint: A != B\r\n");
		method_raise(a, 3);		
	} 
	
	if( method_setproperty<int>(a, 0) < method_setproperty<int>(a, 1) ) {
		//printf("Cint: A < B\r\n");
		method_raise(a, 4);		
	} 
	
	if( method_setproperty<int>(a, 0) >= method_setproperty<int>(a, 1) ) {
		//printf("Cint: A >= B\r\n");
		method_raise(a, 5);		
	}	
}

void __cdecl method_comparison_float(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	//printf("CFLOAT:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("CFLOAT:B %f\r\n", method_setproperty<float>(a, 1));
	if( method_setproperty<float>(a, 0) <= method_setproperty<float>(a, 1) ) {
		//printf("CFLOAT: A <= B\r\n");
		method_raise(a, 0);
	} 
	
	if( method_setproperty<float>(a, 0) > method_setproperty<float>(a, 1) ) {
		//printf("CFLOAT: A > B\r\n");
		method_raise(a, 1);		
	} 
	
	if( method_setproperty<float>(a, 0) == method_setproperty<float>(a, 1) ) {
		//printf("CFLOAT: A == B\r\n");
		method_raise(a, 2);		
	} 

	if( method_setproperty<float>(a, 0) != method_setproperty<float>(a, 1) ) {
		//printf("CFLOAT: A != B\r\n");
		method_raise(a, 2);		
	} 
	
	if( method_setproperty<float>(a, 0) < method_setproperty<float>(a, 1) ) {
		//printf("CFLOAT: A < B\r\n");
		method_raise(a, 3);		
	} 
	
	if( method_setproperty<float>(a, 0) >= method_setproperty<float>(a, 1) ) {
		//printf("CFLOAT: A >= B\r\n");
		method_raise(a, 4);		
	}	
}

void __cdecl method_increment_and_compare_int32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
	method_setproperty<int>(a, 1) = method_getproperty<int>(a, 1);
	method_setproperty<int>(a, 2) = method_getproperty<int>(a, 2);
	method_setproperty<int>(a, 3) = method_getproperty<int>(a, 0) + method_getproperty<int>(a, 2);

	//printf("ICINT32:A %d\r\n", method_setproperty<int>(a, 3));
	//printf("ICINT32:B %d\r\n", method_setproperty<int>(a, 1));
	//printf("ICFLOAT:I %d\r\n", method_setproperty<int>(a, 2));
	//printf("ICFLOAT:R %d\r\n", method_setproperty<int>(a, 3));
	if( method_setproperty<int>(a, 3) <= method_setproperty<int>(a, 1) ) {
		//printf("ICint: R <= B\r\n");
		method_raise(a, 0);
	} 
	
	if( method_setproperty<int>(a, 3) > method_setproperty<int>(a, 1) ) {
		//printf("ICint: R > B\r\n");
		method_raise(a, 1);		
	} 
	
	if( method_setproperty<int>(a, 3) == method_setproperty<int>(a, 1) ) {
		//printf("ICint: R == B\r\n");
		method_raise(a, 2);		
	} 
	
	if( method_setproperty<int>(a, 3) < method_setproperty<int>(a, 1) ) {
		//printf("ICint: R < B\r\n");
		method_raise(a, 3);		
	} 
	
	if( method_setproperty<int>(a, 3) >= method_setproperty<int>(a, 1) ) {
		//printf("ICint: R >= B\r\n");
		method_raise(a, 4);		
	}	
}

void __cdecl method_increment_and_compare_float(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	method_setproperty<float>(a, 2) = method_getproperty<float>(a, 2);
	method_setproperty<float>(a, 3) = method_getproperty<float>(a, 0) + method_getproperty<float>(a, 2);
	//printf("ICFLOAT:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("ICFLOAT:B %f\r\n", method_setproperty<float>(a, 1));
	//printf("ICFLOAT:I %f\r\n", method_setproperty<float>(a, 2));
	//printf("ICFLOAT:R %f\r\n", method_setproperty<float>(a, 3));
	if( method_setproperty<float>(a, 3) <= method_setproperty<float>(a, 1) ) {
		//printf("ICFLOAT: R <= B\r\n");
		method_raise(a, 0);
	} 
	
	if( method_setproperty<float>(a, 3) > method_setproperty<float>(a, 1) ) {
		//printf("ICFLOAT: R > B\r\n");
		method_raise(a, 1);		
	} 
	
	if( method_setproperty<float>(a, 3) == method_setproperty<float>(a, 1) ) {
		//printf("ICFLOAT: R == B\r\n");
		method_raise(a, 2);		
	} 
	
	if( method_setproperty<float>(a, 3) < method_setproperty<float>(a, 1) ) {
		//printf("ICFLOAT: R < B\r\n");
		method_raise(a, 3);		
	} 
	
	if( method_setproperty<float>(a, 3) >= method_setproperty<float>(a, 1) ) {
		//printf("ICFLOAT: R >= B\r\n");
		method_raise(a, 4);		
	}	
}


//Integer arthimetics



void __cdecl method_cycle_int32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
	method_setproperty<int>(a, 1) = method_getproperty<int>(a, 1);
	method_setproperty<int>(a, 2) = method_getproperty<int>(a, 2);


	if( method_setproperty<int>(a, 2) > 0 )
	{
		method_setproperty<int>(a, 3) = method_setproperty<int>(a, 1) + method_getproperty<int>(a, 0);
		while( method_setproperty<int>(a, 3) < 0 )
			method_setproperty<int>(a, 3) += method_setproperty<int>(a, 2);
		method_setproperty<int>(a, 3) %= method_setproperty<int>(a, 2);
	}
	else
	{
		method_setproperty<int>(a, 3) = 0;
	}

	//printf("CYCINT32:A %d\r\n", method_setproperty<int>(a, 0));
	//printf("CYCINT32:B %d\r\n", method_setproperty<int>(a, 1));
	//printf("CYCINT32:C %d\r\n", method_setproperty<int>(a, 2));
	//printf("CYCINT32:D %d\r\n", method_setproperty<int>(a, 3));
	method_raise(a, 0);	
}

void __cdecl method_add_int32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
	method_setproperty<int>(a, 1) = method_getproperty<int>(a, 1);
	method_setproperty<int>(a, 2) = method_setproperty<int>(a, 0) + method_setproperty<int>(a, 1);
	//printf("AINT32:A %d\r\n", method_setproperty<int>(a, 0));
	//printf("AINT32:B %d\r\n", method_setproperty<int>(a, 1));
	//printf("AINT32:C %d\r\n", method_setproperty<int>(a, 2));
	method_raise(a, 0);	
}

void __cdecl method_subtract_int32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
	method_setproperty<int>(a, 1) = method_getproperty<int>(a, 1);
	method_setproperty<int>(a, 2) = method_setproperty<int>(a, 0) - method_setproperty<int>(a, 1);
	//printf("SINT32:A %d\r\n", method_setproperty<int>(a, 0));
	//printf("SINT32:B %d\r\n", method_setproperty<int>(a, 1));
	//printf("SINT32:C %d\r\n", method_setproperty<int>(a, 2));
	method_raise(a, 0);	
}

void __cdecl method_multiply_int32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
	method_setproperty<int>(a, 1) = method_getproperty<int>(a, 1);
	method_setproperty<int>(a, 2) = method_setproperty<int>(a, 0) * method_setproperty<int>(a, 1);
	//printf("MINT32:A %d\r\n", method_setproperty<int>(a, 0));
	//printf("MINT32:B %d\r\n", method_setproperty<int>(a, 1));
	//printf("MINT32:C %d\r\n", method_setproperty<int>(a, 2));
	method_raise(a, 0);	
}

void __cdecl method_divide_int32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
	method_setproperty<int>(a, 1) = method_getproperty<int>(a, 1);
	if( method_setproperty<int>(a, 1) != 0 )
		method_setproperty<int>(a, 2) = method_setproperty<int>(a, 0) / method_setproperty<int>(a, 1);
	else
		method_setproperty<int>(a, 2) = 0;
	//printf("DINT32:A %d\r\n", method_setproperty<int>(a, 0));
	//printf("DINT32:B %d\r\n", method_setproperty<int>(a, 1));
	//printf("DINT32:C %d\r\n", method_setproperty<int>(a, 2));
	method_raise(a, 0);	
}

//Float arthimetics
void __cdecl method_add_real32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	method_setproperty<float>(a, 2) = method_setproperty<float>(a, 0) + method_setproperty<float>(a, 1);
	//printf("AREAL32:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("AREAL32:B %f\r\n", method_setproperty<float>(a, 1));
	//printf("AREAL32:C %f\r\n", method_setproperty<float>(a, 2));
	method_raise(a, 0);	
}

void __cdecl method_subtract_real32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	method_setproperty<float>(a, 2) = method_setproperty<float>(a, 0) - method_setproperty<float>(a, 1);
	//printf("SREAL32:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("SREAL32:B %f\r\n", method_setproperty<float>(a, 1));
	//printf("SREAL32:C %f\r\n", method_setproperty<float>(a, 2));
	method_raise(a, 0);	
}

void __cdecl method_multiply_real32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	method_setproperty<float>(a, 2) = method_setproperty<float>(a, 0) * method_setproperty<float>(a, 1);
	//printf("MREAL32:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("MREAL32:B %f\r\n", method_setproperty<float>(a, 1));
	//printf("MREAL32:C %f\r\n", method_setproperty<float>(a, 2));
	method_raise(a, 0);	
}

void __cdecl method_divide_real32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	if( method_setproperty<float>(a, 1) != 0 )
		method_setproperty<float>(a, 2) = method_setproperty<float>(a, 0) / method_setproperty<float>(a, 1);
	else
		method_setproperty<int>(a, 2) = 0;
	//printf("DREAL32:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("DREAL32:B %f\r\n", method_setproperty<float>(a, 1));
	//printf("DREAL32:C %f\r\n", method_setproperty<float>(a, 2));
	method_raise(a, 0);	
}


//Math
void __cdecl method_sin(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = sin(method_setproperty<float>(a, 0));
	//printf("SIN:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("SIN:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_cos(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = cos(method_setproperty<float>(a, 0));
	//printf("COS:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("COS:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_tan(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = tan(method_setproperty<float>(a, 0));
	//printf("TAN:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("TAN:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_asin(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = asin(method_setproperty<float>(a, 0));
	//printf("ASIN:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("ASIN:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_acos(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = acos(method_setproperty<float>(a, 0));
	//printf("ACOS:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("ACOS:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_atan(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = atan(method_setproperty<float>(a, 0));
	//printf("ATAN:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("ATAN:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}



void __cdecl method_sinh(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = sinh(method_setproperty<float>(a, 0));
	//printf("SINH:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("SINH:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_cosh(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = cosh(method_setproperty<float>(a, 0));
	//printf("COSH:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("COSH:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_tanh(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = tanh(method_setproperty<float>(a, 0));
	//printf("TANH:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("TANH:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}


void __cdecl method_min(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	method_setproperty<float>(a, 2) = method_getproperty<float>(a, 0) < method_setproperty<float>(a, 1) ? method_getproperty<float>(a, 0) : method_getproperty<float>(a, 1);
	//printf("MIN:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("MIN:B %f\r\n", method_setproperty<float>(a, 1));
	//printf("MIN:R %f\r\n", method_setproperty<float>(a, 2));
	method_raise(a, 0);	
}

void __cdecl method_max(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	method_setproperty<float>(a, 2) = method_getproperty<float>(a, 0) > method_setproperty<float>(a, 1) ? method_getproperty<float>(a, 0) : method_getproperty<float>(a, 1);
	//printf("MAX:A %f\r\n", method_setproperty<float>(a, 0));
	//printf("MAX:B %f\r\n", method_setproperty<float>(a, 1));
	//printf("MAX:R %f\r\n", method_setproperty<float>(a, 2));
	method_raise(a, 0);	
}

//Others
void __cdecl method_gate(void* a, int signal)
{
	if( (method_getproperty<int>(a, 0))++ == 5 )
	{
		method_getproperty<int>(a, 0) = 0;		
		//printf("Disallow gate\r\n");
		return;
	}
	else
	{
		//printf("Allow gate\r\n");
		method_raise(a, 0);
	}
}

void __cdecl method_assert(void* a, int signal)
{	
	method_setproperty<unsigned int>(a, 0) = method_getproperty<unsigned int>(a, 0);
	printf("Assert: %s\r\n", (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 0)));
	method_raise(a, 0);	
}

void __cdecl method_assert2(void* a, int signal)
{		
	struct helper
	{
		static bool equals(const char* r, const char* a, const char* b)
		{
			int lengtha = strlen(a);
			int lengthb = strlen(b);	
			char* ret = (char*)alloca(lengtha + lengthb + 1);
			strcpy(ret, a);
			strcpy(ret + lengtha, b);
			ret[lengtha + lengthb] = 0;
			return strcmp(ret, r) == 0x0;
		}
	};

	bool equalsa = strcmp( (char*)method_gettoken(a,method_setproperty<unsigned int>(a, 0)), (char*)method_gettoken(a,method_getproperty<unsigned int>(a, 0)) ) == 0x0;
	bool equalsb = strcmp( (char*)method_gettoken(a,method_setproperty<unsigned int>(a, 1)), (char*)method_gettoken(a,method_getproperty<unsigned int>(a, 1)) ) == 0x0;
	method_compare_and_swap_token(a, 0, &method_releasetoken );
	method_compare_and_swap_token(a, 1, &method_releasetoken );

	if( !(equalsa && equalsb) || !helper::equals((char*)method_gettoken(a,method_setproperty<unsigned int>(a, 2)), (char*)method_gettoken(a,method_setproperty<unsigned int>(a, 0)), (char*)method_gettoken(a,method_setproperty<unsigned int>(a, 1))) ) 
	{
		int lengtha = strlen((char*)method_gettoken(a,method_setproperty<unsigned int>(a, 0)));
		int lengthb = strlen((char*)method_gettoken(a,method_setproperty<unsigned int>(a, 1)));	
		char* value = new char[lengtha + lengthb + 1];
		strcpy(value, (char*)method_gettoken(a,method_setproperty<unsigned int>(a, 0)));
		strcpy(value + lengtha, (char*)method_gettoken(a,method_setproperty<unsigned int>(a, 1)));
		value[lengtha + lengthb] = 0;
		method_compare_and_set_token(a, 2, method_createtoken(a, value), &method_releasetoken);					
	} 
	else
	{
	}


	//printf("ConcatenateA: %s\r\n", (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 0)));
	//printf("ConcatenateB: %s\r\n", (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 1)));
	//printf("ConcatenateR: %s\r\n", (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 2)));
	method_raise(a, 0);	
}

void __cdecl method_sequence(void* a, int signal)
{
	method_raise(a, 0);	
	method_raise(a, 1);	
	method_raise(a, 2);	
	method_raise(a, 3);	
	method_raise(a, 4);	
	method_raise(a, 5);	
	method_raise(a, 6);	
	method_raise(a, 7);	
	method_raise(a, 8);	
}

void __cdecl method_change_input(void* a, int signal)
{
	if( signal == 0 )
	{
		printf("EnableInput:%s %d %d\r\n", 		
			&method_getproperty<char>(a, 0),
			method_getproperty<int>(a, 1),
			method_getproperty<int>(a, 2)
		);	
	}	
	else
	{
		printf("DisableInput:%s %d %d\r\n", 		
			&method_getproperty<char>(a, 0),
			method_getproperty<int>(a, 1),
			method_getproperty<int>(a, 2)
		);	
	}

	method_raise(a, 0);
}

void __cdecl method_raise_signal(void* a, int signal)
{	
	int* method = &method_getproperty<int>(a, 0);
	void* nodeTarget = (char*)method + *method;
	if( nodeTarget == method ) {
		//printf("Target missing\r\n");
	} else {
		method_raise(nodeTarget, 0);
	}
}

void __cdecl method_message(void* a, int signal)
{
	method_compare_and_swap_token(a, 0, &method_releasetoken );
	//printf("Begin Show Message: %s\r\n", (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 0)));
	
	struct helper
	{
		static DWORD __stdcall threadproc(void* a)
		{			
			float delay = method_getproperty<float>(a, 1);
			Sleep( (int)(delay * 1000) );			
			//printf("End Show Message: %s\r\n", (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 0)));
			return 0;
		};
	};	
	
	DWORD threadId = 0;
	HANDLE hThread = CreateThread( 0, 1024 * 2, &helper::threadproc, a, CREATE_SUSPENDED, &threadId );
	ResumeThread(hThread);
	CloseHandle(hThread);	
	method_raise(a, 0);
}

void __cdecl method_delay(void* a, int signal)
{
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	//printf("Delay sleep for: %f\r\n", method_getproperty<float>(a, 0));
	
	struct helper
	{
		static DWORD __stdcall threadproc(void* a)
		{			
			float delay = method_getproperty<float>(a, 0);
			Sleep( (int)(delay * 1000) );			
			method_raise(a, 0);
			return 0;
		};
	};	
	
	DWORD threadId = 0;
	HANDLE hThread = CreateThread( 0, 1024 * 2, &helper::threadproc, a, CREATE_SUSPENDED, &threadId );
	ResumeThread(hThread);
	CloseHandle(hThread);		
}

//Game-specific

void __cdecl method_querylocation(void* a, int signal)
{
	void* node = method_get_global(a);
	method_setproperty<flow::Vector<float, 4>>(a, 1).V[0] = 0;
	method_setproperty<flow::Vector<float, 4>>(a, 1).V[1] = 0;
	method_setproperty<flow::Vector<float, 4>>(a, 1).V[2] = 0;
	method_setproperty<flow::Vector<float, 4>>(a, 1).V[3] = 1;	
	method_setproperty<flow::Vector<float, 3>>(a, 2).V[0] = static_cast<cObject*>(node)->position.x;	
	method_setproperty<flow::Vector<float, 3>>(a, 2).V[1] = static_cast<cObject*>(node)->position.y;	
	method_setproperty<flow::Vector<float, 3>>(a, 2).V[2] = 0;		
	method_raise(a, 0);
}

void __cdecl method_queryhealth(void* a, int signal)
{
	method_setproperty<unsigned int>(a, 0) = method_getproperty<unsigned int>(a, 0);
	method_setproperty<unsigned int>(a, 1) = method_getproperty<unsigned int>(a, 1);
	method_setproperty<unsigned int>(a, 2) = method_getproperty<unsigned int>(a, 2);
	method_setproperty<unsigned int>(a, 1) = player->hp;
	method_setproperty<unsigned int>(a, 2) = 100;
	method_raise(a, 1);
}

void __cdecl method_changehealth(void* a, int signal)
{
	method_setproperty<unsigned int>(a, 0) = method_getproperty<unsigned int>(a, 0);
	method_setproperty<unsigned int>(a, 1) = method_getproperty<unsigned int>(a, 1);
	

	if( method_setproperty<unsigned int>(a, 1) == 0 )
	{
		method_raise(a, 1);
	}
	else
	{
		unsigned int hporig = method_setproperty<unsigned int>(a, 1);
		player->hp += method_setproperty<unsigned int>(a, 1);
		if( player->hp < 0 ) 
			player->hp = 0;
		if( player->hp > 100 )
			player->hp = 100;
		if( player->hp != hporig )
		{
			void* node = 0, *f = a, *n = f;
			if( f != 0 ) 
			do
			{
				unsigned int id = method_getnodekind(f);		
				if( id == flow::Signal::SymbolId )  
				{
					if( stricmp(method_getnodename(f), "HealthChanged") == 0x0 )
					{
						method_raise(f, 0);				
					}		
				}			
			}
			while( n != (f = method_next(f)) );
			method_raise(a, 1);
		}
	}	
}

void __cdecl method_gfxsetvariable(void* a, int signal)
{
	method_setproperty<unsigned int>(a, 0) = method_getproperty<unsigned int>(a, 0);
	method_compare_and_swap_token(a, 1, &method_releasetoken );
	method_setproperty<unsigned int>(a, 2) = method_getproperty<unsigned int>(a, 2);
	char* variableName = (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 1));
	if( strcmp(variableName, "health") == 0x0 ) {
		extern int gfx_var_health;
		gfx_var_health = method_setproperty<unsigned int>(a, 2);
	}
	else if( strcmp(variableName, "score") == 0x0 ) {
		extern int score;
		score = method_setproperty<unsigned int>(a, 2);
	}
	else if( strcmp(variableName, "gameover") == 0x0 ) {
		extern int GameOver;
		GameOver = method_setproperty<unsigned int>(a, 2);
	}
	else if( strcmp(variableName, "gamewon") == 0x0 ) {
		extern int GameWon;
		GameWon = method_setproperty<unsigned int>(a, 2);
	}

	


	


	method_raise(a, 0);
}

void __cdecl method_gfxgetvariable(void* a, int signal)
{
	method_setproperty<unsigned int>(a, 0) = method_getproperty<unsigned int>(a, 0);
	method_compare_and_swap_token(a, 1, &method_releasetoken );
	method_setproperty<unsigned int>(a, 2) = method_getproperty<unsigned int>(a, 2);
	char* variableName = (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 1));
	if( strcmp(variableName, "health") == 0x0 ) {
		extern int gfx_var_health;
		method_setproperty<unsigned int>(a, 2) = gfx_var_health;
	}
	else if( strcmp(variableName, "score") == 0x0 ) {
		extern int score;
		method_setproperty<unsigned int>(a, 2) = score;
	}
	else if( strcmp(variableName, "gameover") == 0x0 ) {
		extern int GameOver;
		method_setproperty<unsigned int>(a, 2) = GameOver;
	}
	else if( strcmp(variableName, "gamewon") == 0x0 ) {
		extern int GameWon;
		method_setproperty<unsigned int>(a, 2) = GameWon;
	}

	method_raise(a, 0);
}

void __cdecl method_spawn(void* a, int signal)
{	
	method_compare_and_swap_token(a, 0, &method_releasetoken );
	method_setproperty<flow::Vector<float, 3>>(a, 1) = method_getproperty<flow::Vector<float, 3>>(a, 1);	
	method_setproperty<flow::Vector<float, 4>>(a, 2) = method_getproperty<flow::Vector<float, 4>>(a, 2);	
	method_setproperty<flow::Vector<float, 3>>(a, 3) = method_getproperty<flow::Vector<float, 3>>(a, 3);
	




	char* classType = (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 0));
	//printf("spawn %s\r\n", classType);

	if( strcmp(classType, "player") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];
		player = new Player();
		player->position.x = x;
		player->position.y = y;
		player->flow = classFlowMapping["player"];
	}
	else if( strcmp(classType, "enemy_01_A") == 0x0 )
	{
		Enemy2* previous = NULL;
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];
		for( int i = 0; i < 5; i++)
		{
			Point p = Point();
			p.x = x;
			p.y = y;
			x += 40;

			Enemy2* node = new Enemy2(NULL, previous);
			node->flow = classFlowMapping["enemy_01"];
			node->position = p;
			previous = node;
			enemies.push_back(node);
		}
	}
	else if( strcmp(classType, "enemy_01_B") == 0x0 )
	{
		Enemy2* previous = NULL;
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];
		for( int i = 0; i < 3; i++)
		{
			Point p = Point();
			p.x = x;
			p.y = y;
			x += 40;

			Enemy2* node = new Enemy2(NULL, previous);
			node->flow = classFlowMapping["enemy_01"];
			node->position = p;
			previous = node;
			enemies.push_back(node);
		}
	}
	else if( strcmp(classType, "enemy_02") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];
		Enemy5* en = new Enemy5();
		en->flow = classFlowMapping["enemy_02"];
		en->position.x = x;
		en->position.y = y;
		enemies.push_back(en);
	}
	else if( strcmp(classType, "enemy_03") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];
		Enemy* en = new Enemy();
		en->flow = classFlowMapping["enemy_03"];
		en->position.x = x;
		en->position.y = y;
		enemies.push_back(en);
	}
	else if( strcmp(classType, "enemy_04") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];
		Enemy1* en = new Enemy1();
		en->flow = classFlowMapping["enemy_04"];
		en->position.x = x;
		en->position.y = y;
		enemies.push_back(en);
	}
	else if( strcmp(classType, "enemy_05_br") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];

		Enemy3* en = new Enemy3();		
		en->mType = BottomRight;
		en->flow = classFlowMapping["enemy_05"];
		en->position.x = x;
		en->position.y = y;
		enemies.push_back(en);
	}
	else if( strcmp(classType, "enemy_05_ur") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];

		Enemy3* en = new Enemy3();		
		en->flow = classFlowMapping["enemy_05"];
		en->mType = UpperRight;
		en->position.x = x;
		en->position.y = y;
		enemies.push_back(en);
	}
	else if( strcmp(classType, "enemy_05_ul") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];

		Enemy3* en = new Enemy3();
		en->flow = classFlowMapping["enemy_05"];
		en->mType = UpperLeft;
		en->position.x = x;
		en->position.y = y;
		enemies.push_back(en);
	}
	else if( strcmp(classType, "enemy_05_bl") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];

		Enemy3* en = new Enemy3();	
		en->flow = classFlowMapping["enemy_05"];
		en->mType = BottomLeft;
		en->position.x = x;
		en->position.y = y;
		enemies.push_back(en);
	}
	else if( strcmp(classType, "enemy_06") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];

		Enemy4* en = new Enemy4();
		en->flow = classFlowMapping["enemy_06"];
		en->position.x = x;
		en->position.y = y;
		enemies.push_back(en);
	}
	else if( strcmp(classType, "enemy_07") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];

		Enemy6* en = new Enemy6();
		en->flow = classFlowMapping["enemy_07"];
		en->position.x = x;
		en->position.y = y;
		enemies.push_back(en);
	}
	else if( strcmp(classType, "enemy_08") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];

		Enemy7* en = new Enemy7();
		en->flow = classFlowMapping["enemy_08"];
		en->position.x = x;
		en->position.y = y;
		enemies.push_back(en);
	}
	else if( strcmp(classType, "enemy_09_A") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];

		Enemy8* previous = NULL;
		for( int i = 0; i < 12; i++)
		{
			Point p = Point();
			p.x = x;
			p.y = y;
			x -= 40;

			Enemy8* node = new Enemy8(NULL, previous);
			node->flow = classFlowMapping["enemy_09"];
			node->position = p;
			previous = node;
			enemies.push_back(node);
		}
	}
	else if( strcmp(classType, "enemy_09_B") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];

		Enemy8* previous = NULL;
		for( int i = 0; i < 4; i++)
		{
			Point p = Point();
			p.x = x;
			p.y = y;
			x -= 50;
			y += 5;

			Enemy8* node = new Enemy8(NULL, previous);
			node->flow = classFlowMapping["enemy_09"];
			node->position = p;
			previous = node;
			enemies.push_back(node);
		}
	}
	else if( strcmp(classType, "enemy_10") == 0x0 )
	{
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];

		Enemy9* en = new Enemy9();
		en->flow = classFlowMapping["enemy_10"];
		en->position.x = x;
		en->position.y = y;
		enemies.push_back(en);
	}
	else if( strcmp(classType, "explosion") == 0x0 )
	{
		/*
		float x = method_setproperty<flow::Vector<float, 3>>(a, 3).V[0];
		float y = method_setproperty<flow::Vector<float, 3>>(a, 3).V[1];

		Enemy9* en = new Enemy9();
		en->position.x = x;
		en->position.y = y;
		enemies.push_back(en);
		*/

		void* node = method_get_global(a);
		static_cast<cObject*>(node)->IsExploding = true;
	}
	


	
	

	method_raise(a, 0);
}


void __cdecl method_use_weapon(void* a, int signal)
{
	/*
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);	
	extern int MenuState;	
	extern void Reset();
	if( MenuState != method_setproperty<int>(a, 0) )
	{
		MenuState = method_setproperty<int>(a, 0);
		if( MenuState == 1 )
			Reset();
	}
	*/

	if( signal == 0 )
	{	
		player->showBeamCollection = true;
		player->isfireing = true;
	}
	else if( signal == 1 )
	{
		if( player->isfireing == true )
		{			
			player->showBeamCollection = false;
			player->isfireing = false;

			BulletType m;
			if( beamstrength < 20 ) {
				m = Bullet1;
			} else if( beamstrength < 40 ) {
				m = Bullet2;
			} else if( beamstrength < 60 ) {
				m = Bullet3;
			} else if( beamstrength < 80 ) {
				m = Bullet4;
			} else if( beamstrength < 100 ) {
				m = Bullet5;
			} else {
				m = Bullet6;
			}
			

			Bullet* b = new Bullet(m);
			b->position = player->position;
			b->position.x += 20;
			v.push_back(b);
       			beamstrength = 0;
		}			
	}	
	else if( signal == 2 )
	{

	}	
	else if( signal == 3 )
	{

	}	
}

void __cdecl method_change_menu_state(void* a, int signal)
{
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
	method_raise(a, 0);
	extern int MenuState;
	

	
	extern void Reset();
	if( MenuState != method_setproperty<int>(a, 0) )
	{	
		extern int score;
		extern char name[12];
		extern char ScoreboardNameList[5][12];
		extern int HighscoresScores[5];
		extern bool AddKeyHook( char key, DelegateCallback callback);
		extern bool RemoveKeyHook( char key, DelegateCallback callback);
		extern void KeyA();
		extern void KeyB();
		extern void KeyC();
		extern void KeyD();
		extern void KeyE();
		extern void KeyF();
		extern void KeyG();
		extern void KeyH();
		extern void KeyI();
		extern void KeyJ();
		extern void KeyK();
		extern void KeyL();
		extern void KeyM();
		extern void KeyN();
		extern void KeyO();
		extern void KeyP();
		extern void KeyQ();
		extern void KeyR();
		extern void KeyS();
		extern void KeyT();
		extern void KeyU();
		extern void KeyV();
		extern void KeyW();
		extern void KeyX();
		extern void KeyY();
		extern void KeyZ();
		extern void KeyBackspacce();

		int orig = MenuState;
		if( orig == 3)
		{
			RemoveKeyHook( 65, &KeyA);
			RemoveKeyHook( 66, &KeyB);
			RemoveKeyHook( 67, &KeyC);
			RemoveKeyHook( 68, &KeyD);
			RemoveKeyHook( 69, &KeyE);
			RemoveKeyHook( 70, &KeyF);
			RemoveKeyHook( 71, &KeyG);
			RemoveKeyHook( 72, &KeyH);
			RemoveKeyHook( 73, &KeyI);
			RemoveKeyHook( 74, &KeyJ);
			RemoveKeyHook( 75, &KeyK);
			RemoveKeyHook( 76, &KeyL);
			RemoveKeyHook( 77, &KeyM);
			RemoveKeyHook( 78, &KeyN);
			RemoveKeyHook( 79, &KeyO);
			RemoveKeyHook( 80, &KeyP);
			RemoveKeyHook( 81, &KeyQ);
			RemoveKeyHook( 82, &KeyR);
			RemoveKeyHook( 83, &KeyS);
			RemoveKeyHook( 84, &KeyT);
			RemoveKeyHook( 85, &KeyU);
			RemoveKeyHook( 86, &KeyV);
			RemoveKeyHook( 87, &KeyW);
			RemoveKeyHook( 88, &KeyX);
			RemoveKeyHook( 89, &KeyY);
			RemoveKeyHook( 90, &KeyZ);
			RemoveKeyHook( key_backspace, &KeyBackspacce);

			//Update highscore table
			for( int i = 4; i > -1; i--)
			{
				if( i == 4 )
				{
					if( score > HighscoresScores[i] )
					{
						HighscoresScores[i] = score;
						for( int a = 0; a < 12; a++)
						{
							name[a] ^= ScoreboardNameList[i][a];
							ScoreboardNameList[i][a] ^= name[a];
							name[a] ^= ScoreboardNameList[i][a];
						}
					}
				}
				else
				{

					if( HighscoresScores[i + 1] > HighscoresScores[i] )
					{
						int temp = HighscoresScores[i];
						HighscoresScores[i] = HighscoresScores[i + 1];
						HighscoresScores[i + 1] = temp;
						
						for( int a = 0; a < 12; a++)
						{
							ScoreboardNameList[i + 1][a] ^= ScoreboardNameList[i][a];
							ScoreboardNameList[i][a] ^= ScoreboardNameList[i + 1][a];
							ScoreboardNameList[i + 1][a] ^= ScoreboardNameList[i][a];
						}

					}
				}
				
			}

			//Open the file
			errno_t err;
			FILE* pFile;
			if( (err  = fopen_s( &pFile, "highscores.cni", "wb" )) == 0 )
			{
				fwrite( (char*)(&ScoreboardNameList), sizeof(char), 60, pFile); 
				fwrite( HighscoresScores, sizeof(int), 5, pFile);

				//Close the file
				fclose(pFile);
			}
		}

		MenuState = method_setproperty<int>(a, 0);
		if( MenuState == 1 )
			Reset();
		if( MenuState == 3)
		{
 			AddKeyHook( 65, &KeyA);
			AddKeyHook( 66, &KeyB);
			AddKeyHook( 67, &KeyC);
			AddKeyHook( 68, &KeyD);
			AddKeyHook( 69, &KeyE);
			AddKeyHook( 70, &KeyF);
			AddKeyHook( 71, &KeyG);
			AddKeyHook( 72, &KeyH);
			AddKeyHook( 73, &KeyI);
			AddKeyHook( 74, &KeyJ);
			AddKeyHook( 75, &KeyK);
			AddKeyHook( 76, &KeyL);
			AddKeyHook( 77, &KeyM);
			AddKeyHook( 78, &KeyN);
			AddKeyHook( 79, &KeyO);
			AddKeyHook( 80, &KeyP);
			AddKeyHook( 81, &KeyQ);
			AddKeyHook( 82, &KeyR);
			AddKeyHook( 83, &KeyS);
			AddKeyHook( 84, &KeyT);
			AddKeyHook( 85, &KeyU);
			AddKeyHook( 86, &KeyV);
			AddKeyHook( 87, &KeyW);
			AddKeyHook( 88, &KeyX);
			AddKeyHook( 89, &KeyY);
			AddKeyHook( 90, &KeyZ);
			AddKeyHook( key_backspace, &KeyBackspacce);
		}
	}

}

void __cdecl method_change_menu_substate(void* a, int signal)
{
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
	method_raise(a, 0);
	extern int options;
	options = method_setproperty<int>(a, 0);
}

void __cdecl method_preexecute_querymenustate(void* a, int signal)
{		
	extern int MenuState;
	method_setproperty<int>(a, 0) = MenuState;
}

void __cdecl method_preexecute_querymenusubstate(void* a, int signal)
{	
	extern int options;
	method_setproperty<int>(a, 0) = options;
}



void* loadvpl(void* global, int argc, char* argv)
{	
	const char* filename = "../flow.xml";
	const char* ofilename = argv;


	int sizeOfExecutable;
	void* addr, *address, *node = 0;

	using namespace flow;

	//Setup function mapping
	std::map<int, method_signature> functionMapping;
	functionMapping[CompareBool::SymbolId] = &method_comparison_bool;	
	functionMapping[CompareInt32::SymbolId] = &method_comparison_int32;
	functionMapping[CompareFloat::SymbolId] = &method_comparison_float;
	functionMapping[CounterInt32::SymbolId] = &method_increment_and_compare_int32;
	functionMapping[CounterFloat::SymbolId] = &method_increment_and_compare_float;
	functionMapping[AddInt32::SymbolId] = &method_add_int32;	
	functionMapping[SubtractInt32::SymbolId] = &method_subtract_int32;	
	functionMapping[MultiplyInt32::SymbolId] = &method_multiply_int32;	
	functionMapping[DivideInt32::SymbolId] = &method_divide_int32;	
	functionMapping[AddFloat::SymbolId] = &method_add_real32;	
	functionMapping[SubtractFloat::SymbolId] = &method_subtract_real32;	
	functionMapping[MultiplyFloat::SymbolId] = &method_multiply_real32;	
	functionMapping[DivideFloat::SymbolId] = &method_divide_real32;	
	functionMapping[Sin::SymbolId] = &method_sin;	
	functionMapping[Cos::SymbolId] = &method_cos;	
	functionMapping[Tan::SymbolId] = &method_tan;	
	functionMapping[ArcSin::SymbolId] = &method_asin;	
	functionMapping[ArcCos::SymbolId] = &method_acos;	
	functionMapping[ArcTan::SymbolId] = &method_atan;	
	functionMapping[SinHyperbolic::SymbolId] = &method_sinh;	
	functionMapping[CosHyperbolic::SymbolId] = &method_cosh;	
	functionMapping[TanHyperbolic::SymbolId] = &method_tanh;	
	functionMapping[Min::SymbolId] = &method_min;	
	functionMapping[Max::SymbolId] = &method_max;		
	functionMapping[Sequence::SymbolId] = &method_sequence;
	functionMapping[Message::SymbolId] = &method_message;
	functionMapping[Delay::SymbolId] = &method_delay;
	functionMapping[Assert::SymbolId] = &method_assert;
	functionMapping[Concatenate::SymbolId] = &method_assert2;	
	functionMapping[ChangeMenuState::SymbolId] = &method_change_menu_state;	
	functionMapping[ChangeMenuSubState::SymbolId] = &method_change_menu_substate;	
	functionMapping[CycleInt32::SymbolId] = &method_cycle_int32;	
	functionMapping[UseWeapon::SymbolId] = &method_use_weapon;
	functionMapping[Spawn::SymbolId] = &method_spawn;
	functionMapping[GFXSetVariable::SymbolId] = &method_gfxsetvariable;
	functionMapping[GFXGetVariable::SymbolId] = &method_gfxgetvariable;
	functionMapping[ChangeHealth::SymbolId] = &method_changehealth;
	functionMapping[QueryHealthOfUnit::SymbolId] = &method_queryhealth;
	functionMapping[QueryLocationOfUnit::SymbolId] = &method_querylocation;

	

	

	
	
	

	
	//functionMapping[ChangeInput::SymbolId] = &method_change_input;
	functionMapping[Raise::SymbolId] = &method_raise_signal;

	//Setup coerce value mapping
	std::map<int, method_signature> coerceMapping;
	coerceMapping[RandomInt32::SymbolId]   = &method_preexecute_random_int32;
	coerceMapping[RandomFloat::SymbolId]   = &method_preexecute_random_real32;
	coerceMapping[QueryMenuState::SymbolId]   = &method_preexecute_querymenustate;
	coerceMapping[QueryMenuSubState::SymbolId]   = &method_preexecute_querymenusubstate;




	//Parse the language to generate the executable (base-)image
	addr = parse(filename, ofilename, sizeOfExecutable);
	if( addr != 0 )
	{
		address = addr;

		//Create shadow copy of the resource
		//address = _aligned_malloc(sizeOfExecutable, 4096);
		//memcpy(address, addr, sizeOfExecutable );

		//Free memory created by the parser.
		//_aligned_free(addr);

		//Hotpatch the executable image to link our recipient code into it.
		dynamic_receptors_link(functionMapping, coerceMapping, &method_missing, address);

		//Load the executable imagine i.e. commmit the memory to being executable.
		addr = load(global, address, sizeOfExecutable);

		return addr;			
	}
	
	return 0;

}
