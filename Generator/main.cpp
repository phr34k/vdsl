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
#include "tinyxml.h"
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <cstdio>

struct helper
{
	FILE* fRead, * fFile;
	static DWORD __stdcall Message(void* param)
	{					
		FILE* fRead = static_cast<helper*>(param)->fRead;
		FILE* fFile = static_cast<helper*>(param)->fFile;		
		char buffer[4096]; int readBytes = 0, fileLength = 0;
		char rbuffer[4096]; int rBytes = 0;

		fseek(fFile, 0, SEEK_SET);
		do
		{
			readBytes = fread(&buffer, 1, 4096, fRead);							
			fileLength += readBytes;
			if( readBytes != 0 )
			{
				rBytes = fread(&rbuffer, 1, 4096, fFile);				

				if( rBytes != readBytes )
				{
					fseek(fFile, -rBytes, SEEK_CUR);
					fwrite(buffer, 1,  readBytes, fFile);
				}
				else if( memcmp(&buffer, &rbuffer, rBytes) != 0)
				{
					fseek(fFile, -rBytes, SEEK_CUR);
					fwrite(buffer, 1,  readBytes, fFile);									
				}														
			}							
		}
		while(readBytes != 0);						
		int filedes = _fileno(fFile);
		_chsize_s(filedes, fileLength);
		fclose(fFile);

		return 0;
	};
};


void output(FILE* fs, const char* filename)
{
	//fprintf(fWrite, "#pragma once\n");
	//fprintf(fWrite, "namespace flow {\n");			

	TiXmlDocument doc1; TiXmlElement *node;
	doc1.LoadFile(filename);
	if( doc1.Error() || doc1.FirstChildElement() == 0 || stricmp(doc1.FirstChildElement()->Value(), "Flow") != 0x0 )
	{
		printf("File contains errors\n");
		return;
	}
	else
	{
		//FILE* fs = fopen(ofilename, "w");		



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  
 
 
 


































		
		fprintf(fs, "/**\n");
		fprintf(fs, " * Domain Specific Visual Programming Language\n");
		fprintf(fs, " * Copyright (c) 2013 NHTV UNIVERSITY OF APPLIED SCIENCES\n");
		fprintf(fs, " * All rights reserved.\n");
		fprintf(fs, " *\n");
		fprintf(fs, " * Redistribution and use in source and binary forms, with or without\n");
		fprintf(fs, " * modification, are permitted provided that the following conditions\n");
		fprintf(fs, " * are met:\n");
		fprintf(fs, " * 1. Redistributions of source code must retain the above copyright\n");
		fprintf(fs, " *    notice, this list of conditions and the following disclaimer.\n");
		fprintf(fs, " * 2. Redistributions in binary form must reproduce the above copyright\n");
		fprintf(fs, " *    notice, this list of conditions and the following disclaimer in the\n");
		fprintf(fs, " *    documentation and/or other materials provided with the distribution.\n");
		fprintf(fs, " * 3. Neither the name of the copyright holders nor the names of its\n");
		fprintf(fs, " *    contributors may be used to endorse or promote products derived from\n");
		fprintf(fs, " *    this software without specific prior written permission.\n");
		fprintf(fs, " *\n");
		fprintf(fs, " * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n");
		fprintf(fs, " * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n");
		fprintf(fs, " * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n");
		fprintf(fs, " * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE\n");
		fprintf(fs, " * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n");
		fprintf(fs, " * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\n");
		fprintf(fs, " * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\n");
		fprintf(fs, " * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\n");
		fprintf(fs, " * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n");
		fprintf(fs, " * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF\n");
		fprintf(fs, " * THE POSSIBILITY OF SUCH DAMAGE.\n");
		fprintf(fs, " *\n");
		fprintf(fs, " * Author: Lawrence Kok\n");
		fprintf(fs, " *\n");
		fprintf(fs, " */\n");
		fprintf(fs, "\n");
		fprintf(fs, "\n");
	
		fprintf(fs, "#pragma once\n");
		fprintf(fs, "namespace flow {\n");
		fprintf(fs, "\n");
		
		int autoId = 0;
		node = doc1.FirstChildElement()->FirstChildElement();
		for( ; node != 0; node = node->NextSiblingElement()) 
		{		
			if( stricmp(node->Value(), "Node") == 0x0 ) 
			{	
				fprintf(fs, "\tstruct %s\n", node->Attribute("Name"));		
				fprintf(fs, "\t{\n");	
				fprintf(fs, "\t\n");
				fprintf(fs, "\t\tenum\n");	
				fprintf(fs, "\t\t{\n");	
				fprintf(fs, "\t\t\tSymbolId = 0x%04X\n", autoId++);	
				fprintf(fs, "\t\t};\n");		
				fprintf(fs, "\t\n");
				
				fprintf(fs, "\t\tstruct Internal;\n");	
				fprintf(fs, "\t\tfriend Internal;\n");	
				fprintf(fs, "\t\n");

				fprintf(fs, "\t\tstruct %s\n", "Properties");		
				fprintf(fs, "\t\t{\n");	
				fprintf(fs, "\t\t\tenum\n");	
				fprintf(fs, "\t\t\t{\n");	
				for( TiXmlElement* xnode = node->FirstChildElement("Member"); xnode != 0; xnode = xnode->NextSiblingElement("Member")) 
					fprintf(fs, "\t\t\t\t%s,\n", xnode->Attribute("Name"));	
				fprintf(fs, "\t\t\t\t%s,\n", "Max");	
				fprintf(fs, "\t\t\t};\n");
				fprintf(fs, "\t\t};\n");
				fprintf(fs, "\t};\n");		
				fprintf(fs, "\n");			
			}
		}


		fprintf(fs, "\n");
		fprintf(fs, "}\n");
	}
}


void main(int argc, char** argv)
{	
	
	const char* filename = /*"C:\\Users\\Phr34k\\Desktop\\dds\\flow\\flow.xml"*/ argv[1];
	const char* ofilename = /*"C:\\Users\\Phr34k\\Desktop\\Import\\dllinterface - Copy\\Aurora.AABB\\Flow\\Generated.h"*/ argv[2];

	
	HANDLE hReadHandle = INVALID_HANDLE_VALUE, hWriteHandle = INVALID_HANDLE_VALUE, hThread = INVALID_HANDLE_VALUE;
	CreatePipe(&hReadHandle,&hWriteHandle, NULL, 4096);
	if(hWriteHandle != INVALID_HANDLE_VALUE && hReadHandle != INVALID_HANDLE_VALUE )
	{		
		//Default use anom pipes to create write-if semantics. 
        int fWritedDesc = _open_osfhandle((intptr_t)hWriteHandle, _O_APPEND | _O_RDONLY | _O_TEXT);
		int fReadDesc = _open_osfhandle((intptr_t)hReadHandle, _O_RDONLY);		
        if(fWritedDesc != -1 && fReadDesc != -1)
        {
			FILE* fWrite = _fdopen(fWritedDesc, "a+");
			FILE* fRead  = _fdopen(fReadDesc, "r");
			FILE* fFile = fopen(ofilename, "rb+");
			if( fFile == 0 ) fFile = fopen(ofilename, "wb+");

			if(fWrite != 0 && fRead != 0 && fFile != 0)
			{
				//Start transacted write-server
				helper pipe;
				pipe.fRead = fRead;
				pipe.fFile = fFile;
				hThread = ::CreateThread(0,0, &helper::Message, &pipe, 0, 0);				

				output(fWrite, filename);

				//Close translacted write-server
				fclose(fWrite);
				::WaitForSingleObject(hThread, INFINITE);
				fclose(fRead);
				fclose(fFile);
			}			
			else
			{
				if(fWrite ) fclose(fWrite);
				if(fRead ) fclose(fRead);
				if(fFile ) fclose(fFile);				
			}
		}  
	}
	else
	{
		//Fallback if anom pipes fails..
		FILE* fFile = fopen(ofilename, "w");
		if( fFile != 0x0 )
		{
			output(fFile, filename);
			fclose(fFile);
		}
	};
	
	

	
	return;
}