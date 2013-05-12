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
 * Source: http://gamesfromwithin.com/managing-data-relationships
 *
 */
#pragma once
namespace aurora
{
	template<int N=12, int C=15> struct WeakReferenceHandle
	{
		WeakReferenceHandle() : m_index(0), m_counter(0), m_type(0) 
		{}

		WeakReferenceHandle(unsigned int index, unsigned int counter, unsigned int type)
			: m_index(index), m_counter(counter), m_type(type)
		{}

		inline operator unsigned int() const
		{
			return m_type << (N + C) | m_counter << N | m_index;
		}

		unsigned int m_index		: N;
		unsigned int m_counter	: C;
		unsigned int m_type		: ((sizeof(unsigned int) * 8) - N - C);
	};

	
	
	template<int N=12, int C=15> class WeakReferenceManager
	{
	public:
		typedef WeakReferenceHandle<N, C> WeakReference;
		//enum { MaxEntries = 64 };

		WeakReferenceManager();
		~WeakReferenceManager(){}
		
		WeakReference Add(void* p, unsigned int type);
		void   Update(WeakReference handle, void* p);
		void   Remove(WeakReference handle);
		void   Reset();		
		void*  Get(WeakReference handle) const;
		bool   Get(WeakReference handle, void*& out) const;
		int	   Count() const;

	private:
		WeakReferenceManager(const WeakReferenceManager&);
		WeakReferenceManager& operator=(const WeakReferenceManager&);
	public:

		struct WeakReferenceEntry
		{
			WeakReferenceEntry();
			explicit WeakReferenceEntry(unsigned int nextFreeIndex);
			
			unsigned int m_nextFreeIndex : N;
			unsigned int m_counter	   : C;
			unsigned int m_active		   : 1;
			unsigned int m_endOfList	   : 1;
			void* m_entry;
		};
				
		signed int		   m_activeEntryCount;		
		unsigned int	   m_firstFreeEntry;
		unsigned int	   m_maximumEntryCount;
		WeakReferenceEntry m_entries[0];
	};


	template<int N, int C> WeakReferenceManager<N,C>::WeakReferenceEntry::WeakReferenceEntry() 
		: m_nextFreeIndex(0)
		, m_counter(1)
		, m_active(0)
		, m_endOfList(0)
		, m_entry(0)
	{}

	template<int N, int C> WeakReferenceManager<N,C>::WeakReferenceEntry::WeakReferenceEntry(unsigned int nextFreeIndex)
		: m_nextFreeIndex(nextFreeIndex)
		, m_counter(1)
		, m_active(0)
		, m_endOfList(0)
		, m_entry(0)
	{}

	template<int N, int C> WeakReferenceManager<N,C>::WeakReferenceManager() : m_maximumEntryCount(0)
	{
				
	}

	template<int N, int C> void WeakReferenceManager<N,C>::Reset()
	{
		m_activeEntryCount = 0;
		m_firstFreeEntry = 0;

		for (int i = 0; i < m_maximumEntryCount - 1; ++i)
			m_entries[i] = WeakReferenceEntry(i + 1);
		m_entries[m_maximumEntryCount - 1] = WeakReferenceEntry();
		m_entries[m_maximumEntryCount - 1].m_endOfList = true;
	}


	template<int N, int C> WeakReferenceHandle<N, C> WeakReferenceManager<N,C>::Add(void* p, unsigned int type)
	{
		const int newIndex = m_firstFreeEntry;

		m_firstFreeEntry = m_entries[newIndex].m_nextFreeIndex;
		m_entries[newIndex].m_nextFreeIndex = 0;
		m_entries[newIndex].m_counter = m_entries[newIndex].m_counter + 1;
		if (m_entries[newIndex].m_counter == 0)
			m_entries[newIndex].m_counter = 1;
		m_entries[newIndex].m_active = true;
		m_entries[newIndex].m_entry = p;

		++m_activeEntryCount;

		return WeakReference(newIndex, m_entries[newIndex].m_counter, type);
	}


	template<int N, int C> void WeakReferenceManager<N,C>::Update(WeakReference handle, void* p)
	{
		const int index = handle.m_index;

		m_entries[index].m_entry = p;
	}


	template<int N, int C> void WeakReferenceManager<N,C>::Remove(const WeakReference handle)
	{
		const unsigned int index = handle.m_index;

		m_entries[index].m_nextFreeIndex = m_firstFreeEntry;
		m_entries[index].m_active = 0;
		m_firstFreeEntry = index;

		--m_activeEntryCount;
	}


	template<int N, int C> void* WeakReferenceManager<N,C>::Get(WeakReference handle) const
	{
		void* p = 0;
		if (!Get(handle, p))
			return 0;
		return p;
	}


	template<int N, int C> bool WeakReferenceManager<N,C>::Get(const WeakReference handle, void*& out) const
	{
		const int index = handle.m_index;
		if (m_entries[index].m_counter != handle.m_counter ||
			m_entries[index].m_active == false)
			return false;

		out = m_entries[index].m_entry;
		return true;
	}


	template<int N, int C> int WeakReferenceManager<N,C>::Count() const
	{
		return m_activeEntryCount;
	}

};