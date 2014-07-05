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

#include <map>

template<class _Ty> class flow_allocator : public std::allocator<_Ty>
{
public:
	void construct(pointer _Ptr, const _Ty& _Val)
	{	
		::new (_Ptr) _Ty(_Val);
	}
	flow_allocator() : allocator() { }
	flow_allocator(const flow_allocator& other) throw() : allocator(other)
	{
		
	}

	template <class U> flow_allocator(const flow_allocator<U>& alloc) throw() : allocator(alloc)
	{

	}


	pointer allocate(size_type n)
	{	
		if (n <= 0)
			n = 0;
		else if (((size_t)(-1) / n) < sizeof(_Ty))
			throw std::bad_alloc();		
		const int size = n * sizeof(_Ty);
		_Ty* ptr = static_cast<_Ty*>(userAlloc(size));
		if( !ptr ) 
			throw std::bad_alloc();		
		return ptr;
	}

	pointer allocate(size_type n, allocator<void>::const_pointer hint)
	{	
		return allocate(n);
	}

	void deallocate (pointer p, size_type n)
	{
		return userFree(p);
	}

    template <class Type> struct rebind {
		typedef flow_allocator<Type> other;
	};

	void* userAlloc(int size)
	{
		//MemoryService::system->allocate(size, 0)
		return malloc(size);
	}

	void userFree(void* ptr)
	{
		//MemoryService::system->deallocate(p)
		free(ptr);
	}

};

//Imported methods
template<typename T> T& method_getproperty(void* a, int offset)
{
	extern void* method_getpropertyaddr(void* a, int offset);
	return *static_cast<T*>(method_getpropertyaddr(a, offset));
}

template<typename T> T& method_setproperty(void* a, int offset)
{
	extern void* method_setpropertyaddr(void* a, int offset);
	return *static_cast<T*>(method_setpropertyaddr(a, offset));
}

template<typename T> T& method_readproperty(void* a, int offset)
{
	extern void* method_readpropertyaddr(void* a, int offset);
	return *static_cast<T*>(method_readpropertyaddr(a, offset));
}



typedef void (__cdecl *method_signature)(void* a, int signal);
typedef void (__cdecl *method_releasetokensig)(void* a, unsigned int token);
typedef std::map<int, method_signature, std::less<int>, flow_allocator<std::pair<int, method_signature>>> 
	function_dictionary;


extern void* method_gettoken(void* a, unsigned int token);
extern unsigned int method_createtoken(void* a, void* ptr);
extern int  method_gettokentype(void* a, unsigned int token);
extern void method_reftoken_inc(void* a, unsigned int token);
extern void method_reftoken_dec(void* a, unsigned int token, method_releasetokensig);
extern void method_compare_and_set_token(void* a, unsigned int index, unsigned int token, method_releasetokensig sig);
extern void method_compare_and_swap_token(void* a, unsigned int index, method_releasetokensig sig);

class TiXmlDocument;
extern void* parse(TiXmlDocument& metaformat, TiXmlDocument& filename, int& outputSize);
extern void* parse(const char* metaformat, const char* filename, int& outputSize);
extern int  method_getnodekind(void* a);
extern void* method_getfirst(void* a);
extern void* method_next(void* a);
extern char* method_getnodename(void* a);
extern void* method_getbyname(void* a, const char* name);
extern unsigned int method_num_active_tokens(void* a);

extern void method_raise(void* a, int offset);
extern void dynamic_receptors_link(function_dictionary const& functionMapping, function_dictionary const& coerceMapping, method_signature def, void* adress);
extern void  setglobal(void* global, void* address);
extern void* load(void* global, void* address, int maxSize);
extern void  unload(void* address);


extern void* method_get_global(void* a);
extern void  method_set_global(void* a, void* addr);