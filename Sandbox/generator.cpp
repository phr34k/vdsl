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

#include "generator.h"
#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include <string>
#include <map>
#include "tinyxml.h"
#include "weak.h"


const int ptr_size		= sizeof(void*);
const int prologue_jump = /*13*/ 23 + 16;
const int prologue_size = /*18*/ prologue_jump + 5;
const int method_base   = 4 + 4 + 4 + 4;
const int method_table  = method_base + 5 + 5 + 5;
const int method_exec   = method_base;
const int method_pexec  = method_base + 5;
const int method_pexec2  = method_base + 5 + 5;

struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

typedef void (__cdecl *method_releasetokensig)(void* a, unsigned int token);
typedef std::map<unsigned int, TiXmlElement*, std::less<int>, flow_allocator<std::pair<unsigned int, TiXmlElement*>>> dictionary_nodexml;
typedef std::map<std::string, int, std::less<std::string>, flow_allocator<std::pair<std::string,  int>>> dictionary_ordeal;
typedef std::map<std::string, TiXmlElement*, std::less<std::string>, flow_allocator<std::pair<std::string,  TiXmlElement*>>> dictionary_types;
typedef std::map<std::string, void*, std::less<std::string>, flow_allocator<std::pair<std::string,  void*>>> dictionary_strings;
typedef std::multimap<const char*, TiXmlElement*, ltstr, flow_allocator<std::pair<const char*, TiXmlElement*>>> dictionary_connections;


struct FlowHeader
{
	void* head;
	void* tail;
};




/*
template<typename T> T& method_getproperty(void* a, int offset)
{
	//Template sugar
	extern void* method_getpropertyaddr(void* a, int offset);
	return *static_cast<T*>(method_getpropertyaddr(a, offset));
}

template<typename T> T& method_setproperty(void* a, int offset)
{
	//Template sugar
	extern void* method_setpropertyaddr(void* a, int offset);
	return *static_cast<T*>(method_setpropertyaddr(a, offset));
}
*/

void* method_getpropertyaddr(void* a, int offset)
{
	//Each node can have one or more proprties. Each propety is represented by two relative pointers. One pointing to 
	//the value it reads, and one pointing to the value it writes. This routine retrieves the adress for the former, 
	//when the property is linked it points directly to the memory of another node, otherwise it points to the write 
	//memory of itself.
	char* s = reinterpret_cast<char*>(a) - 8 - offset * 8;
	int* r = (int*)s;
	return s + *r;	
}

void* method_setpropertyaddr(void* a, int offset)
{
	//Each node can have one or more proprties. Each propety is represented by two relative pointers. One pointing to 
	//the value it reads, and one pointing to the value it writes. This routine retrieves the adress for the latter. 
	//Other nodes may reference this data.
	char* s = reinterpret_cast<char*>(a) - 4 - offset * 8;
	int* r = (int*)s;
	return s + *r;	
}

void* method_readpropertyaddr(void* a, int offset)
{	
	//This is a bit of hack, to support a concept i refer to as property overloading. Basicly in order allow certain properties 
	//their types to be chosen at design time, we have to facillitate either rerouting the method invoked based on the 
	//signature, or be able to deduct type information and perform make a decision at runtime. The latter has been implemented; 
	//in the four bytes before the the actual data, a type id for the data is stored.
	char* s = reinterpret_cast<char*>(a) - 4 - offset * 8;
	int* r = (int*)s;
	return (s + *r) - 4;	
}

char* method_getnodename(void* a)
{
	//Each node can be assigned a name. When the node has no name assigned a simply '0' is returned. Otherwise retrieve 
	//the name from the memory using relative pointers.
	char* s = reinterpret_cast<char*>(a) + 4 + 4;
	if( *reinterpret_cast<int*>(s)  == 0 ) 
		return 0;
	else
		return (char*)(reinterpret_cast<char*>(s)  + *reinterpret_cast<int*>(s));
}

int  method_getnodekind(void* a)
{
	//Each node references a type id, to identify the node. This is id is used in the 'dynamic reception' to map the correct 
	//routines to each ot the nodes.
	char* s = reinterpret_cast<char*>(a) + 4;
	return *(int*)s;
}

void method_signal(void* a, int signal)
{
	//This function directly pre-activates and activates the specified node using the specified input. This method can 
	//be used to implement events, such as a keypressed, or a keyreleased event. These are events that should be raised 
	//by the host application.

	char* executeFunction = (char*)a + method_exec;

	__asm
	{
		//Prologue for pre-activation
		push signal;
		push a;		
		//Pre-activate
		call method_pexec;
		call executeFunction;	
		//Epilogue
		add  esp, 8;
	}	
}

void method_raise(void* a, int offset)
{
	//This function raises a output link of our node. Basicly this routine evaluates roughly as
	//	foreach( emanating output in outputs ) 
	//		method_signal(output.connectedTo as signal, output.connectedTo as node)

	int r = method_table + prologue_size * 2 * offset;

	__asm 
	{			
		//Prologue for pre-activation
		push eax
		mov  eax, a;
		add  eax, r;
		add  eax, prologue_size;				
		//Push a dummy variable so it matches the invocation signature (cdecl void)(void*, int)
		push 6;
		//Push a dummy variable so it matches the invocation signature (cdecl void)(void*, int)
		push a;		
		//Call the pre-activation routine (ret if not present), the dummy variables will be rewritten
		call eax;
		//Epilogue
		add  esp, 8;

		//Prologue for activation
		mov  eax, a;
		add  eax, r;
		//Push a dummy variable so it matches the invocation signature (cdecl void)(void*, int)
		push 6;
		//Push a dummy variable so it matches the invocation signature (cdecl void)(void*, int)
		push a;		
		//Call the pre-activation routine (ret if not present), the dummy variables will be rewritten
		call eax;
		//Epilogue
		add  esp, 8;
		pop  eax;
	}
}




void* method_getfirst(void* a)
{
	//Searches the flow executable for the first node.
	if( *reinterpret_cast<int*>(a)  == 0 ) 
		return 0;
	else
		return reinterpret_cast<char*>(a)  + *reinterpret_cast<int*>(a) ;
}

void* method_next(void* a)
{
	/*
	if( reinterpret_cast<char*>(a) + *reinterpret_cast<int*>(a) == a )
		return 0;
	*/

	//Searches for the next flow node, given a input node. Each flow node is linked 
	//using a cyclic linked list.
	return reinterpret_cast<char*>(a) + *reinterpret_cast<int*>(a);
}



void* method_getbyname(void* a, const char* name)
{
	//Seaches the the flow executable for the first node with a specific name.
	void* node = 0, *f = method_getfirst(a), *n = f;
	if( f != 0 ) do {
		char* member = method_getnodename(f);		
		if( member != 0 && stricmp(member, name) == 0 ) node = f;
	} while( n != (f = method_next(f)) );
	return node;
}




void* method_get_global(void* a)
{
	//Fetch a global pointer, could be used to couple the flow executale to a particulair object.
	char* baseImage = reinterpret_cast<char*>(a) + ptr_size + ptr_size + ptr_size;
	baseImage = (char*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage));
	baseImage += 12;
	return *reinterpret_cast<void**>(baseImage);
}

void  method_set_global(void* a, void* addr)
{
	//Store a global pointer, could be used to couple the flow executale to a particulair object.
	char* baseImage = reinterpret_cast<char*>(a) + ptr_size + ptr_size + ptr_size;
	baseImage = (char*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage));
	baseImage += 12;
	*reinterpret_cast<void**>(baseImage) = addr;
}

unsigned int method_num_active_tokens(void* a)
{
	//Helper function to obtain the number of active tokens. Heap allocated types i.e. strings, vectors should be stored using tokens with copy-on-write semantics.
	char* baseImage = (char*)a;
	baseImage = baseImage + 8;
	aurora::WeakReferenceManager<12, 15>& weakreferenceManager = *(aurora::WeakReferenceManager<12, 15>*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage));	
	return weakreferenceManager.Count();
}

unsigned int method_createtoken(void* a, void* ptr)
{
	//Create a token. Heap allocated types i.e. strings, vectors should be stored using tokens with copy-on-write semantics.
	char* baseImage = reinterpret_cast<char*>(a) + 4 + 4 + 4;
	baseImage = (char*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage));
	void* ptr_diff = (void*)(static_cast<char*>(ptr) - static_cast<char*>(baseImage));
	baseImage = baseImage + 4;
	unsigned int* referenceCounter = (unsigned int*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage));
	baseImage = baseImage + 4;
	aurora::WeakReferenceManager<12, 15>& weakreferenceManager = *(aurora::WeakReferenceManager<12, 15>*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage));


	if( weakreferenceManager.m_activeEntryCount < weakreferenceManager.m_maximumEntryCount )
	{
		aurora::WeakReferenceHandle<12,15> h = weakreferenceManager.Add(ptr_diff, 1);				
		unsigned int weakRefAsValue = h;
		referenceCounter[h.m_index] = 1;
		return weakRefAsValue;
	}	
	else
	{
		extern void __cdecl method_assert(const char* message);
		method_assert("Insufficient tokens\r\n");
		return 0xFFFFFFFF;
	}
}

void  method_reftoken_inc(void* a, unsigned int token)
{
	//Increase the reference count for a token.
	char* baseImage = reinterpret_cast<char*>(a) + 4 + 4 + 4;
	baseImage = (char*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage)) + 4;
	
	unsigned int* referenceCounter = (unsigned int*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage));
	baseImage = baseImage + 4;
	aurora::WeakReferenceManager<12, 15>& weakreferenceManager = *(aurora::WeakReferenceManager<12, 15>*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage));

	aurora::WeakReferenceHandle<12, 15>& s = *(aurora::WeakReferenceHandle<12, 15>*)&token;	
	referenceCounter[s.m_index]++;
}

void  method_reftoken_dec(void* a, unsigned int token, method_releasetokensig sig)
{
	//Decrease the reference count for a token. A release function is supplemented so the tokens can released 
	char* baseImage = reinterpret_cast<char*>(a) + 4 + 4 + 4;
	baseImage = (char*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage)) + 4;

	unsigned int* referenceCounter = (unsigned int*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage));
	baseImage = baseImage + 4;
	aurora::WeakReferenceManager<12, 15>& weakreferenceManager = *(aurora::WeakReferenceManager<12, 15>*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage));

	aurora::WeakReferenceHandle<12, 15>& s = *(aurora::WeakReferenceHandle<12, 15>*)&token;	
	if( --referenceCounter[s.m_index] == 0) {				
		sig(a, token);
		weakreferenceManager.Remove(s);	
	}
}

void method_compare_and_set_token(void* a, unsigned int index, unsigned int token, method_releasetokensig sig)
{
	//Helper method to replace a token on the write adress.

	//Cache the tokens
	int parentToken = *static_cast<unsigned int*>(method_getpropertyaddr(a, index));
	int selfToken   = *static_cast<unsigned int*>(method_setpropertyaddr(a, index));
	
	//Increment the token I own.
	//method_reftoken_inc(a, selfToken);	

	//Increment the token of my parent.
	method_reftoken_inc(a, parentToken);	
	
	//Swawp the tokens
	*static_cast<unsigned int*>(method_setpropertyaddr(a, index)) = token;	
	
	//Decrement previous owned token
	//method_reftoken_dec(a, selfToken, &method_releasetoken);					

	//Decrement the token I used to own		
	method_reftoken_dec(a, selfToken, sig);
	
	//Decrement the token I used to own			
	method_reftoken_dec(a, parentToken, sig);	
}

void method_compare_and_swap_token(void* a, unsigned int index, method_releasetokensig sig)
{
	//Helper method to exchange tokens from a read adress and write adress.
	unsigned int d  = *static_cast<unsigned int*>(method_getpropertyaddr(a, index));
	unsigned int c  = *static_cast<unsigned int*>(method_setpropertyaddr(a, index));	
	if( d != c )
	{		
		method_reftoken_inc( a, d );
		*static_cast<unsigned int*>(method_setpropertyaddr(a, index)) = d;
		method_reftoken_dec( a, c, sig);
	}
}


void* method_gettoken(void* a, unsigned int token)
{
	//Get the memory associated with the token.
	char* baseImage = reinterpret_cast<char*>(a) + 4 + 4 + 4;
	baseImage = (char*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage));
	baseImage = baseImage + 8;
	aurora::WeakReferenceManager<12, 15>& weakreferenceManager = *(aurora::WeakReferenceManager<12, 15>*)(reinterpret_cast<char*>(baseImage)  + *reinterpret_cast<int*>(baseImage));
	baseImage = baseImage - 8;

	aurora::WeakReferenceHandle<12, 15>& s = *(aurora::WeakReferenceHandle<12, 15>*)&token;	
	return baseImage + (unsigned int)weakreferenceManager.Get(s);		
}

int  method_gettokentype(void* a, unsigned int token)
{	
	//Get the type of token, the type can be used to differentiate between a char* literal and a char* allocated at runtime. Conceptually 
	//a wide range of things can be expressable like this, collections, sets, dictionaries.
	aurora::WeakReferenceHandle<12, 15>& s = *(aurora::WeakReferenceHandle<12, 15>*)&token;	
	return s.m_type;
}





void* emitCharLiteral( bool preview, void* memory, const char* v)
{
	//Write a string literal to memory; preview is used to only participate in the memory adress computation.
	char* real = static_cast<char*>( memory );
	if( !preview )
	{
		strcpy(real, v);
		real[strlen(v)] = 0;
	}
	
	return real + strlen(v) + 1;
}

void* emitRelative( bool preview, void* memory, void* memoryAdress)
{
	//Write a 32-bit relative pointer to memory; preview is used to only participate in the memory adress computation.
	char* real = static_cast<char*>( memory );
	if( !preview )
	{		
		int   diff = static_cast<char*>( memoryAdress ) - static_cast<char*>( memory );
		char* v = reinterpret_cast<char*>( &diff );	
		real[0] = v[0];
		real[1] = v[1];
		real[2] = v[2];
		real[3] = v[3];
	}

	return real + 4;
}

void* emitInt32( bool preview, void* memory, int value )
{
	//Write a 32-bit integer to memory; preview is used to only participate in the memory adress computation.
	char* real = static_cast<char*>( memory );
	if( !preview )
	{		
		char*    v = reinterpret_cast<char*>( &value );
		real[0] = v[0];
		real[1] = v[1];
		real[2] = v[2];
		real[3] = v[3];
	}
	return real + 4;
}

void* emitFloat( bool preview, void* memory, float value )
{
	//Write a 32-bit floating point to memory; preview is used to only participate in the memory adress computation.
	char* real = static_cast<char*>( memory );
	if( !preview )
	{
		char*    v = reinterpret_cast<char*>( &value );
		real[0] = v[0];
		real[1] = v[1];
		real[2] = v[2];
		real[3] = v[3];
	}

	return real + 4;
}

void* emitCall(bool preview, void* memory, void* memoryAdress)
{
	//Write a x86 call instruction to memory.
	char* real = static_cast<char*>( memory );
	if( !preview )
	{
		int   diff = static_cast<char*>( memoryAdress ) - static_cast<char*>( memory ) - 5;
		char* v = reinterpret_cast<char*>( &diff );	
		real[0] = 0xE8;
		real[1] = v[0];
		real[2] = v[1];
		real[3] = v[2];
		real[4] = v[3];
	}
	
	return real + 5;
}

void* emitRet(bool preview, void* memory)
{
	//Write a x86 ret instruction to memory.
	char* real = static_cast<char*>( memory );
	if( !preview )
	{
		real[0] = 0xC3;
	}

	return real + 1;
}


void* emitJump(bool preview, void* memory, void* memoryAdress)
{
	//Write a x86 jump instruction to memory.
	char* real = static_cast<char*>( memory );
	if( !preview )
	{
		int   diff = static_cast<char*>( memoryAdress ) - static_cast<char*>( memory ) - 5;
		char* v = reinterpret_cast<char*>( &diff );	
		real[0] = 0xE9;
		real[1] = v[0];
		real[2] = v[1];
		real[3] = v[2];
		real[4] = v[3];
	}

	return real + 5;
}

void* emitPushEAX(bool preview, void* memory)
{
	//Write a x86 push eax instruction to memory.
	char* real = static_cast<char*>( memory );
	if( !preview )
	{
		real[0] = 0x50;
	}

	return real + 1;
}


void* emitPush8(bool preview,void* memory, int value)
{
	//Write a x86 push 8-bit integer instruction to memory.
	char* real = static_cast<char*>( memory );
	if( !preview )
	{
		char*    v = reinterpret_cast<char*>( &value );
		real[0] = 0x6A;
		real[1] = v[0];
	}
	return real + 2;
}

void* emitAddEsp8(bool preview,void* memory, int value)
{
	//Write a x86 add esp by a 8-bit integer instruction to memory.
	char* real = static_cast<char*>( memory );
	if( !preview )
	{
		char*    v = reinterpret_cast<char*>( &value );
		real[0] = 0x83;
		real[1] = 0xC4;
		real[2] = v[0];
	}
	return real + 3;
}

void* emitGetRelativeAdress(bool preview,void* memory, void* memoryAdress)
{
	//Write a x86 snippet to retrieve a relative pointer to the instruction pointer.
	char* real = static_cast<char*>( memory );
	if( !preview )
	{
		int   diff = static_cast<char*>( memoryAdress ) - static_cast<char*>( &real[11] );
		char* v = reinterpret_cast<char*>( &diff );	
		real[0] = 0xEB; //jmp 4 (@call real + 2)
		real[1] = 0x04;
		real[2] = 0x8B;	//mov ebp-4, eax
		real[3] = 0x04; 
		real[4] = 0x24;	
		real[5] = 0xC3;	//ret
		emitCall(preview, real + 6, real + 2 );		//call real + 2	
		real[11] = 0x05; //add eax + diff	
		real[12] = v[0];	
		real[13] = v[1];	
		real[14] = v[2];	
		real[15] = v[3];	
		//real[16] = 0xC3;
	}
	return real + 16;
}


void* emitPrologue(bool preview,void* memory, void* memoryAdress, int source)
{
	char* real = static_cast<char*>( memory );
	if( !preview )
	{
		char* v = reinterpret_cast<char*>( &memoryAdress );
		char* s = reinterpret_cast<char*>( &source );

		//Get relative adress from the instruction pointer (stored in eax).
		real = (char*)emitGetRelativeAdress(preview, real, memoryAdress);


		
		//int   diff = static_cast<char*>( memoryAdress ) - static_cast<char*>( memory ) - 5;
		//char* v = reinterpret_cast<char*>( &diff );	
		real[0] = 0x83; //mov ebp, ebp - 4
		real[1] = 0xC4;
		real[2] = 0x04;

		/*
		real[3] = 0xC7;
		real[4] = 0x04;
		real[5] = 0x24;
		real[6] = v[0];
		real[7] = v[1];
		real[8] = v[2];
		real[9] = v[3];
		*/

		real[3] = 0x89; //mov ebp, @eax
		real[4] = 0x04;
		real[5] = 0x24;
		real[6] = 0x90;	//nop;
		real[7] = 0x90;	//nop;
		real[8] = 0x90;	//nop;
		real[9] = 0x90;	//nop;

		  

		real[10] = 0x83; //mov ebp, ebp - 4
		real[11] = 0xC4;
		real[12] = 0x04;

		real[13] = 0xC7; //mov ebp, (@slot)
		real[14] = 0x04;
		real[15] = 0x24;
		real[16] = s[0];
		real[17] = s[1];
		real[18] = s[2];
		real[19] = s[3];

		real[20] = 0x83; //mov ebp, ebp + 8
		real[21] = 0xEC;
		real[22] = 0x08;

		/*
		real[10] = 0x83;
		real[11] = 0xEC;
		real[12] = 0x04;
		*/
	}

	return real + 23;
}



void* emitNodePrologue(bool preview,void* sequence, void* a, TiXmlElement* instanceInfo, TiXmlElement* typeInfo)
{	
	//Nodes the have multiple emanating arcs from a single output property must have a intermediate method generated 
	//that can activate all of the arcs. This is because our node structure reserves memory for one method per output 
	//link.
	void* methodBody = sequence;
	TiXmlElement* document = instanceInfo->Parent()->ToElement();
	TiXmlElement* node = document->FirstChildElement();
	for( ; node != 0; node = node->NextSiblingElement()) {
		if( stricmp(node->Value(), "Connection") == 0x0 ) {							
			char sourceEvent[1024]; char targetEvent[1024]; int targetSlot = -1, sourceSlot = -1;
			unsigned int source, target, elementId; 
			sscanf(node->Attribute("Source"), "%x.%s", &source, &sourceEvent);
			sscanf(node->Attribute("Target"), "%x.%s", &target, &targetEvent);
			sscanf(instanceInfo->Attribute("Id"), "%x", &elementId);
			if( elementId == source || elementId == target ) {
				if( stricmp(node->Attribute("Type"), "Property") == 0x0 ) 
				{					
					if( source == elementId ) 
					{												
						TiXmlElement* xnode = document->FirstChildElement();
						for( ; xnode != 0; xnode = xnode->NextSiblingElement()) {
							if( stricmp(xnode->Value(), "Node") == 0x0 ) {							
								unsigned deserializedId;
								sscanf(xnode->Attribute("Id"), "%x", &deserializedId);
								if( deserializedId == target ) {	
									sequence = emitGetRelativeAdress(preview, sequence, xnode->GetUserData());
									sequence = emitPush8(preview, sequence, 6);	
									sequence = emitPushEAX(preview, sequence);
									sequence = emitCall(preview, sequence, (char*)xnode->GetUserData() + method_pexec2 );
									sequence = emitAddEsp8(preview, sequence, 8);
								}
							}
						}
					}
				}
			}							
		}
	}

	sequence = emitRet(preview, sequence);
	emitJump(preview, (char*)a + method_pexec, methodBody );
	return sequence;
}





void emitPreNodeData(int version, TiXmlElement* instanceInfo, TiXmlElement* typeInfo, unsigned int& tokenCount, dictionary_strings& stringInterning  )
{
	if( version == 1 )
	{
		//Before the node data, is emited perform a pass for string-interning.
		TiXmlElement* node = typeInfo->FirstChildElement();
		for( ; node != 0; node = node->NextSiblingElement()) {
			if( stricmp(node->Value(), "Member") == 0x0  ) {
				if( node->Attribute("Type") == 0x0 ) 
				{				
					const char* v = instanceInfo->Attribute(node->Attribute("Name"));	
					stringInterning[std::string(v ? v : "")] = 0;
					tokenCount++;
				} 			
			}
		}
	}	
	else if( version == 2 )
	{
		TiXmlElement* node = typeInfo->FirstChildElement();
		for( ; node != 0; node = node->NextSiblingElement()) {
			if( stricmp(node->Value(), "Member") == 0x0  ) 
			{
				if( node->Attribute("Type") == 0x0 ) 
				{
					TiXmlElement* xnode = instanceInfo->FirstChildElement();
					for( ; xnode != 0; xnode = xnode->NextSiblingElement()) {
						if( stricmp(xnode->Value(), "Property") == 0x0 && stricmp(xnode->Attribute("Name"), node->Attribute("Name")) == 0x0) {				
							const char* v = xnode->Attribute("Value");
							stringInterning[std::string(v ? v : "")] = 0;
							tokenCount++;
						}
					}
				}
			}
		}
	}	
	else
	{
		assert(false);
	}
}

void* emitNodeDataInner(bool preview, void* sequence, void* a, TiXmlElement* instanceInfo, TiXmlElement* typeInfo, void* base, 
						unsigned int* referenceCounter, aurora::WeakReferenceManager<12, 15>& weakreferenceManager, 
						dictionary_strings& stringInterning,
						dictionary_nodexml& node_mappings2,						
						const char* propertyValue,
						const char* type,
						const char* types,						
						void*& userData)
{
	bool isspecial = types != 0x0;		
	if( type == 0x0 ) 
	{
		
		const char* v = propertyValue;								
		void* seq = stringInterning[std::string(v ? v : "")];
		assert(seq);

		void* ptr_diff = (void*)(static_cast<char*>(seq) - static_cast<char*>(base));
		aurora::WeakReferenceHandle<12,15> h = weakreferenceManager.Add(ptr_diff, 0);				
		referenceCounter[h.m_index] = 1;		
		//sequence = emitCharLiteral(sequence, v ? v : "");				

					
		unsigned int weakRefAsValue = h;
		if( isspecial ) sequence = emitInt32(preview, sequence, 0x000001);
		userData = sequence;
		sequence = emitInt32(preview, sequence, weakRefAsValue);

	} else if( stricmp(type, "vector3") == 0x0 ) {								
		const char* v = propertyValue;				
		float x, y, z; int d = sscanf( v, "%f %f %f", &x, &y, &z);
		if( isspecial ) sequence = emitInt32(preview, sequence, 0x000002);
		userData = sequence;
		sequence = emitFloat(preview, sequence, x);
		sequence = emitFloat(preview, sequence, y);
		sequence = emitFloat(preview, sequence, z);
	} else if( stricmp(type, "quaternion") == 0x0 ) {								
		const char* v = propertyValue;				
		float x, y, z, w; int d = sscanf( v, "%f %f %f %s", &x, &y, &z, &w);
		if( isspecial ) sequence = emitInt32(preview, sequence, 0x000003);
		userData = sequence;
		sequence = emitFloat(preview, sequence, x);
		sequence = emitFloat(preview, sequence, y);
		sequence = emitFloat(preview, sequence, z);
		sequence = emitFloat(preview, sequence, w);
	} else if( stricmp(type, "float") == 0x0 ) {				
		const char* v = propertyValue;				
		if( isspecial ) sequence = emitInt32(preview, sequence, 0x000004);
		userData = sequence;
		sequence = emitFloat(preview, sequence, atof(v ? v : "0"));
	} else if( stricmp(type, "int") == 0x0 ) {				
		const char* v = propertyValue;
		if( isspecial ) sequence = emitInt32(preview, sequence, 0x000005);
		userData = sequence;
		sequence = emitInt32(preview, sequence, atoi(v ? v : "0"));
	} else if( stricmp(type, "bool") == 0x0 ) {				
		const char* v = propertyValue;
		if( isspecial ) sequence = emitInt32(preview, sequence, 0x000006);
		userData = sequence;
		sequence = emitInt32(preview, sequence, atoi(v ? v : "0"));
	} else if( stricmp(type, "signal") == 0x0 ) {
		
		const char* name = propertyValue;
		void* mnode = 0, *f = a, *n = f;
		do {
			char* member = method_getnodename(f);		
			if( member != 0 && stricmp(member, name) == 0 ) mnode = f;
		} while( n != (f = method_next(f)) );
		if( mnode != 0 ) 
		{
			if( isspecial ) sequence = emitInt32(preview, sequence, 0x000007);
			userData = sequence;
			sequence = emitRelative(preview, sequence, mnode);
		} 
		else 
		{														
			//Deserialize the unique identifier for the node.
			unsigned int uniquenodeid; 
			sscanf(name, "%x", &uniquenodeid);
			std::map<unsigned int, TiXmlElement*>::iterator itt = node_mappings2.find(uniquenodeid);
			if( itt == node_mappings2.end() ) 
			{
				printf("Signal missing %s on node (%s:%s)\r\n", name, instanceInfo->Attribute("Id"), instanceInfo->Attribute("Name"));
				if( isspecial ) sequence = emitInt32(preview, sequence, 0x000007);
				userData = sequence;
				sequence = emitRelative(preview, sequence, sequence);
			}
			else 
			{						
				mnode = itt->second->GetUserData();
				if( isspecial ) sequence = emitInt32(preview, sequence, 0x000007);
				userData = sequence;
				sequence = emitRelative(preview, sequence, mnode);
			}
		}		
	} else {
		//Emit the data-member patch table..
		const char* v = propertyValue;				
		TiXmlElement* xnode = typeInfo->Parent()->FirstChildElement();
		for( ; xnode != 0; xnode = xnode->NextSiblingElement()) {
			if( stricmp( xnode->Value(), "Enum" ) == 0x0 && stricmp(xnode->Attribute("Name"),type) == 0x0 ) {
				TiXmlElement* enode = xnode->FirstChildElement();
				for( ; enode != 0; enode = enode->NextSiblingElement()) {				
					if( strcmp(enode->Attribute("Name"), v) == 0x0 ) 
					{
						unsigned int value;
						if( sscanf(enode->Attribute("Value"), "0x%X", &value) == 0 )
						{									
							if( isspecial ) sequence = emitInt32(preview, sequence, 0x000008);
							userData = sequence;
							sequence = emitInt32(preview, sequence, atoi(enode->Attribute("Value")));
						}
						else
						{																	
							if( isspecial ) sequence = emitInt32(preview, sequence, 0x000008);
							userData = sequence;
							sequence = emitInt32(preview, sequence, value);
						}								
					}
				}
			}
		}		
	}

	return sequence;
}

void* emitNodeData(bool preview, void* sequence, void* a, TiXmlElement* instanceInfo, TiXmlElement* typeInfo, void* base, unsigned int* referenceCounter, aurora::WeakReferenceManager<12, 15>& weakreferenceManager, dictionary_strings& stringInterning, dictionary_nodexml& node_mappings, dictionary_nodexml& node_mappings2 )
{
	//Emit the data-member patch table..
	TiXmlElement* node = typeInfo->FirstChildElement();
	for( ; node != 0; node = node->NextSiblingElement()) {
		if( stricmp(node->Value(), "Member") == 0x0 ) 
		{					
			const char* propertyValue = instanceInfo->Attribute(node->Attribute("Name"));
			const char* type = node->Attribute("Type");
			const char* types = node->Attribute("Types");
			bool isspecial = types != 0x0;					
			if( isspecial )
			{
				TiXmlElement* xnode = instanceInfo->FirstChildElement();
				for( ; xnode != 0; xnode = xnode->NextSiblingElement()) {
					if( stricmp(xnode->Value(), "Property") == 0x0 && stricmp(xnode->Attribute("Name"), node->Attribute("Name")) == 0x0) {
						type = xnode->Attribute("Type");						
						propertyValue = xnode->Attribute("Value");
					}
				}
			}
			else
			{
				TiXmlElement* xnode = instanceInfo->FirstChildElement();
				for( ; xnode != 0; xnode = xnode->NextSiblingElement()) {
					if( stricmp(xnode->Value(), "Property") == 0x0 && stricmp(xnode->Attribute("Name"), node->Attribute("Name")) == 0x0) {				
						propertyValue = xnode->Attribute("Value");
					}
				}
			}


			void* userData = 0;
			sequence = emitNodeDataInner(preview, sequence, a, instanceInfo, typeInfo, base, referenceCounter, weakreferenceManager, 
				stringInterning, /*node_mappings,*/ node_mappings2, propertyValue, type, types, userData);			
			node->SetUserData( userData );
		}
		else if( stricmp(node->Value(), "Generator") == 0x0 )
		{
			int count = 0;
			TiXmlElement* xnode = instanceInfo->FirstChildElement();
			for( ; xnode != 0; xnode = xnode->NextSiblingElement()) 
			{
				if( stricmp(xnode->Value(), "Property") == 0x0 && stricmp(xnode->Attribute("Name"), node->Attribute("Name")) == 0x0) 
				{
					count++;
				}
			}

			char value[32];
			sprintf(value, "%d", count );
			void* userData = 0;
			sequence = emitNodeDataInner(preview, sequence, a, instanceInfo, typeInfo, base, referenceCounter, weakreferenceManager, 
				stringInterning, /*node_mappings,*/ node_mappings2, value, "int", 0, userData);			
			node->SetUserData( userData );

			const char* propertyValue = instanceInfo->Attribute(node->Attribute("Name"));
			const char* type = node->Attribute("Type");
			const char* types = node->Attribute("Types");
			bool isspecial = types != 0x0;					
			xnode = instanceInfo->FirstChildElement();
			for( ; xnode != 0; xnode = xnode->NextSiblingElement()) 
			{
				if( stricmp(xnode->Value(), "Property") == 0x0 && stricmp(xnode->Attribute("Name"), node->Attribute("Name")) == 0x0) 
				{
					type = xnode->Attribute("Type");						
					propertyValue = xnode->Attribute("Value");

					void* userData = 0;
					sequence = emitNodeDataInner(preview, sequence, a, instanceInfo, typeInfo, base, referenceCounter, weakreferenceManager, 
						stringInterning, /*node_mappings,*/ node_mappings2, propertyValue, type, types, userData);			
					xnode->SetUserData( userData );
				}
			}
		}
	}

	node = typeInfo->FirstChildElement();	
	for( int i = 0; node != 0; node = node->NextSiblingElement(), i++) {
		if( stricmp(node->Value(), "Member") == 0x0 ) {											
			void* usr = node->GetUserData();
			node->SetUserData(0);			
			emitRelative(preview, reinterpret_cast<char*>(a) - 8 - i * 8, usr);
			emitRelative(preview, reinterpret_cast<char*>(a) - 4 - i * 8, usr);
		}
		else if( stricmp(node->Value(), "Generator") == 0x0 ) 
		{			
			void* usr = node->GetUserData();
			node->SetUserData(0);			
			emitRelative(preview, reinterpret_cast<char*>(a) - 8 - i * 8, usr);
			emitRelative(preview, reinterpret_cast<char*>(a) - 4 - i * 8, usr);
			i++;

			TiXmlElement* xnode = instanceInfo->FirstChildElement();
			for( ; xnode != 0; xnode = xnode->NextSiblingElement()) 
			{
				if( stricmp(xnode->Value(), "Property") == 0x0 && stricmp(xnode->Attribute("Name"), node->Attribute("Name")) == 0x0) 
				{
					void* usr = xnode->GetUserData();
					xnode->SetUserData(0);			
					emitRelative(preview, reinterpret_cast<char*>(a) - 8 - i * 8, usr);
					emitRelative(preview, reinterpret_cast<char*>(a) - 4 - i * 8, usr);
					i++;
				}
			}			
		}
	}	

	return sequence;
}

void* emitNode( bool preview, int nodeKind, void* memory, FlowHeader* header, void*& out, TiXmlElement* instanceInfo, TiXmlElement* typeInfo, void* base  )
{
	//Emit rdata
	void* uniqueName, *sequence = memory, *sequenceu;
	bool hasUniqueName = stricmp(instanceInfo->Attribute("Id"), instanceInfo->Attribute("Designer.Name")) != 0x0;
	if( hasUniqueName ) uniqueName = sequence, sequence = emitCharLiteral(preview, sequence, instanceInfo->Attribute("Designer.Name"));
	
	TiXmlElement* node = 0;
	node = typeInfo->LastChild()->ToElement();
	for( ; node != 0; node = node->PreviousSibling() == 0 ? 0 : node->PreviousSibling()->ToElement()) {
		if( stricmp(node->Value(), "Member") == 0x0 ) {											
			void* usr = node->GetUserData();
			node->SetUserData(0);
			sequence = emitRelative(preview, sequence, sequence);
			sequence = emitRelative(preview, sequence, sequence);
		}
		else if( stricmp(node->Value(), "Generator") == 0x0 ) 
		{		
			sequence = emitRelative(preview, sequence, sequence);
			sequence = emitRelative(preview, sequence, sequence);

			TiXmlElement* instanceNode = 0;
			instanceNode = instanceInfo->LastChild()->ToElement();
			for( ; instanceNode != 0; instanceNode = instanceNode->PreviousSibling() == 0 ? 0 : instanceNode->PreviousSibling()->ToElement()) {
				if( stricmp(instanceNode->Attribute("Name"), node->Attribute("Name")) == 0x0 ) {
					printf("Generated property\r\n");
					void* usr = instanceNode->GetUserData();
					instanceNode->SetUserData(0);
					sequence = emitRelative(preview, sequence, sequence);
					sequence = emitRelative(preview, sequence, sequence);
				}
			}
		}
	}

		
	//Emit l-table  data.
	if( header->tail != 0x0 )
	emitRelative( preview, header->tail, sequence);
	if( header->head == 0x0 )
	header->head = sequence;
	header->tail = sequence;

	out = sequence;
	sequence = emitRelative(preview, sequence, header->head);
	sequence = emitInt32(preview, sequence, nodeKind );
	sequence = hasUniqueName ? emitRelative(preview, sequence, uniqueName) : emitRelative(preview, sequence, sequence);
	sequence = emitRelative(preview, sequence, base);
	
	
	//Reserve space to hot-patch the dynamicly linked implementation method..
	sequenceu = sequence;
	sequence  = emitJump(preview, sequence, sequence);
	emitRet(preview, sequenceu);

	//Reserve space for the pre-execution function
	sequenceu = sequence;
	sequence  = emitJump(preview, sequence, sequence );
	emitRet(preview, sequenceu);

	//Reserve space for the pre-execution function
	sequenceu = sequence;
	sequence  = emitJump(preview, sequence, sequence );
	emitRet(preview, sequenceu);

	//Emit the output signals patch table..
	node = typeInfo->FirstChildElement();
	for( ; node != 0; node = node->NextSiblingElement()) {
		if( stricmp(node->Value(), "Signal") == 0x0 && stricmp(node->Attribute("Type"), "Out") == 0x0 ) 
		{							
			//Reserve space to hot-patch the sequence of bytes..
			sequenceu = sequence;
			sequence  = emitPrologue(preview, sequence, 0, -1);												
			sequence  = emitJump(preview, sequence, sequence);		
			emitRet(preview, sequenceu);

			//Reserve space to hot-patch the sequence of bytes..
			sequenceu = sequence;
			sequence  = emitPrologue(preview, sequence, 0, -1);												
			sequence  = emitJump(preview, sequence, &sequence);	
			emitRet(preview, sequenceu);
		}
	}	

	
	return sequence;
}

void* emitLink(void* node1, void* node2, int offset, int source)
{
	void* a = reinterpret_cast<char*>(node1) + method_table + (prologue_size * 2 * offset) + 0;
	void* s = reinterpret_cast<char*>(node1) + method_table + (prologue_size * 2 * offset) + prologue_jump;

	void* a1 = reinterpret_cast<char*>(node1) + method_table + (prologue_size * 2 * offset) + 0 + prologue_size;
	void* s1 = reinterpret_cast<char*>(node1) + method_table + (prologue_size * 2 * offset) + prologue_jump + prologue_size;


	//Overwrite the prologue so the correct node and connectors are referenced.
	emitPrologue( false, a, node2, source);
	//Overwrite the jump instruction from method_missing to the dynamic receptor of the linked node.
	emitJump( false, s, reinterpret_cast<char*>(node2) + method_exec);

	//Overwrite the prologue so the correct node and connectors are referenced.
	emitPrologue( false, a1, node2, source);
	//Overwrite the jump instruction from method_missing to the dynamic receptor of the linked node.
	emitJump( false, s1, reinterpret_cast<char*>(node2) + method_pexec);


	return 0;
}


void emitDataLink(void* node1, void* node2, int offset1, int offset2)
{
	//Links the data of two nodes together.
	//read ptr for property of node1
	void* a = reinterpret_cast<char*>(node1) - 4 - 8 * offset1;	
	//write-ptr for property of node2
	void* s = reinterpret_cast<char*>(node2) - 8 - 8 * offset2; 
	//set the read-ptr to the write-ptr.
	emitRelative(false, s, reinterpret_cast<char*>(a) + *(int*)a); 
}












void* emitNodeMultiConnections( bool preview, void* sequence, TiXmlDocument& doc1, dictionary_nodexml& node_mappings, dictionary_nodexml& node_mappings2)
{
	dictionary_connections connectionMap;
	
	
	TiXmlElement *node;
	node = doc1.FirstChildElement()->FirstChildElement();
	for( ; node != 0; node = node->NextSiblingElement()) {
		if( stricmp(node->Value(), "Connection") == 0x0 && stricmp(node->Attribute("Type"), "Element") == 0x0 ) {
			connectionMap.insert( 
				std::make_pair(
					node->Attribute("Target"),
					node
				)
			);
		}
	}
	



	int count = 0;
	dictionary_connections::iterator current = connectionMap.begin(), upper, lower2;
	while( current != connectionMap.end() ) 
	{
		lower2   = connectionMap.lower_bound( current->first );		
		upper   = connectionMap.upper_bound( current->first );		
		count   = connectionMap.count( current->first );	
		current = upper;


		if( count > 1 )
		{	
			char sourceEvent[1024]; char targetEvent[1024]; int targetSlot = -1, sourceSlot = -1;
			unsigned int source, target; 			


			void* methodBody1 = sequence;			
			for( dictionary_connections::iterator lower = lower2; lower != upper; lower++ ) 
			{				
				//Resolve the destination				
				TiXmlElement* connection = lower->second;
				sscanf(connection->Attribute("Source"), "%x.%s", &source, &sourceEvent);
				sscanf(connection->Attribute("Target"), "%x.%s", &target, &targetEvent);			
				strcpy(&sourceEvent[0], 1 + strstr(connection->Attribute("Source"), "."));
				strcpy(&targetEvent[0], 1 + strstr(connection->Attribute("Target"), "."));

				//Search for the method symbol
				TiXmlElement *xnode = node_mappings[source]->FirstChildElement();
				for( int signal = 0; xnode != 0; xnode = xnode->NextSiblingElement()) {
					if( stricmp(xnode->Value(), "Signal") == 0x0 && stricmp(xnode->Attribute("Type"), "In") == 0x0) { 						
						if( stricmp(xnode->Attribute("Name"), sourceEvent) == 0x0 ) {
							sourceSlot = signal;
							break;
						}
						else
						{
							signal++;
						}						
					}
				}

				xnode = node_mappings[target]->FirstChildElement();
				for( int signal = 0; xnode != 0; xnode = xnode->NextSiblingElement()) {
					if( stricmp(xnode->Value(), "Signal") == 0x0 && stricmp(xnode->Attribute("Type"), "Out") == 0x0 ) { 						
						if( stricmp(xnode->Attribute("Name"), targetEvent) == 0x0 ) {
							targetSlot = signal;
							break;
						}
						else
						{
							signal++;
						}						
					}
				}

				
				assert(targetSlot != -1);
				assert(sourceSlot != -1);		
				sequence = emitGetRelativeAdress(preview, sequence, node_mappings2[source]->GetUserData());
				sequence = emitPush8(preview, sequence, sourceSlot);	
				sequence = emitPushEAX(preview, sequence);
				sequence = emitCall(preview, sequence, (char*)node_mappings2[source]->GetUserData() + method_exec );
				sequence = emitAddEsp8(preview, sequence, 8);
			}

			sequence = emitRet(preview, sequence);

			void* methodBody2 = sequence;			
			for( dictionary_connections::iterator lower = lower2; lower != upper; lower++ ) 
			{				
				//Resolve the destination				
				TiXmlElement* connection = lower->second;
				sscanf(connection->Attribute("Source"), "%x.%s", &source, &sourceEvent);
				sscanf(connection->Attribute("Target"), "%x.%s", &target, &targetEvent);			
				strcpy(&sourceEvent[0], 1 + strstr(connection->Attribute("Source"), "."));
				strcpy(&targetEvent[0], 1 + strstr(connection->Attribute("Target"), "."));

				//Search for the method symbol
				TiXmlElement *xnode = node_mappings[source]->FirstChildElement();
				for( int signal = 0; xnode != 0; xnode = xnode->NextSiblingElement()) {
					if( stricmp(xnode->Value(), "Signal") == 0x0 && stricmp(xnode->Attribute("Type"), "In") == 0x0) { 						
						if( stricmp(xnode->Attribute("Name"), sourceEvent) == 0x0 ) {
							sourceSlot = signal;
							break;
						}
						else
						{
							signal++;
						}						
					}
				}

				xnode = node_mappings[target]->FirstChildElement();
				for( int signal = 0; xnode != 0; xnode = xnode->NextSiblingElement()) {
					if( stricmp(xnode->Value(), "Signal") == 0x0 && stricmp(xnode->Attribute("Type"), "Out") == 0x0 ) { 						
						if( stricmp(xnode->Attribute("Name"), targetEvent) == 0x0 ) {
							targetSlot = signal;
							break;
						}
						else
						{
							signal++;
						}						
					}
				}

				
				assert(targetSlot != -1);
				assert(sourceSlot != -1);		
				sequence = emitGetRelativeAdress(preview, sequence, node_mappings2[source]->GetUserData());
				sequence = emitPush8(preview, sequence, sourceSlot);	
				sequence = emitPushEAX(preview, sequence);
				sequence = emitCall(preview, sequence, (char*)node_mappings2[source]->GetUserData() + method_pexec );
				sequence = emitAddEsp8(preview, sequence, 8);
			}

			sequence = emitRet(preview, sequence);






			assert(targetSlot != -1);	
			void* rtarget = node_mappings2[target]->GetUserData(), *rsource = node_mappings2[source]->GetUserData();
			assert(rtarget != 0 );
			assert(rsource != 0 );
		

			void* a = reinterpret_cast<char*>(rtarget) + method_table + (prologue_size * 2 * targetSlot) + 0;
			void* s = reinterpret_cast<char*>(rtarget) + method_table + (prologue_size * 2 * targetSlot) + prologue_jump;
			void* a1 = reinterpret_cast<char*>(rtarget) + method_table + (prologue_size * 2 * targetSlot) + 0 + prologue_size;
			void* s1 = reinterpret_cast<char*>(rtarget) + method_table + (prologue_size * 2 * targetSlot) + prologue_jump + prologue_size;

			/*
			//Overwrite the prologue so the correct node and connectors are referenced.
			emitPrologue( false, a, node2, source);
			//Overwrite the jump instruction from method_missing to the dynamic receptor of the linked node.
			emitJump( false, s, reinterpret_cast<char*>(node2) + method_exec);
			*/

			emitJump( preview, a, methodBody1);
			emitJump( preview, a1, methodBody2);
		}				
	}


	/*
				emitLink(						
					node_mappings2[target]->GetUserData(),
					node_mappings2[source]->GetUserData(),
					targetSlot, sourceSlot
				);
	*/

	/*
	void* a = reinterpret_cast<char*>(node1) + method_table + (prologue_size * 2 * offset) + 0;
	void* s = reinterpret_cast<char*>(node1) + method_table + (prologue_size * 2 * offset) + prologue_jump;

	void* a1 = reinterpret_cast<char*>(node1) + method_table + (prologue_size * 2 * offset) + 0 + prologue_size;
	void* s1 = reinterpret_cast<char*>(node1) + method_table + (prologue_size * 2 * offset) + prologue_jump + prologue_size;


	//Overwrite the prologue so the correct node and connectors are referenced.
	emitPrologue( false, a, node2, source);
	//Overwrite the jump instruction from method_missing to the dynamic receptor of the linked node.
	emitJump( false, s, reinterpret_cast<char*>(node2) + method_exec);

	//Overwrite the prologue so the correct node and connectors are referenced.
	emitPrologue( false, a1, node2, source);
	//Overwrite the jump instruction from method_missing to the dynamic receptor of the linked node.
	emitJump( false, s1, reinterpret_cast<char*>(node2) + method_pexec);
	*/



	/*
	node = doc1.FirstChildElement()->FirstChildElement();
	for( ; node != 0; node = node->NextSiblingElement()) {
		if( stricmp(node->Value(), "Connection") == 0x0 && stricmp(node->Attribute("Type"), "Element") == 0x0 ) {
			char sourceEvent[1024]; char targetEvent[1024]; int targetSlot = -1, sourceSlot = -1;
			unsigned int source, target; 
			sscanf(node->Attribute("Source"), "%x.%s", &source, &sourceEvent);
			sscanf(node->Attribute("Target"), "%x.%s", &target, &targetEvent);			
			strcpy(&sourceEvent[0], 1 + strstr(node->Attribute("Source"), "."));
			strcpy(&targetEvent[0], 1 + strstr(node->Attribute("Target"), "."));

			
			const char* connectionSource = node->Attribute("Target");
			std::multimap<const char*, TiXmlElement*, ltstr>::iterator itt 
				= connectionMap.lower_bound( connectionSource  );
			std::multimap<const char*, TiXmlElement*, ltstr>::iterator upp 
				= connectionMap.upper_bound( connectionSource );

			int count = connectionMap.count( connectionSource );
			if( count > 1 ) 
			{
				continue;			
			}

						
			TiXmlElement *xnode = node_mappings[source]->FirstChildElement();
			for( int signal = 0; xnode != 0; xnode = xnode->NextSiblingElement()) {
				if( stricmp(xnode->Value(), "Signal") == 0x0 && stricmp(xnode->Attribute("Type"), "In") == 0x0) { 						
					if( stricmp(xnode->Attribute("Name"), sourceEvent) == 0x0 ) {
						sourceSlot = signal;
						break;
					}
					else
					{
						signal++;
					}						
				}
			}

			xnode = node_mappings[target]->FirstChildElement();
			for( int signal = 0; xnode != 0; xnode = xnode->NextSiblingElement()) {
				if( stricmp(xnode->Value(), "Signal") == 0x0 && stricmp(xnode->Attribute("Type"), "Out") == 0x0 ) { 						
					if( stricmp(xnode->Attribute("Name"), targetEvent) == 0x0 ) {
						targetSlot = signal;
						break;
					}
					else
					{
						signal++;
					}						
				}
			}

			if( sourceSlot != -1 && targetSlot != -1  )
			{
				emitLink(						
					node_mappings2[target]->GetUserData(),
					node_mappings2[source]->GetUserData(),
					targetSlot, sourceSlot
				);
			}
		}
	}
	*/


	return sequence;
}




void  parse_identifier(const char* value,  unsigned int& node, const char* name, unsigned int& offset )
{
	sscanf(value, "%x.%s", &node, name);
	strcpy(const_cast<char*>(name), 1 + strstr(value, "."));

	const char* dot = strstr(name, ".");
	if( dot ) {
		offset = atoi(dot + 1);
		*const_cast<char*>(dot) = 0;
	} else {
		offset = 0;
	}
}

void  parse_overwrite_data(TiXmlDocument& doc1, dictionary_nodexml& node_mappings, dictionary_nodexml& node_mappings2)
{
	dictionary_connections connectionMap;
	
	
	TiXmlElement *node;
	node = doc1.FirstChildElement()->FirstChildElement();
	for( ; node != 0; node = node->NextSiblingElement()) {
		if( stricmp(node->Value(), "Connection") == 0x0 && stricmp(node->Attribute("Type"), "Element") == 0x0 ) {
			connectionMap.insert( 
				std::make_pair(
					node->Attribute("Target"),
					node
				)
			);
		}
	}
	

	node = doc1.FirstChildElement()->FirstChildElement();
	for( ; node != 0; node = node->NextSiblingElement()) {
		if( stricmp(node->Value(), "Connection") == 0x0 && stricmp(node->Attribute("Type"), "Element") == 0x0 ) {
			char sourceEvent[1024]; char targetEvent[1024]; int targetSlot = -1, sourceSlot = -1;
			unsigned int source, target, sourceOff, targetOff; 
			parse_identifier( node->Attribute("Source"), source, sourceEvent, sourceOff );
			parse_identifier( node->Attribute("Target"), target, targetEvent, targetOff );

			
			const char* connectionSource = node->Attribute("Target");
			std::multimap<const char*, TiXmlElement*, ltstr>::iterator itt 
				= connectionMap.lower_bound( connectionSource  );
			std::multimap<const char*, TiXmlElement*, ltstr>::iterator upp 
				= connectionMap.upper_bound( connectionSource );

			int count = connectionMap.count( connectionSource );
			if( count > 1 ) 
			{
				continue;			
			}

					
			TiXmlElement *xnode = node_mappings[source]->FirstChildElement();
			for( int signal = 0; xnode != 0; xnode = xnode->NextSiblingElement()) {
				if( stricmp(xnode->Value(), "Signal") == 0x0 && stricmp(xnode->Attribute("Type"), "In") == 0x0) { 						
					if( stricmp(xnode->Attribute("Name"), sourceEvent) == 0x0 ) {
						sourceSlot = signal;
						break;
					}
					else
					{
						signal++;
					}						
				}
			}

			xnode = node_mappings[target]->FirstChildElement();
			for( int signal = 0; xnode != 0; xnode = xnode->NextSiblingElement()) {
				if( stricmp(xnode->Value(), "Signal") == 0x0 && stricmp(xnode->Attribute("Type"), "Out") == 0x0 ) { 						
					if( stricmp(xnode->Attribute("Name"), targetEvent) == 0x0 ) {
						targetSlot = signal;
						break;
					}
					else
					{
						signal++;
					}						
				}
			}

			if( sourceSlot != -1 && targetSlot != -1  )
			{
				emitLink(						
					node_mappings2[target]->GetUserData(),
					node_mappings2[source]->GetUserData(),
					targetSlot, sourceSlot
				);
			}
		}
	}

	node = doc1.FirstChildElement()->FirstChildElement();
	for( ; node != 0; node = node->NextSiblingElement()) {
		if( stricmp(node->Value(), "Connection") == 0x0 ) {
			char sourceEvent[1024]; char targetEvent[1024]; int targetSlot = -1, sourceSlot = -1;
			unsigned int source, target,  sourceOff = 0, targetOff = 0;
			parse_identifier( node->Attribute("Source"), source, sourceEvent, sourceOff );
			parse_identifier( node->Attribute("Target"), target, targetEvent, targetOff );
						
			TiXmlElement *xnode = node_mappings[source]->FirstChildElement();
			for( int signal = 0; xnode != 0; xnode = xnode->NextSiblingElement()) {
				if( stricmp(xnode->Value(), "Member") == 0x0  ) { 						
					if( stricmp(xnode->Attribute("Name"), sourceEvent) == 0x0 ) {
						sourceSlot = signal;
						break;
					}
					else
					{
						signal++;
					}						
				}
				else if( stricmp(xnode->Value(), "Generator") == 0x0 )
				{
					TiXmlElement *inode = node_mappings2[source]->FirstChildElement();
					for( int offset = 0; inode != 0; inode = inode->NextSiblingElement()) {					
						if( stricmp(xnode->Attribute("Name"), inode->Attribute("Name")) == 0x0 )  
						{						
							if( offset == sourceOff )
							{
								sourceSlot = signal;
								break;
							}
							else
							{
								offset++;
								signal++;
							}
						}
					}
				}
			}

			xnode = node_mappings[target]->FirstChildElement();
			for( int signal = 0; xnode != 0; xnode = xnode->NextSiblingElement()) {
				if( stricmp(xnode->Value(), "Member") == 0x0 ) { 						
					if( stricmp(xnode->Attribute("Name"), targetEvent) == 0x0 ) {
						targetSlot = signal;
						break;
					}
					else
					{
						signal++;
					}						
				}
				else if( stricmp(xnode->Value(), "Generator") == 0x0 )
				{
					signal++;
					TiXmlElement *inode = node_mappings2[target]->FirstChildElement();
					for( int offset = 0; inode != 0; inode = inode->NextSiblingElement()) {					
						if( stricmp(xnode->Attribute("Name"), inode->Attribute("Name")) == 0x0 )  
						{						
							if( offset == targetOff )
							{
								targetSlot = signal;
								break;
							}
							else
							{
								offset++;
								signal++;
							}
						}
					}
				}
			}

			if( sourceSlot != -1 && targetSlot != -1  )
			{
				emitDataLink(
					node_mappings2[target]->GetUserData(),
					node_mappings2[source]->GetUserData(),
					targetSlot, sourceSlot
				);
			}
		}
	}
}

void* parse(TiXmlDocument& doc2, TiXmlDocument& doc1, int& outputSize)
{
	if( doc1.Error() || doc1.FirstChildElement() == 0 || stricmp(doc1.FirstChildElement()->Value(), "Flow") != 0x0 )
	{
		printf("File contains errors\r\n");
		return 0;
	}
	else if( doc2.Error() || doc2.FirstChildElement() == 0 || stricmp(doc2.FirstChildElement()->Value(), "Flow") != 0x0 )
	{
		printf("Meta-model contains errors\r\n");
		return 0;
	}
	else
	{			
		int version = doc1.FirstChildElement()->Attribute("Version") ?  atoi(doc1.FirstChildElement()->Attribute("Version")) : 1;
		const int maxSize = 4096 * 11;
		FlowHeader header = { 0 };
		void* address, *sequence, *a, *b; 
		address = sequence = _aligned_malloc(maxSize, 4096);
		memset(address, 0x90,  maxSize);

		//Auxillary data structures
		TiXmlElement *node; void* weakReferenceTable, *countedReferenceTable;
		dictionary_ordeal  type_ordialmapping;
		dictionary_types   type_descriptor;
		dictionary_nodexml node_mappings;
		dictionary_nodexml node_mappings2;		
		dictionary_strings stringInterning;

		//Always construct one extra token.
		unsigned int tokenCount = 1; 

		


		//Generate node ordial mapping
		assert(doc2.FirstChildElement());
		assert(doc2.FirstChildElement()->FirstChildElement());
		node = doc2.FirstChildElement()->FirstChildElement();
		for( int lastOrdial = 0; node != 0; node = node->NextSiblingElement()) {
			if( stricmp(node->Value(), "Node") == 0x0 ) {
				type_descriptor.insert( std::make_pair( 
					std::string( node->Attribute("Name") ), node ) );
				type_ordialmapping[node->Attribute("Name")] = 
					lastOrdial++;
			}			
		}

		//Generate node mapping
		node = doc1.FirstChildElement()->FirstChildElement();
		for( ; node != 0; node = node->NextSiblingElement()) 
		{		
			if( stricmp(node->Value(), "Node") == 0x0 ) 
			{				
				//Deserialize the unique identifier for the node.
				unsigned int uniquenodeid; 
				sscanf(node->Attribute("Id"), "%x", &uniquenodeid);
				node_mappings[uniquenodeid] = type_descriptor[std::string(node->Attribute("Type"))];				
				node_mappings2[uniquenodeid] = node;
			}				
		}
		
		//Generate string interning and count number of token types.
		node = doc1.FirstChildElement()->FirstChildElement();
		for( ; node != 0; node = node->NextSiblingElement()) 
		{		
			if( stricmp(node->Value(), "Node") == 0x0 ) 
			{	
				//Deserialize the unique identifier for the node.
				unsigned int uniquenodeid; 
				sscanf(node->Attribute("Id"), "%x", &uniquenodeid);
				emitPreNodeData( version, node, node_mappings[uniquenodeid], tokenCount, stringInterning );				
			}
		}


		//Relptr:firstnode
		sequence = emitRelative(false, sequence, sequence);
		//Relptr:table_refcount
		sequence = emitRelative(false,sequence, sequence);
		//Relptr:table_reftokens
		sequence = emitRelative(false, sequence, sequence);
		//Relptr:global_data
		sequence = emitRelative(false, sequence, sequence);

		node = doc1.FirstChildElement()->FirstChildElement();
		for( ; node != 0; node = node->NextSiblingElement()) 
		{		
			if( stricmp(node->Value(), "Node") == 0x0 ) 
			{	
				unsigned int uniquenodeid; 
				sscanf(node->Attribute("Id"), "%x", &uniquenodeid);
				sequence = emitNode(false, type_ordialmapping[node->Attribute("Type")], sequence,
					&header, a, node, node_mappings[uniquenodeid], address );
				node->SetUserData(a);
				assert( ((char*)sequence - (char*)address) < maxSize );
			}
		}

		sequence = emitNodeMultiConnections(false, sequence, doc1, 
			node_mappings, node_mappings2);
		assert( ((char*)sequence - (char*)address) < maxSize );

		node = doc1.FirstChildElement()->FirstChildElement();
		for( ; node != 0; node = node->NextSiblingElement()) 
		{		
			if( stricmp(node->Value(), "Node") == 0x0 ) 
			{				
				unsigned int uniquenodeid; 
				sscanf(node->Attribute("Id"), "%x", &uniquenodeid);
				sequence = emitNodePrologue(false, sequence, node->GetUserData(), 
					node, node_mappings[uniquenodeid] );				
				assert( ((char*)sequence - (char*)address) < maxSize );
			}
		}
		
		//Allocate the token storage reference counter
		countedReferenceTable = sequence;
		unsigned int* referenceCounter = (unsigned int*)sequence;
		sequence = (char*)sequence + sizeof(unsigned int) * tokenCount;
		
		//Allocate the token storage.
		aurora::WeakReferenceManager<12, 15>* w = new (sequence) aurora::WeakReferenceManager<12, 15>(); sequence = w;	
		w->m_maximumEntryCount = tokenCount;
		weakReferenceTable = sequence;

		//Move the sequence to the dynamic array allocation
		sequence = (char*)sequence + 12;

		//Allocate the entries for the tokens.
		for( int i = 0; i < tokenCount; i++ ) {
			new( sequence ) aurora::WeakReferenceManager<12, 15>::WeakReferenceEntry();
			sequence = (char*)sequence + sizeof(aurora::WeakReferenceManager<12, 15>::WeakReferenceEntry);
			assert( ((char*)sequence - (char*)address) < maxSize );
		}

		//Initialize the token storage.
		assert( ((char*)sequence - (char*)address) < maxSize );
		w->Reset();


		dictionary_strings::iterator itt;
		for( itt = stringInterning.begin(); itt != stringInterning.end(); ++itt )
		{			
			stringInterning[itt->first] = sequence;
			sequence = emitCharLiteral(false, sequence, itt->first.c_str());				
			assert( ((char*)sequence - (char*)address) < maxSize );
		}
		
		//Emit all remaining node data.
		node = doc1.FirstChildElement()->FirstChildElement();
		for( ; node != 0; node = node->NextSiblingElement()) 
		{		
			if( stricmp(node->Value(), "Node") == 0x0 ) 
			{	
				//Deserialize the unique identifier for the node.
				unsigned int uniquenodeid; 
				sscanf(node->Attribute("Id"), "%x", &uniquenodeid);
				sequence = emitNodeData(false,
					sequence, node->GetUserData(), node, node_mappings[uniquenodeid], address, referenceCounter, 
					*w, stringInterning, node_mappings, node_mappings2);
				assert( ((char*)sequence - (char*)address) < maxSize );
			}
		}
		

		parse_overwrite_data(doc1, node_mappings, node_mappings2);		
		emitRelative(false, (char*)address + ptr_size * 0, header.head);
		emitRelative(false, (char*)address + ptr_size * 1, countedReferenceTable);		
		emitRelative(false, (char*)address + ptr_size * 2, weakReferenceTable);


		outputSize = maxSize;
		int size =  (char*)sequence - (char*)address ;
		assert( size < maxSize );
		return address;
	}			
}

void* parse(const char* metaformat, const char* filename, int& outputSize)
{
	if( metaformat == 0x0 )
	{
		printf("No meta-format specified\r\n");
		return 0;
	}

	if( filename == 0x0 )
	{
		printf("No filename specified\r\n");
		return 0;
	}

	TiXmlDocument doc1, doc2;
	doc1.LoadFile(filename);
	doc2.LoadFile(metaformat);
	return parse(doc2, doc1, outputSize);
}

void dynamic_receptors_link(function_dictionary const& functionMapping, function_dictionary const& coerceMapping, method_signature def, void* adress)
{
	function_dictionary::const_iterator itt;
	void *f = method_getfirst(adress), *n = f;
	if( f != 0 ) do 
	{
		//Get the type of node, which is the ordial of the node, in our schema format.
		void* prev = f;
		int kind = method_getnodekind(prev);			
			
		//Match the type of the node to a function where that we want to link in. Currently all functions 
		//are linked in this way, even built-ins. As long as we have a function that follows the abi (__cdecl)
		//we're happy. So conceptually we can get the functions from our runtime, or even a dynamicly reflected 
		//dll.
		itt = functionMapping.find(kind);
		if( itt != functionMapping.end() ) {				
			emitJump(false, reinterpret_cast<char*>(prev) + method_exec, itt->second);
		} else {
			if( def != 0x0 ) {
				emitJump(false, reinterpret_cast<char*>(prev) + method_exec, def);
			}
		}
		
		//Perform the same type of mapping but also for preactivation functions. When a node gets activated, each of the node
		//it's referencing for it's data-dependecies gets preactivated. At this point, it can perform any number of tasks to 
		//update its value. For example 'random number between 10 and 100', 'get local player'
		itt = coerceMapping.find(kind);
		if( itt != coerceMapping.end() ) {				
			emitJump(false, reinterpret_cast<char*>(prev) + method_pexec2, itt->second);
		}		

	} 	
	while( n != (f = method_next(f)) );
}


void* load(void* global, void* address, int maxSize)
{
	//Set global data pointer, for example a pointer to an object, or a level.
	char* baseImage = (char*)address; baseImage += 12;
	*reinterpret_cast<void**>(baseImage) = global;	

	#ifdef WIN32 || WIN64
		//Change the virtual protection mode to permit executing the memory. Conceptually for linux, mprotect can
		//be used in the following line.
		DWORD oldExecutingMode;
		VirtualProtect(address, maxSize, PAGE_EXECUTE_READWRITE, &oldExecutingMode );
		//Flush the instruction cache to ensure, the cache doesn't hold a wrong view of the memory.
		FlushInstructionCache(GetCurrentProcess(), address, maxSize );
		return address;
	#else		
		#error "Platform not supported"
	#endif
}

void unload(void* address)
{
	#ifdef WIN32 || WIN64
		//Reset the virtual procetion mode to the default read/write memory. Conceptually for linux, mprotect can
		//be used in the following line.
		DWORD oldExecutingMode;
		VirtualProtect(address, 4096, PAGE_READWRITE, &oldExecutingMode );  
		//Free the actual memory.
		_aligned_free(address);
	#else		
		#error "Platform not supported"
	#endif
}


