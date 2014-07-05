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


#pragma once
namespace flow {

	struct Comment
	{
	
		enum
		{
			SymbolId = 0x0000
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Max,
			};
		};
	};

	struct Signal
	{
	
		enum
		{
			SymbolId = 0x0001
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Max,
			};
		};
	};

	struct Raise
	{
	
		enum
		{
			SymbolId = 0x0002
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Signal,
				Max,
			};
		};
	};

	struct Assert
	{
	
		enum
		{
			SymbolId = 0x0003
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Message,
				Max,
			};
		};
	};

	struct Message
	{
	
		enum
		{
			SymbolId = 0x0004
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Message,
				Delay,
				Argument,
				Max,
			};
		};
	};

	struct Sequence
	{
	
		enum
		{
			SymbolId = 0x0005
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Max,
			};
		};
	};

	struct CycleInt32
	{
	
		enum
		{
			SymbolId = 0x0006
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Offset,
				Current,
				Maximum,
				Value,
				Max,
			};
		};
	};

	struct AddInt32
	{
	
		enum
		{
			SymbolId = 0x0007
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				C,
				Max,
			};
		};
	};

	struct SubtractInt32
	{
	
		enum
		{
			SymbolId = 0x0008
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				C,
				Max,
			};
		};
	};

	struct MultiplyInt32
	{
	
		enum
		{
			SymbolId = 0x0009
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				C,
				Max,
			};
		};
	};

	struct DivideInt32
	{
	
		enum
		{
			SymbolId = 0x000A
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				C,
				Max,
			};
		};
	};

	struct AddFloat
	{
	
		enum
		{
			SymbolId = 0x000B
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				C,
				Max,
			};
		};
	};

	struct SubtractFloat
	{
	
		enum
		{
			SymbolId = 0x000C
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				C,
				Max,
			};
		};
	};

	struct MultiplyFloat
	{
	
		enum
		{
			SymbolId = 0x000D
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				C,
				Max,
			};
		};
	};

	struct DivideFloat
	{
	
		enum
		{
			SymbolId = 0x000E
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				C,
				Max,
			};
		};
	};

	struct CompareBool
	{
	
		enum
		{
			SymbolId = 0x000F
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				Max,
			};
		};
	};

	struct CompareInt32
	{
	
		enum
		{
			SymbolId = 0x0010
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				Max,
			};
		};
	};

	struct CompareFloat
	{
	
		enum
		{
			SymbolId = 0x0011
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				Max,
			};
		};
	};

	struct CounterInt32
	{
	
		enum
		{
			SymbolId = 0x0012
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				Amount,
				Result,
				Max,
			};
		};
	};

	struct CounterFloat
	{
	
		enum
		{
			SymbolId = 0x0013
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				Amount,
				Result,
				Max,
			};
		};
	};

	struct ParameterString
	{
	
		enum
		{
			SymbolId = 0x0014
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Value,
				Max,
			};
		};
	};

	struct ParameterInt32
	{
	
		enum
		{
			SymbolId = 0x0015
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Value,
				Max,
			};
		};
	};

	struct RandomInt32
	{
	
		enum
		{
			SymbolId = 0x0016
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Value,
				Minimum,
				Maximum,
				Max,
			};
		};
	};

	struct ParameterFloat
	{
	
		enum
		{
			SymbolId = 0x0017
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Value,
				Max,
			};
		};
	};

	struct RandomFloat
	{
	
		enum
		{
			SymbolId = 0x0018
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Value,
				Minimum,
				Maximum,
				Max,
			};
		};
	};

	struct Concatenate
	{
	
		enum
		{
			SymbolId = 0x0019
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				C,
				Max,
			};
		};
	};

	struct Delay
	{
	
		enum
		{
			SymbolId = 0x001A
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Delay,
				Max,
			};
		};
	};

	struct Sin
	{
	
		enum
		{
			SymbolId = 0x001B
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				Result,
				Max,
			};
		};
	};

	struct Cos
	{
	
		enum
		{
			SymbolId = 0x001C
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				Result,
				Max,
			};
		};
	};

	struct Tan
	{
	
		enum
		{
			SymbolId = 0x001D
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				Result,
				Max,
			};
		};
	};

	struct ArcSin
	{
	
		enum
		{
			SymbolId = 0x001E
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				Result,
				Max,
			};
		};
	};

	struct ArcCos
	{
	
		enum
		{
			SymbolId = 0x001F
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				Result,
				Max,
			};
		};
	};

	struct ArcTan
	{
	
		enum
		{
			SymbolId = 0x0020
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				Result,
				Max,
			};
		};
	};

	struct SinHyperbolic
	{
	
		enum
		{
			SymbolId = 0x0021
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				Result,
				Max,
			};
		};
	};

	struct CosHyperbolic
	{
	
		enum
		{
			SymbolId = 0x0022
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				Result,
				Max,
			};
		};
	};

	struct TanHyperbolic
	{
	
		enum
		{
			SymbolId = 0x0023
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				Result,
				Max,
			};
		};
	};

	struct Max
	{
	
		enum
		{
			SymbolId = 0x0024
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				Result,
				Max,
			};
		};
	};

	struct Min
	{
	
		enum
		{
			SymbolId = 0x0025
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				A,
				B,
				Result,
				Max,
			};
		};
	};

	struct KeyPressed
	{
	
		enum
		{
			SymbolId = 0x0026
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Key,
				Max,
			};
		};
	};

	struct QueryMenuState
	{
	
		enum
		{
			SymbolId = 0x0027
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Value,
				Max,
			};
		};
	};

	struct QueryMenuSubState
	{
	
		enum
		{
			SymbolId = 0x0028
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Value,
				Max,
			};
		};
	};

	struct ChangeMenuState
	{
	
		enum
		{
			SymbolId = 0x0029
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				StateId,
				Max,
			};
		};
	};

	struct ChangeMenuSubState
	{
	
		enum
		{
			SymbolId = 0x002A
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				SubStateId,
				Max,
			};
		};
	};

	struct UseWeapon
	{
	
		enum
		{
			SymbolId = 0x002B
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Max,
			};
		};
	};

	struct Spawn
	{
	
		enum
		{
			SymbolId = 0x002C
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				SpawnClass,
				SpawnScale,
				SpawnOrientation,
				SpawnPoint,
				Max,
			};
		};
	};

	struct ChangeHealth
	{
	
		enum
		{
			SymbolId = 0x002D
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Unit,
				Amount,
				Max,
			};
		};
	};

	struct QueryHealthOfUnit
	{
	
		enum
		{
			SymbolId = 0x002E
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Unit,
				Current,
				Maximum,
				Max,
			};
		};
	};

	struct GFXSetVariable
	{
	
		enum
		{
			SymbolId = 0x002F
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Movie,
				Variable,
				Value,
				Max,
			};
		};
	};

	struct GFXGetVariable
	{
	
		enum
		{
			SymbolId = 0x0030
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Movie,
				Variable,
				Value,
				Max,
			};
		};
	};

	struct QueryLocationOfUnit
	{
	
		enum
		{
			SymbolId = 0x0031
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Unit,
				Orientation,
				Location,
				Max,
			};
		};
	};

	struct GFXGetVariable2
	{
	
		enum
		{
			SymbolId = 0x0032
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Movie,
				Variable,
				Value,
				Max,
			};
		};
	};

	struct GFXCall
	{
	
		enum
		{
			SymbolId = 0x0033
		};
	
		struct Internal;
		friend Internal;
	
		struct Properties
		{
			enum
			{
				Movie,
				Variable,
				Max,
			};
		};
	};


}
