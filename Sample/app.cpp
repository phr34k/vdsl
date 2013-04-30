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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include "core/surface.h"
#include "core/delegate.h"
#include "vpl/generated.h"
#include "vpl/generator.h"


static WNDCLASS wc;
static HWND wnd;
static char bitmapbuffer[sizeof( BITMAPINFO ) + 16];
static BITMAPINFO* bh;
HDC window_hdc;
Surface* surface = 0;
Pixel* buffer = 0;
extern int p;
extern bool IsGamePaused;
extern bool IsLeftPressed;
extern bool IsRightPressed;
extern bool IsDownPressed;
extern bool IsUpPressed;
extern bool IsSpacePressed;
void Init();
bool Tick();

extern bool __keystates[];
extern Delegate __onKeyDown[];
extern Delegate __onKeyRelease[];


extern void KeyPressed(int key);
extern void KeyReleased(int key);


static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int result = 0;
	switch (message)
	{
	case WM_CREATE:
		{
			int outHandle, errHandle, inHandle;
			FILE *outFile, *errFile, *inFile;
			AllocConsole();
			CONSOLE_SCREEN_BUFFER_INFO coninfo;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
			coninfo.dwSize.Y = 9999;
			SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
			outHandle = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
			errHandle = _open_osfhandle((long)GetStdHandle(STD_ERROR_HANDLE),_O_TEXT);
			inHandle = _open_osfhandle((long)GetStdHandle(STD_INPUT_HANDLE),_O_TEXT );
			outFile = _fdopen(outHandle, "w" );
			errFile = _fdopen(errHandle, "w");
			inFile =  _fdopen(inHandle, "r");
			*stdout = *outFile;
			*stderr = *errFile;
			*stdin = *inFile;
			setvbuf( stdout, NULL, _IONBF, 0 );
			setvbuf( stderr, NULL, _IONBF, 0 );
			setvbuf( stdin, NULL, _IONBF, 0 );
			std::ios::sync_with_stdio();
		}
	case WM_PAINT:
		if (!buffer) break;
		StretchDIBits( window_hdc, 0, 0, SCRWIDTH, SCRHEIGHT, 0, 0, SCRWIDTH, SCRHEIGHT, buffer, bh, DIB_RGB_COLORS, SRCCOPY );
		ValidateRect( wnd, NULL );
		break;
	case WM_KEYUP:
		if( wParam < 255 )
		{
			__keystates[wParam] = false;
			__onKeyDown[wParam].Execute(); 
		}
		

	/*
		if (wParam == 38) 
			IsUpPressed = false;
		else if (wParam == 37) 
			IsLeftPressed = false;
		else if (wParam == 40) 
			IsDownPressed = false;
		else if (wParam == 39) 
			IsRightPressed = false;
		else if (wParam == 32) 
		{
			IsSpacePressed = false;
			OnReleaseSpace();
		}*/



		KeyReleased(wParam);


		break;
	case WM_KEYDOWN:


		if( wParam < 255 )
		{
			__keystates[wParam] = true;
			__onKeyRelease[wParam].Execute(); 
		}



		KeyPressed(wParam);		

		/*
		if( wParam < 255 )
		

	
		if (wParam == 38) 
			IsUpPressed = true;
		else if (wParam == 37) 
			IsLeftPressed = true;
		else if (wParam == 40) 
			IsDownPressed = true;
		else if (wParam == 32) 
			IsSpacePressed = true;
		else if (wParam == 39) 
			IsRightPressed = true;*/
		/*
		else if( wParam == 19) 
			IsGamePaused ^= true;*/
		if ((wParam & 0xFF) != 27) 
			break;



	case WM_CLOSE:
		ReleaseDC( wnd, window_hdc );
		DestroyWindow( wnd );
		SystemParametersInfo( SPI_SETSCREENSAVEACTIVE, 1, 0, 0 );
		ExitProcess( 0 );
		break;
//some new code to view
	case	WM_CHAR: // read key code stuff here
		break;
	default:
		result = (int)DefWindowProc(hWnd,message,wParam,lParam);
	}
	return result;
}

void DrawWindow()
{
	MSG message;
	HACCEL haccel = 0;
	InvalidateRect( wnd,NULL,TRUE );
	SendMessage( wnd, WM_PAINT, 0, 0 );
	while (PeekMessage( &message, wnd, 0, 0, PM_REMOVE ))
	{
		if (TranslateAccelerator( wnd, haccel, &message ) == 0)
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	}
	Sleep( 0 );
}

void InitWindow()
{
	RECT rect;
	unsigned int cc;
	wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hInstance = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(0,IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Template";
	RegisterClass(&wc);
	rect.left = rect.top = 0;
	rect.right = SCRWIDTH, rect.bottom = SCRHEIGHT;
	AdjustWindowRect( &rect, WS_POPUP|WS_SYSMENU|WS_CAPTION, 0 );
	rect.right -= rect.left, rect.bottom -= rect.top;
	wnd = CreateWindowEx( 0, "Template", "Template", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom, 0, 0, 0, 0 );
	ShowWindow(wnd,SW_NORMAL);
	for ( cc = 0; cc < sizeof( BITMAPINFOHEADER ) + 16; cc++ ) bitmapbuffer[cc] = 0;
	bh = (BITMAPINFO *)&bitmapbuffer;
	bh->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	bh->bmiHeader.biPlanes = 1;
	bh->bmiHeader.biCompression = BI_BITFIELDS;
	bh->bmiHeader.biWidth = SCRWIDTH, bh->bmiHeader.biHeight = -SCRHEIGHT;
	bh->bmiHeader.biBitCount = 16;
	((unsigned long*)bh->bmiColors)[0] = 31 << 11;
	((unsigned long*)bh->bmiColors)[1] = 63 << 5;
	((unsigned long*)bh->bmiColors)[2] = 31;
	window_hdc = GetDC(wnd);
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 0, 0, 0);

}

#pragma warning( push )
#pragma warning( disable : 4127 )
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{


	//Reference variables to supress bloody warnings on  warning level 4
	nCmdShow;
	lpCmdLine;
	hPrevInstance;
	hInstance;

	InitWindow();
	// prepare output canvas
	surface = new Surface( SCRWIDTH, SCRHEIGHT );
	buffer = surface->GetBuffer();
	surface->Clear( 0 );
	surface->InitCharset();
	// prepare renderer
	bool initialized = false;
	// go
	int count = 4;
	while (1)
	{
		if (!initialized)
		{
			initialized = true;
			surface->Clear( 0 );
			Init();
		}
		else
		{
			Tick();
			if (!--count) { DrawWindow(); count = 2; }
		}
	}
	return 1;
}
#pragma warning( pop ) 

