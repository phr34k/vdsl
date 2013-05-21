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


void __cdecl method_releasetoken(void* a, unsigned int token)
{
	//Releasing token	
	unsigned int type = method_gettokentype( a, token );
	if( type == 1 ) 
	{		
		static int l = 0;
		//printf("Delete string %d\r\n", ++l);
		delete (char*)method_gettoken(a, token);
	}
}

//Method reciptents
void __cdecl method_preexecute_random_real32(void* a, int signal)
{		
	float min = method_getproperty<float>(a, 1), max = method_getproperty<float>(a, 2);
	float v = (float)(((double)rand() / (double)RAND_MAX) * (max - min)) + min;
	method_setproperty<float>(a, 0) = v;
	printf("RFLOAT:%f\r\n", v);
}

void __cdecl method_preexecute_random_int32(void* a, int signal)
{	
	int min = method_getproperty<int>(a, 1), max = method_getproperty<int>(a, 2);
	int v = (int)(((double)rand() / (double)RAND_MAX) * (max - min)) + min;
	method_setproperty<int>(a, 0) = v;
	printf("RINT:%d\r\n", v);
}

void __cdecl method_missing(void* a, int signal)
{
	printf("method_missing %d\r\n", 
		method_getnodekind(a));
}

void __cdecl method_comparison_bool(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<bool>(a, 0) = method_getproperty<bool>(a, 0);
	method_setproperty<bool>(a, 1) = method_getproperty<bool>(a, 1);
	printf("CBOOL:A %s\r\n", method_setproperty<bool>(a, 0) ? "True" : "False");
	printf("CBOOL:B %s\r\n", method_setproperty<bool>(a, 1) ? "True" : "False");
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
	printf("CINT32:A %d\r\n", method_setproperty<int>(a, 0));
	printf("CINT32:B %d\r\n", method_setproperty<int>(a, 1));
	if( method_setproperty<int>(a, 0) <= method_setproperty<int>(a, 1) ) {
		printf("Cint: A <= B\r\n");
		method_raise(a, 0);
	} 
	
	if( method_setproperty<int>(a, 0) > method_setproperty<int>(a, 1) ) {
		printf("Cint: A > B\r\n");
		method_raise(a, 1);		
	} 
	
	if( method_setproperty<int>(a, 0) == method_setproperty<int>(a, 1) ) {
		printf("Cint: A == B\r\n");
		method_raise(a, 2);		
	} 
	
	if( method_setproperty<int>(a, 0) < method_setproperty<int>(a, 1) ) {
		printf("Cint: A < B\r\n");
		method_raise(a, 3);		
	} 
	
	if( method_setproperty<int>(a, 0) >= method_setproperty<int>(a, 1) ) {
		printf("Cint: A >= B\r\n");
		method_raise(a, 4);		
	}	
}

void __cdecl method_comparison_float(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	printf("CFLOAT:A %f\r\n", method_setproperty<float>(a, 0));
	printf("CFLOAT:B %f\r\n", method_setproperty<float>(a, 1));
	if( method_setproperty<float>(a, 0) <= method_setproperty<float>(a, 1) ) {
		printf("CFLOAT: A <= B\r\n");
		method_raise(a, 0);
	} 
	
	if( method_setproperty<float>(a, 0) > method_setproperty<float>(a, 1) ) {
		printf("CFLOAT: A > B\r\n");
		method_raise(a, 1);		
	} 
	
	if( method_setproperty<float>(a, 0) == method_setproperty<float>(a, 1) ) {
		printf("CFLOAT: A == B\r\n");
		method_raise(a, 2);		
	} 
	
	if( method_setproperty<float>(a, 0) < method_setproperty<float>(a, 1) ) {
		printf("CFLOAT: A < B\r\n");
		method_raise(a, 3);		
	} 
	
	if( method_setproperty<float>(a, 0) >= method_setproperty<float>(a, 1) ) {
		printf("CFLOAT: A >= B\r\n");
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

	printf("ICINT32:A %d\r\n", method_setproperty<int>(a, 3));
	printf("ICINT32:B %d\r\n", method_setproperty<int>(a, 1));
	printf("ICFLOAT:I %d\r\n", method_setproperty<int>(a, 2));
	printf("ICFLOAT:R %d\r\n", method_setproperty<int>(a, 3));
	if( method_setproperty<int>(a, 3) <= method_setproperty<int>(a, 1) ) {
		printf("ICint: R <= B\r\n");
		method_raise(a, 0);
	} 
	
	if( method_setproperty<int>(a, 3) > method_setproperty<int>(a, 1) ) {
		printf("ICint: R > B\r\n");
		method_raise(a, 1);		
	} 
	
	if( method_setproperty<int>(a, 3) == method_setproperty<int>(a, 1) ) {
		printf("ICint: R == B\r\n");
		method_raise(a, 2);		
	} 
	
	if( method_setproperty<int>(a, 3) < method_setproperty<int>(a, 1) ) {
		printf("ICint: R < B\r\n");
		method_raise(a, 3);		
	} 
	
	if( method_setproperty<int>(a, 3) >= method_setproperty<int>(a, 1) ) {
		printf("ICint: R >= B\r\n");
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
	printf("ICFLOAT:A %f\r\n", method_setproperty<float>(a, 0));
	printf("ICFLOAT:B %f\r\n", method_setproperty<float>(a, 1));
	printf("ICFLOAT:I %f\r\n", method_setproperty<float>(a, 2));
	printf("ICFLOAT:R %f\r\n", method_setproperty<float>(a, 3));
	if( method_setproperty<float>(a, 3) <= method_setproperty<float>(a, 1) ) {
		printf("ICFLOAT: R <= B\r\n");
		method_raise(a, 0);
	} 
	
	if( method_setproperty<float>(a, 3) > method_setproperty<float>(a, 1) ) {
		printf("ICFLOAT: R > B\r\n");
		method_raise(a, 1);		
	} 
	
	if( method_setproperty<float>(a, 3) == method_setproperty<float>(a, 1) ) {
		printf("ICFLOAT: R == B\r\n");
		method_raise(a, 2);		
	} 
	
	if( method_setproperty<float>(a, 3) < method_setproperty<float>(a, 1) ) {
		printf("ICFLOAT: R < B\r\n");
		method_raise(a, 3);		
	} 
	
	if( method_setproperty<float>(a, 3) >= method_setproperty<float>(a, 1) ) {
		printf("ICFLOAT: R >= B\r\n");
		method_raise(a, 4);		
	}	
}


//Integer arthimetics
void __cdecl method_add_int32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
	method_setproperty<int>(a, 1) = method_getproperty<int>(a, 1);
	method_setproperty<int>(a, 2) = method_setproperty<int>(a, 0) + method_setproperty<int>(a, 1);
	printf("AINT32:A %d\r\n", method_setproperty<int>(a, 0));
	printf("AINT32:B %d\r\n", method_setproperty<int>(a, 1));
	printf("AINT32:C %d\r\n", method_setproperty<int>(a, 2));
	method_raise(a, 0);	
}

void __cdecl method_subtract_int32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
	method_setproperty<int>(a, 1) = method_getproperty<int>(a, 1);
	method_setproperty<int>(a, 2) = method_setproperty<int>(a, 0) - method_setproperty<int>(a, 1);
	printf("SINT32:A %d\r\n", method_setproperty<int>(a, 0));
	printf("SINT32:B %d\r\n", method_setproperty<int>(a, 1));
	printf("SINT32:C %d\r\n", method_setproperty<int>(a, 2));
	method_raise(a, 0);	
}

void __cdecl method_multiply_int32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
	method_setproperty<int>(a, 1) = method_getproperty<int>(a, 1);
	method_setproperty<int>(a, 2) = method_setproperty<int>(a, 0) * method_setproperty<int>(a, 1);
	printf("MINT32:A %d\r\n", method_setproperty<int>(a, 0));
	printf("MINT32:B %d\r\n", method_setproperty<int>(a, 1));
	printf("MINT32:C %d\r\n", method_setproperty<int>(a, 2));
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
	printf("DINT32:A %d\r\n", method_setproperty<int>(a, 0));
	printf("DINT32:B %d\r\n", method_setproperty<int>(a, 1));
	printf("DINT32:C %d\r\n", method_setproperty<int>(a, 2));
	method_raise(a, 0);	
}

//Float arthimetics
void __cdecl method_add_real32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	method_setproperty<float>(a, 2) = method_setproperty<float>(a, 0) + method_setproperty<float>(a, 1);
	printf("AREAL32:A %f\r\n", method_setproperty<float>(a, 0));
	printf("AREAL32:B %f\r\n", method_setproperty<float>(a, 1));
	printf("AREAL32:C %f\r\n", method_setproperty<float>(a, 2));
	method_raise(a, 0);	
}

void __cdecl method_subtract_real32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	method_setproperty<float>(a, 2) = method_setproperty<float>(a, 0) - method_setproperty<float>(a, 1);
	printf("SREAL32:A %f\r\n", method_setproperty<float>(a, 0));
	printf("SREAL32:B %f\r\n", method_setproperty<float>(a, 1));
	printf("SREAL32:C %f\r\n", method_setproperty<float>(a, 2));
	method_raise(a, 0);	
}

void __cdecl method_multiply_real32(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	method_setproperty<float>(a, 2) = method_setproperty<float>(a, 0) * method_setproperty<float>(a, 1);
	printf("MREAL32:A %f\r\n", method_setproperty<float>(a, 0));
	printf("MREAL32:B %f\r\n", method_setproperty<float>(a, 1));
	printf("MREAL32:C %f\r\n", method_setproperty<float>(a, 2));
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
	printf("DREAL32:A %f\r\n", method_setproperty<float>(a, 0));
	printf("DREAL32:B %f\r\n", method_setproperty<float>(a, 1));
	printf("DREAL32:C %f\r\n", method_setproperty<float>(a, 2));
	method_raise(a, 0);	
}


//Math
void __cdecl method_sin(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = sin(method_setproperty<float>(a, 0));
	printf("SIN:A %f\r\n", method_setproperty<float>(a, 0));
	printf("SIN:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_cos(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = cos(method_setproperty<float>(a, 0));
	printf("COS:A %f\r\n", method_setproperty<float>(a, 0));
	printf("COS:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_tan(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = tan(method_setproperty<float>(a, 0));
	printf("TAN:A %f\r\n", method_setproperty<float>(a, 0));
	printf("TAN:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_asin(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = asin(method_setproperty<float>(a, 0));
	printf("ASIN:A %f\r\n", method_setproperty<float>(a, 0));
	printf("ASIN:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_acos(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = acos(method_setproperty<float>(a, 0));
	printf("ACOS:A %f\r\n", method_setproperty<float>(a, 0));
	printf("ACOS:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_atan(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = atan(method_setproperty<float>(a, 0));
	printf("ATAN:A %f\r\n", method_setproperty<float>(a, 0));
	printf("ATAN:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}



void __cdecl method_sinh(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = sinh(method_setproperty<float>(a, 0));
	printf("SINH:A %f\r\n", method_setproperty<float>(a, 0));
	printf("SINH:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_cosh(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = cosh(method_setproperty<float>(a, 0));
	printf("COSH:A %f\r\n", method_setproperty<float>(a, 0));
	printf("COSH:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}

void __cdecl method_tanh(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = tanh(method_setproperty<float>(a, 0));
	printf("TANH:A %f\r\n", method_setproperty<float>(a, 0));
	printf("TANH:R %f\r\n", method_setproperty<float>(a, 1));
	method_raise(a, 0);	
}


void __cdecl method_min(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	method_setproperty<float>(a, 2) = method_getproperty<float>(a, 0) < method_setproperty<float>(a, 1) ? method_getproperty<float>(a, 0) : method_getproperty<float>(a, 1);
	printf("MIN:A %f\r\n", method_setproperty<float>(a, 0));
	printf("MIN:B %f\r\n", method_setproperty<float>(a, 1));
	printf("MIN:R %f\r\n", method_setproperty<float>(a, 2));
	method_raise(a, 0);	
}

void __cdecl method_max(void* a, int signal)
{
	//Propegate the values..
	method_setproperty<float>(a, 0) = method_getproperty<float>(a, 0);
	method_setproperty<float>(a, 1) = method_getproperty<float>(a, 1);
	method_setproperty<float>(a, 2) = method_getproperty<float>(a, 0) > method_setproperty<float>(a, 1) ? method_getproperty<float>(a, 0) : method_getproperty<float>(a, 1);
	printf("MAX:A %f\r\n", method_setproperty<float>(a, 0));
	printf("MAX:B %f\r\n", method_setproperty<float>(a, 1));
	printf("MAX:R %f\r\n", method_setproperty<float>(a, 2));
	method_raise(a, 0);	
}

//Others
void __cdecl method_gate(void* a, int signal)
{
	if( (method_getproperty<int>(a, 0))++ == 5 )
	{
		method_getproperty<int>(a, 0) = 0;		
		printf("Disallow gate\r\n");
		return;
	}
	else
	{
		printf("Allow gate\r\n");
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


	printf("ConcatenateA: %s\r\n", (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 0)));
	printf("ConcatenateB: %s\r\n", (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 1)));
	printf("ConcatenateR: %s\r\n", (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 2)));
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
		printf("Target missing\r\n");
	} else {
		method_raise(nodeTarget, 0);
	}
}

void __cdecl method_message(void* a, int signal)
{
	method_compare_and_swap_token(a, 0, &method_releasetoken );
	printf("Begin Show Message: %s\r\n", (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 0)));
	
	struct helper
	{
		static DWORD __stdcall threadproc(void* a)
		{			
			float delay = method_getproperty<float>(a, 1);
			Sleep( (int)(delay * 1000) );			
			printf("End Show Message: %s\r\n", (char*)method_gettoken(a, method_setproperty<unsigned int>(a, 0)));
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
	printf("Delay sleep for: %f\r\n", method_getproperty<float>(a, 0));
	
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

void __cdecl method_getvariable(void* a, int signal)
{
	method_setproperty<int>(a, 0) = method_getproperty<int>(a, 0);
		
	
	if( method_readproperty<int>(a, 2) == 5 /* Determines if the type was a integer */ )
	{
		method_setproperty<int>(a, 2) = method_getproperty<int>(a, 2);
		method_setproperty<int>(a, 2) = 22;
		method_raise(a, 0);
	}
	else if( method_readproperty<int>(a, 2) == 4 /* Determines if the type was a float */ )
	{
		method_setproperty<float>(a, 2) = method_getproperty<float>(a, 2);
		method_setproperty<float>(a, 2) = 9.8;
		method_raise(a, 0);
	}	
	else
	{
		method_raise(a, 0);
	}
}



void main(int argc, char** argv)
{
	if( argc == 1 )
	{
		printf("This executable was designed to run from the SandboxDesigner\r\n");
		printf("Usage: Sandbox.exe filename schemafilename \r\n");
		getchar();
		return;
	}
	const char* filename = /*"C:\\Users\\Phr34k\\Desktop\\dds\\flow\\flow.xml"*/ argv[1];
	const char* ofilename = /*"C:\\Users\\Phr34k\\Desktop\\dds\\flow\\level3.xml"*/ argv[2];


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
	functionMapping[GFXGetVariable2::SymbolId] = &method_getvariable;
	
	

	
	//functionMapping[ChangeInput::SymbolId] = &method_change_input;
	functionMapping[Raise::SymbolId] = &method_raise_signal;

	//Setup coerce value mapping
	std::map<int, method_signature> coerceMapping;
	coerceMapping[RandomInt32::SymbolId]   = &method_preexecute_random_int32;
	coerceMapping[RandomFloat::SymbolId]   = &method_preexecute_random_real32;

	/* initialize random seed: */
	srand (time(NULL));	

	/* pre-generate a bunch of numbers as a hack for creating numbers on consecutive application startups */
	for( int i = 0; i < 20; i++ ) rand();	

	//Parse the language to generate the executable (base-)image
	addr = parse(filename, ofilename, sizeOfExecutable);
	if( addr != 0 )
	{
		//Create shadow copy of the resource
		address = _aligned_malloc(sizeOfExecutable, 4096);
		memcpy(address, addr, sizeOfExecutable );

		//Free memory created by the parser.
		_aligned_free(addr);

		//Hotpatch the executable image to link our recipient code into it.
		dynamic_receptors_link(functionMapping, coerceMapping, &method_missing, address);

		//Load the executable imagine i.e. commmit the memory to being executable.
		addr = load(0, address, sizeOfExecutable );
		
		//Find then entry point and execute the node.
		if( 0 == (node = method_getbyname(address, "Execute"))) {
			printf("Entry point was not found in the executable\r\n");
		} else {
			int c = method_num_active_tokens(address);
			method_raise(node, 0);
			printf("%d\r\n", c);
			printf("%d\r\n", method_num_active_tokens(address));
		}
		
		//Unload the executable image i.e. reset the memory to non-executable 
		//unload(address);
	}
	
	
	getchar();
}
