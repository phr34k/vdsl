// -----------------------------------------------------------
// surface.h
// 2004 - Jacco Bikker - jacco@bik5.com - www.bik5.com -   <><
// -----------------------------------------------------------

#ifndef I_SURFACE_H
#define I_SURFACE_H

#define SCRWIDTH            800
#define SCRHEIGHT	        300

#include "string.h"

#define REDMASK (0xF800)
#define GREENMASK (0x07E0)
#define BLUEMASK (0x001F)
#define WHITE 0xffff
#define GREY  ((8<<11)+(16<<5)+8)
#define DGREY ((4<<11)+(8<<5)+4)
#define LGREY ((24<<11)+(48<<5)+24)
#define RED	  (22<<11)
#define BLUE  (20)
#define SHFTMASK	((15<<11)+(31<<5)+15)
#define DSHFTMASK	((7<<11)+(15<<5)+7)
#define TSHFTMASK	((3<<11)+(7<<5)+3)

typedef unsigned short Pixel;

inline Pixel AddBlend( Pixel a_Color1, Pixel a_Color2 )
{
	int r = (a_Color1 & REDMASK) + (a_Color2 & REDMASK);
	int g = (a_Color1 & GREENMASK) + (a_Color2 & GREENMASK);
	int b = (a_Color1 & BLUEMASK) + (a_Color2 & BLUEMASK);
	r = (r > REDMASK)?REDMASK:r;
	g = (g > GREENMASK)?GREENMASK:g;
	b = (b > BLUEMASK)?BLUEMASK:b;
	return (Pixel)(r + g + b);
}

// subtractive blending
inline Pixel SubBlend( Pixel a_Color1, Pixel a_Color2 )
{
	int red = (a_Color1 & REDMASK) - (a_Color2 & REDMASK);
	int green = (a_Color1 & GREENMASK) - (a_Color2 & GREENMASK);
	int blue = (a_Color1 & BLUEMASK) - (a_Color2 & BLUEMASK);
	if (red < 0) red = 0;
	if (green < 0) green = 0;
	if (blue < 0) blue = 0;
	return (Pixel)(red + green + blue);
}

// color scaling
inline Pixel ScaleColor( Pixel c, int mul )
{
	unsigned int p = (((c&0xF81F)<<11)|((c&0x7E0)>>5))*mul;
	return (Pixel)(((p>>16)&0xF81F)|(p&0x7E0));
}

class Surface
{
	enum
	{
		OWNER = 1
	};

public:
	// constructor / destructors
	Surface( int a_Width, int a_Height );
	Surface( int a_Width, int a_Height, Pixel* a_Buffer );
	Surface( char* a_File );
	~Surface();

	// member data access
    Pixel* GetBuffer() { return m_Buffer; }
	unsigned char* GetAlpha() { return m_Alpha; }
	void SetBuffer( Pixel* a_Buffer ) { m_Buffer = a_Buffer; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	void CreateAlphaMap() { m_Alpha = new unsigned char[m_Width * m_Height]; }

	// Special operations
	void InitCharset();
	void InitPolyOutline();
	void SetChar( int c, char* c1, char* c2, char* c3, char* c4, char* c5 );
	void Print(const char* a_String, int x1, int y1, Pixel color );
	void Clear( Pixel a_Color );
	void Bar( int x1, int y1, int x2, int y2, Pixel a_Color );
	void AddBar( int x1, int y1, int x2, int y2, Pixel a_Color );
	void AddBox( int x1, int y1, int x2, int y2, Pixel a_Color );
	void Box( int x1, int y1, int x2, int y2, Pixel a_Color );
	void Line( int x1, int y1, int x2, int y2, Pixel color );
	void HLine( int x1, int y1, int l, Pixel a_Color );
	void VLine( int x1, int y1, int l, Pixel a_Color );
	void AddHLine( int x1, int y1, int l, Pixel a_Color );
	void AddVLine( int x1, int y1, int l, Pixel a_Color );
	void SubVLine( int x1, int y1, int l, Pixel a_Color );
	void TgaLoad(const char* a_File);
	void ParseHeader( unsigned char* a_Header );
	void CopyTo( Surface* a_Dst, int a_X, int a_Y );
	void CopyAreaTo( int a_X1, int a_Y1, int a_X2, int a_Y2, Surface* a_Dst, int a_DX, int a_DY );
	void AddAreaTo( int a_X1, int a_Y1, int a_X2, int a_Y2, Surface* a_Dst, int a_DX, int a_DY );
	void PartialCopyAreaTo( int a_X1, int a_Y1, int a_X2, int a_Y2, Surface* a_Dst, int a_DX, int a_DY, Pixel color = 0 );
	void Blur();
	void Bloom();
	void HBlur();
	void Fade( int a_Level, int a_Start, int a_End );
	void Resize( int a_Width, int a_Height );
	void ResizeTo( Surface* a_Dest );
	void TgaSave( char* a_File );
	void Colorize( Pixel a_Color, int a_Multiply, bool a_Alpha );
	void Plot( int x, int y, Pixel a_Color );

private:
	// Attributes
	Pixel* m_Buffer;	
	unsigned char* m_Alpha;
	int m_Width, m_Height;	
	
	// Static attributes for the buildin font
	unsigned char* s_Font;
	int s_Transl[256];		

	// Polygon outline tables
	int* left, *right;

	// Static attributes for the tga loader
	int m_TgaIDLen;					
	int m_TgaCMapType;				
	int m_TgaImgType;				
	int m_TgaWidth;					
	int m_TgaHeight;
	int m_TgaDepth;					
};

class Surface8
{
public:
	Surface8(const char* a_File );
	Surface8( int w, int h );
	~Surface8();
	unsigned char* GetBuffer() { return m_Buffer; }
	Pixel* GetPalette() { return m_Pal; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	void CopyTo( Surface8* a_Dest, int x = 0, int y = 0 );
	void CopyTo( Surface* a_Dest, int x = 0, int y = 0 );
	void Clear( unsigned char a_Val ) { memset( m_Buffer, a_Val, m_Width * m_Height ); }
	void TgaLoad(const char* a_File, int w, int h, unsigned char* dest );
	void ParseHeader( unsigned char* a_Header );
	void CreateAlphaMap() { m_Alpha = new unsigned char[m_Width * m_Height]; }
	unsigned char* GetAlpha() { return m_Alpha; }
	void SetPalette( Pixel* a_Pal ) { m_Pal = a_Pal; }
	void FindAnchor( int& a_X, int& a_Y );
private:
	unsigned char* m_Buffer, *m_Alpha;
	Pixel* m_Pal;
	int m_Width, m_Height;	
	int m_TgaIDLen;					
	int m_TgaCMapType;				
	int m_TgaImgType;				
	int m_TgaWidth;					
	int m_TgaHeight;					
	float x, y, vx, vy;
	bool alive;
};

class Sprite
{
public:
	// Sprite flags
	enum
	{
		FLARE		= (1<< 0),
		OPFLARE		= (1<< 1),	
		FLASH		= (1<< 4),	
		DISABLED	= (1<< 6),	
		GMUL		= (1<< 7),
		BLACKFLARE	= (1<< 8),	
		BRIGHTEST   = (1<< 9),
		RFLARE		= (1<<12),
		GFLARE		= (1<<13),
		NOCLIP		= (1<<14)
	};
	
	// Structors
	Sprite( Surface* a_Surface, unsigned int a_NumFrames );
	Sprite( Surface8* a_Surface, unsigned int a_NumFrames );
	~Sprite();
	// Methods
	void DrawShadow( int a_X, int a_Y, Surface* a_D, Pixel scol );
	void Draw( int a_X, int a_Y, Surface* a_D ) { if (m_RLE) DrawRLE( a_X, a_Y, a_D ); else if (m_RLE16) DrawRLE16( a_X, a_Y, a_D ); else if (m_Surf8) Draw8( a_X, a_Y, a_D ); else Draw16( a_X, a_Y, a_D ); }
	void Draw8( int a_X, int a_Y, Surface* a_Target = 0 );
	void Draw16( int a_X, int a_Y, Surface* a_Target = 0 );
	void DrawRLE( int a_X, int a_Y, Surface* a_Target = 0 );
	void DrawRLE16( int a_X, int a_Y, Surface* a_Target = 0 );
	void BuildRLEData();
	void BuildRLEData16();
	void DrawScaled( int a_X, int a_Y, int a_Width, int a_Height, Surface* a_Target );
	void SetFlags( unsigned int a_Flags ) { m_Flags = a_Flags; }
	void SetFrame( unsigned int a_Index ) { m_CurrentFrame = a_Index; }
	unsigned int GetFlags() const { return m_Flags; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	Pixel* GetBuffer() { return m_Surface->GetBuffer(); }	
	unsigned char* GetBuffer8() { return m_Surf8->GetBuffer(); }	
	unsigned char* GetAlpha() { return (m_Surface)?m_Surface->GetAlpha():m_Surf8->GetAlpha(); }
	void PushClipWindow() { m_CX1b = m_CX1, m_CY1b = m_CY1, m_CX2b = m_CX2, m_CY2b = m_CY2; }
	void PopClipWindow() { m_CX1 = m_CX1b, m_CY1 = m_CY1b, m_CX2 = m_CX2b, m_CY2 = m_CY2b; }
	void SetClipWindow( int x1, int y1, int x2, int y2 ) { m_CX1 = x1, m_CY1 = y1, m_CX2 = x2, m_CY2 = y2; }
	void ResetClipWindow();
	unsigned int Frames() { return m_NumFrames; }
	Surface* GetSurface() { return m_Surface; }
private:
	// Methods
	void InitializeStartData();
	void InitializeStartData8();
	void InitializeStartDataAlpha();
	// Attributes
	int m_Width, m_Height, m_Pitch;
	unsigned int m_NumFrames;          
	unsigned int m_CurrentFrame;       
	unsigned int m_Flags;
	unsigned int** m_Start;
	Surface* m_Surface;
	Surface8* m_Surf8;
	unsigned char** m_RLE;
	unsigned short** m_RLE16;
	int m_CX1, m_CY1, m_CX2, m_CY2;
	int m_CX1b, m_CY1b, m_CX2b, m_CY2b;
};

class Font
{
public:
	Font();
	Font( char* a_File, char* a_Chars );
	~Font();
	void NegPrint( Surface* a_Target, char* a_Text, int a_X, int a_Y );
	void NegCentre( Surface* a_Target, char* a_Text, int a_Y );
	void Print( Surface* a_Target, char* a_Text, int a_X, int a_Y, bool clip = false );
	void FadePrint( Surface* a_Target, char* a_Text, int a_X, int a_Y, int a_Scale );
	void FadeCentre( Surface* a_Target, char* a_Text, int a_X, int a_Y, int a_Scale );
	void FadeCentre( Surface* a_Target, char* a_Text, int a_Y, int a_Scale );
	void Centre( Surface* a_Target, char* a_Text, int a_Y );
	int Width( char* a_Text );
	int Height() { return m_Surface->GetHeight(); }
	void YClip( int y1, int y2 ) { m_CY1 = y1; m_CY2 = y2; }
private:
	Surface* m_Surface;
	int* m_Offset, *m_Width, *m_Trans, m_Height, m_CY1, m_CY2;
};

#endif
