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

#include <stddef.h>
#include "delegate.h"

Delegate::Delegate()
{
	this->functions = NULL;
}

bool Delegate::Add(DelegateCallback func)
{
		//Loop over all functions see if it already exists
		//and return false if it does.
		__DelegateItem* p = this->functions;
		__DelegateItem* p2 = NULL;
		while( p != NULL )
		{
			if( p->function == func )
				return false;
			p = p->next;
			p2 = p;
		};
		

		//Function callback already exists, we use p2 as a helper 
		//to quickly see our p before it becomes NULL
		if( p2 == NULL )
		{
			this->functions = new __DelegateItem();
			this->functions->function = func;
			this->functions->next = NULL;
		}
		else
		{
			p2->next = new __DelegateItem();
			p2->next->function = func;
			p2->next->next = NULL;
		}


		return true;
}

bool Delegate::Remove(DelegateCallback func)
{
		//Loop over all functions see if it already exists
		//and return true if it does remove it.
		__DelegateItem* p = this->functions;
		__DelegateItem* p2 = NULL;
		while( p != NULL )
		{
			if( p->function == func )
			{	
				if( p2 != NULL )
					p2->next = p->next;
				else
					this->functions = p->next;

				delete p;

				return true;
			}

			p = p->next;
			p2 = p;
		};
		

		//The function is not found so return false
		return false;
}

void Delegate::Execute()
{
	//Loop over all functions see if it already exists
	//and return false if it does.
	__DelegateItem* p = this->functions;
	while( p != NULL )
	{
		if( p->function != NULL )
		{
			
			//Excute function
			DelegateCallback c = p->function;

			//Need to set p = p->next before calling the function because if we remove it 
			//we get a exception that. Because we're trying to delete element p while element p is still
			//on our stack.
			p = p->next;
			(*c)();
		}
		else
		{
			//Just to prevent infinite loops
			p = p->next;
		}
	};
}