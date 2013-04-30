// -----------------------------------------------------------
// surface.cpp
// 2004 - Jacco Bikker - jacco@bik5.com - www.bik5.com -   <><
// -----------------------------------------------------------

#include "surface.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"
#include "exception"

// -----------------------------------------------------------
// Palettized surface class implementation
// -----------------------------------------------------------
Surface8::Surface8(const char* a_File ) :
	m_Buffer( NULL ),
	m_Width( 0 ), m_Height( 0 ),
	m_Alpha( 0 )
{
	unsigned char* tgabuff = new unsigned char[20];
	FILE* tga;
	fopen_s( &tga, a_File, "rb" );
	if (!tga) 
	{
		exit( 0 );
	}
	fread( tgabuff, 1, 20, tga );
	fclose( tga );
	ParseHeader( tgabuff );
	m_Width = m_TgaWidth;
	m_Height = m_TgaHeight;
	m_Buffer = new unsigned char[m_Width * m_Height];
	m_Pal = new Pixel[256];
	TgaLoad( a_File, m_Width, m_Height, m_Buffer );
	delete [] tgabuff;
}

Surface8::Surface8( int w, int h ) :
	m_Width( w ), m_Height( h ),
	m_Alpha( 0 ), m_Pal( 0 )
{
	m_Buffer = new unsigned char[w * h];
	m_Pal = new Pixel[256];
}

void Surface8::TgaLoad(const char* a_File, int w, int h, unsigned char* dest )
{
	int size = w * h + 20 + 768;
	unsigned char* tgabuff = new unsigned char[size];
	FILE* f;
	fopen_s( &f, a_File, "rb" );
	fread( tgabuff, 1, size, f );
	fclose( f );
	ParseHeader( tgabuff );
	if (m_TgaImgType == 1)
	{
		for ( int i = 0; i < 256; i++ )
		{
			int b = *(tgabuff + 18 + m_TgaIDLen + i * 3);
			int g = *(tgabuff + 18 + m_TgaIDLen + i * 3 + 1);
			int r = *(tgabuff + 18 + m_TgaIDLen + i * 3 + 2);
			m_Pal[i] = (Pixel)(((r >> 3) << 11) + ((g >> 2) << 5) + (b >> 3));
		}
		unsigned char* src = tgabuff + 18 + m_TgaIDLen + 768 + (h - 1) * w;
		for ( int y = 0; y < h; y++ )
		{
			memcpy( dest, src, w );
			dest += w;
			src -= w;
		}
	}
	delete tgabuff;
}

void Surface8::ParseHeader( unsigned char* a_Header )
{
	m_TgaIDLen		= *a_Header;
	m_TgaCMapType	= *(a_Header + 1);
	m_TgaImgType	= *(a_Header + 2);
	m_TgaWidth		= *(a_Header + 12) + 256 * *(a_Header + 13);
	m_TgaHeight		= *(a_Header + 14) + 256 * *(a_Header + 15);
}

void Surface8::CopyTo( Surface8* a_Dest, int a_X, int a_Y )
{
	unsigned char* dst = a_Dest->GetBuffer() + a_X + a_Y * a_Dest->GetWidth();
	unsigned char* src = m_Buffer;
	for ( int y = 0; y < m_Height; y++ )
	{
		memcpy( dst, src, m_Width );
		dst += a_Dest->GetWidth();
		src += m_Width;
	}
}

void Surface8::CopyTo( Surface* a_Dest, int a_X, int a_Y )
{
	Pixel* dst = a_Dest->GetBuffer() + a_X + a_Y * a_Dest->GetWidth();
	unsigned char* src = m_Buffer;
	for ( int y = 0; y < m_Height; y++ )
	{
		for ( int x = 0; x < m_Width; x++ ) dst[x] = m_Pal[src[x]];
		dst += a_Dest->GetWidth();
		src += m_Width;
	}
}

void Surface8::FindAnchor( int& a_X, int& a_Y )
{
	int s = m_Width * m_Height;
	int anchor = (31 << 11) + 31;
	for ( int i = 0; i < s; i++ ) if (m_Pal[m_Buffer[i]] == anchor)
	{
		m_Buffer[i] = 0;
		a_X = i % m_Width;
		a_Y = i / m_Width;
		break;
	}
}

Surface8::~Surface8()
{
	delete [] m_Buffer;
}

// -----------------------------------------------------------
// Surface class implementation
// -----------------------------------------------------------

Surface::Surface( int a_Width, int a_Height ) :
	m_Width( a_Width ),
	m_Height( a_Height ),
	left( 0 ), right( 0 ),
	m_Alpha( 0 )
{
	m_Buffer = new Pixel[a_Width * a_Height];
}

Surface::Surface( int a_Width, int a_Height, Pixel* a_Buffer ) :
	m_Width( a_Width ),
	m_Height( a_Height ),
	left( 0 ), right( 0 ),
	m_Alpha( 0 )
{
	m_Buffer = a_Buffer;
}

Surface::Surface( char* a_File ) :
	m_Buffer( NULL ),
	m_Width( 0 ), m_Height( 0 ),
	left( 0 ), right( 0 ),
	m_Alpha( 0 )
{
	m_Buffer = new Pixel[0];
	TgaLoad(a_File);
}

void Surface::ParseHeader( unsigned char* a_Header )
{
	m_TgaIDLen		= *a_Header;
	m_TgaCMapType	= *(a_Header + 1);
	m_TgaImgType	= *(a_Header + 2);
	m_TgaWidth		= *(a_Header + 12) + 256 * *(a_Header + 13);
	m_TgaHeight		= *(a_Header + 14) + 256 * *(a_Header + 15);
	m_TgaDepth		= *(a_Header + 16);
}

Surface::~Surface()
{
	delete [] m_Buffer;
	if (m_Alpha) delete m_Alpha;
}

void Surface::CopyTo( Surface* a_Dst, int a_X, int a_Y )
{
	Pixel* dst = a_Dst->GetBuffer();
	Pixel* src = m_Buffer;
	if ((src) && (dst)) 
	{
		int srcwidth = m_Width;
		int srcheight = m_Height;
		int srcpitch = srcwidth;
		int dstwidth = a_Dst->GetWidth();
		int dstheight = a_Dst->GetHeight();
		int dstpitch = dstwidth;
		if ((srcwidth + a_X) > dstwidth) srcwidth = dstwidth - a_X;
		if ((srcheight + a_Y) > dstheight) srcheight = dstheight - a_Y;
		if (a_X < 0) src -= a_X, srcwidth += a_X, a_X =0;
		if (a_Y < 0) src -= a_Y * srcpitch, srcheight += a_Y, a_Y = 0;
		if ((srcwidth > 0) && (srcheight > 0))
		{
			dst += a_X + dstpitch * a_Y;
			int y;
			for ( y = 0; y < srcheight; y++ )
			{
				
				memcpy( dst, src, srcwidth * 2 );
			#ifdef PARANOID
				if (y < (srcheight - 1)) 
				{
					dst += dstpitch;
					src += srcpitch;
				}
			#else
				dst += dstpitch;
				src += srcpitch;
			#endif
			}
		}
	}
}

void Surface::AddAreaTo( int a_X1, int a_Y1, int a_X2, int a_Y2, Surface* a_Dst, int a_DX, int a_DY )
{
	if (a_DX < 0)
	{
		a_X1 -= a_DX;
		a_DX = 0;
	}
	if (a_DY < 0)
	{
		a_Y1 -= a_DY;
		a_DY = 0;
	}
	if ((a_DX + (a_X2 - a_X1)) > a_Dst->GetWidth())
	{
		a_X2 = a_X1 + (a_Dst->GetWidth() - a_DX);
	}
	if ((a_DY + (a_Y2 - a_Y1)) > a_Dst->GetHeight())
	{
		a_Y2 = a_Y1 + (a_Dst->GetHeight() - a_DY);
	}
	if ((a_X1 < a_X2) && (a_Y1 < a_Y2))
	{
		int offset = a_X1 + a_Y1 * m_Width;
		int doffs = a_DX + a_DY * a_Dst->GetWidth();
		int w = (a_X2 - a_X1) + 1, h = (a_Y2 - a_Y1) + 1;
		if ((a_DX + w) > a_Dst->GetWidth()) w = a_Dst->GetWidth() - a_DX;
		if ((a_DY + h) > a_Dst->GetHeight()) h = a_Dst->GetHeight() - a_DY;
		Pixel* src = m_Buffer + offset;
		Pixel* dst = a_Dst->GetBuffer() + doffs;
		for ( int y = 0; y < h; y++ )
		{
			for ( int x = 0; x < w; x++ ) dst[x] = AddBlend( dst[x], src[x] );
			src += m_Width, dst += a_Dst->GetWidth();
		}
	}
}

void Surface::CopyAreaTo( int a_X1, int a_Y1, int a_X2, int a_Y2, Surface* a_Dst, int a_DX, int a_DY )
{
	if (a_DX < 0)
	{
		a_X1 -= a_DX;
		a_DX = 0;
	}
	if (a_DY < 0)
	{
		a_Y1 -= a_DY;
		a_DY = 0;
	}
	if ((a_DX + (a_X2 - a_X1)) > a_Dst->GetWidth())
	{
		a_X2 = a_X1 + (a_Dst->GetWidth() - a_DX);
	}
	if ((a_DY + (a_Y2 - a_Y1)) > a_Dst->GetHeight())
	{
		a_Y2 = a_Y1 + (a_Dst->GetHeight() - a_DY);
	}
	if ((a_X1 < a_X2) && (a_Y1 < a_Y2))
	{
		int offset = a_X1 + a_Y1 * m_Width;
		int doffs = a_DX + a_DY * a_Dst->GetWidth();
		int w = (a_X2 - a_X1) + 1, h = (a_Y2 - a_Y1) + 1;
		if ((a_DX + w) > a_Dst->GetWidth()) w = a_Dst->GetWidth() - a_DX;
		if ((a_DY + h) > a_Dst->GetHeight()) h = a_Dst->GetHeight() - a_DY;
		Pixel* src = m_Buffer + offset;
		Pixel* dst = a_Dst->GetBuffer() + doffs;
		for ( int y = 0; y < h; y++ )
		{
			for ( int x = 0; x < w; x++ ) dst[x] =  src[x];
			src += m_Width, dst += a_Dst->GetWidth();
		}
	}
}

void Surface::PartialCopyAreaTo( int a_X1, int a_Y1, int a_X2, int a_Y2, Surface* a_Dst, int a_DX, int a_DY, Pixel color )
{
	if (a_DX < 0)
	{
		a_X1 -= a_DX;
		a_DX = 0;
	}
	if (a_DY < 0)
	{
		a_Y1 -= a_DY;
		a_DY = 0;
	}
	if ((a_DX + (a_X2 - a_X1)) > a_Dst->GetWidth())
	{
		a_X2 = a_X1 + (a_Dst->GetWidth() - a_DX);
	}
	if ((a_DY + (a_Y2 - a_Y1)) > a_Dst->GetHeight())
	{
		a_Y2 = a_Y1 + (a_Dst->GetHeight() - a_DY);
	}
	if ((a_X1 < a_X2) && (a_Y1 < a_Y2))
	{
		int offset = a_X1 + a_Y1 * m_Width;
		int doffs = a_DX + a_DY * a_Dst->GetWidth();
		int w = (a_X2 - a_X1) + 1, h = (a_Y2 - a_Y1) + 1;
		if ((a_DX + w) > a_Dst->GetWidth()) w = a_Dst->GetWidth() - a_DX;
		if ((a_DY + h) > a_Dst->GetHeight()) h = a_Dst->GetHeight() - a_DY;
		Pixel* src = m_Buffer + offset;
		Pixel* dst = a_Dst->GetBuffer() + doffs;
		for ( int y = 0; y < h; y++ )
		{
			for ( int x = 0; x < w; x++ ) dst[x] = (( src[x] == color ) ?  dst[x] : src[x]);
			src += m_Width, dst += a_Dst->GetWidth();
		}
	}
}




void Surface::Clear( Pixel a_Color )
{
	memset(m_Buffer,a_Color,m_Width * m_Height * sizeof(Pixel));
}

void Surface::Bar( int x1, int y1, int x2, int y2, Pixel a_Color )
{
	if (y1 < 0) y1 = 0;
	if (y1 > (m_Height - 1)) y1 = m_Height - 1;
	if (y2 < 0) y2 = 0;
	if (y2 > (m_Height - 1)) y2 = m_Height - 1;
	if (x1 < 0) x1 = 0;
	if (x2 < 0) x2 = 0;
	if (x1 > (m_Width - 1)) x1 = m_Width - 1;
	if (x2 > (m_Width - 1)) x2 = m_Width - 1;
	Pixel* d = m_Buffer + y1 * m_Width;
	for ( int y = y1; y < y2; y++ ) 
	{
		for ( int x = x1; x < x2; x++ ) d[x] = a_Color;
		d += m_Width;
	}
}

void Surface::AddBar( int x1, int y1, int x2, int y2, Pixel a_Color )
{
	if (y1 < 0) y1 = 0;
	if (y1 > (m_Height - 1)) y1 = m_Height - 1;
	if (y2 < 0) y2 = 0;
	if (y2 > (m_Height - 1)) y2 = m_Height - 1;
	if (x1 < 0) x1 = 0;
	if (x2 < 0) x2 = 0;
	if (x1 > (m_Width - 1)) x1 = m_Width - 1;
	if (x2 > (m_Width - 1)) x2 = m_Width - 1;
	Pixel* d = m_Buffer + y1 * m_Width;
	for ( int y = y1; y < y2; y++ ) 
	{
		for ( int x = x1; x < x2; x++ ) d[x] = AddBlend( d[x], a_Color );
		d += m_Width;
	}
}

void Surface::Box( int x1, int y1, int x2, int y2, Pixel a_Color )
{
	if (y1 < 0) y1 = 0;
	if (y1 > (m_Height - 1)) y1 = m_Height - 1;
	if (y2 < 0) y2 = 0;
	if (y2 > (m_Height - 1)) y2 = m_Height - 1;
	if (x1 < 0) x1 = 0;
	if (x2 < 0) x2 = 0;
	if (x1 > (m_Width - 1)) x1 = m_Width - 1;
	if (x2 > (m_Width - 1)) x2 = m_Width - 1;
	Pixel* a = m_Buffer + y1 * m_Width;
	Pixel* b = m_Buffer + y2 * m_Width;
	for ( int x = x1; x <= x2; x++ ) a[x] = b[x] = a_Color;
	for ( int y = y1; y <= y2; y++, a += m_Width ) a[x1] = a[x2] = a_Color;
}

void Surface::AddBox( int x1, int y1, int x2, int y2, Pixel a_Color )
{
	if (y1 < 0) y1 = 0;
	if (y1 > (m_Height - 1)) y1 = m_Height - 1;
	if (y2 < 0) y2 = 0;
	if (y2 > (m_Height - 1)) y2 = m_Height - 1;
	if (x1 < 0) x1 = 0;
	if (x2 < 0) x2 = 0;
	if (x1 > (m_Width - 1)) x1 = m_Width - 1;
	if (x2 > (m_Width - 1)) x2 = m_Width - 1;
	Pixel* a = m_Buffer + y1 * m_Width;
	Pixel* b = m_Buffer + y2 * m_Width;
	for ( int x = x1; x <= x2; x++ ) 
	{
		a[x] = AddBlend( a[x], a_Color );
		b[x] = AddBlend( b[x], a_Color );
	}
	for ( int y = y1; y <= y2; y++ ) 
	{
		a[x1] = AddBlend( a[x1], a_Color );
		a[x2] = AddBlend( a[x2], a_Color );
		a += m_Width;
	}
}

void Surface::HLine( int x1, int y1, int l, Pixel a_Color )
{
	Pixel* a = m_Buffer + x1 + y1 * m_Width;
	for ( int i = 0; i < l; i++ ) *a++ = a_Color;
}

void Surface::VLine( int x1, int y1, int l, Pixel a_Color )
{
	Pixel* a = m_Buffer + x1 + y1 * m_Width;
	for ( int i = 0; i < l; i++ ) { *a = a_Color; a += m_Width; }
}

void Surface::AddHLine( int x1, int y1, int l, Pixel a_Color )
{
	Pixel* a = m_Buffer + x1 + y1 * m_Width;
	for ( int i = 0; i < l; i++, a++ ) *a = AddBlend( *a, a_Color );
}

void Surface::AddVLine( int x1, int y1, int l, Pixel a_Color )
{
	Pixel* a = m_Buffer + x1 + y1 * m_Width;
	for ( int i = 0; i < l; i++, a += m_Width ) { *a = AddBlend( *a, a_Color ); }
}

void Surface::SubVLine( int x1, int y1, int l, Pixel a_Color )
{
	Pixel* a = m_Buffer + x1 + y1 * m_Width;
	for ( int i = 0; i < l; i++, a += m_Width ) { *a = SubBlend( *a, a_Color ); }
}

void Surface::Print(const char* a_String, int x1, int y1, Pixel color )
{
	Pixel* t = m_Buffer + x1 + y1 * m_Width;
	int i, xpos = x1;
	for ( i = 0; i < (int)(strlen( a_String )); i++ )
	{	
		long pos = 0;
		if ((a_String[i] >= 'A') && (a_String[i] <= 'Z')) pos = s_Transl[(unsigned short)(a_String[i] - ('A' - 'a'))];
													 else pos = s_Transl[(unsigned short)a_String[i]];
		Pixel* a = t;
		unsigned char* c = s_Font + pos * 25;
		int h, v;
		for ( v = 0; v < 5; v++ ) 
		{
			for ( h = 0; h < 5; h++ ) if (*c++ == 'o') *(a + h) = color;
			a += m_Width;
		}
		t += 6;
		xpos += 6;
	}
}

void Surface::Line( int x1, int y1, int x2, int y2, Pixel c )
{
	if ((x1 < 0) || (x2 < 0) || (y1 < 0) || (y2 < 0) ||
		(x1 > (SCRWIDTH - 1)) || (x2 > (SCRWIDTH - 1)) ||
		(y1 > (SCRHEIGHT - 1)) || (y2 > (SCRHEIGHT - 1))) return;
	int b = x2 - x1;
	int h = y2 - y1;
	int l = abs( b );
	if (abs ( h ) > l) l = abs( h );
	if (l == 0) return;
	int dx = (b << 10) / l;
	int dy = (h << 10) / l;
	x1 <<= 10, y1 <<= 10;
	for ( int i = 0; i <= l; i++ )
	{
		Pixel* d = m_Buffer + (x1 >> 10) + (y1 >> 10) * m_Width;
		*d = AddBlend( *d, c );
		x1 += dx, y1 += dy;
	}
}

void Surface::SetChar( int c, char* c1, char* c2, char* c3, char* c4, char* c5 )
{
	memcpy( s_Font + c * 25, c1, 5 );
	memcpy( s_Font + c * 25 + 5, c2, 5 );
	memcpy( s_Font + c * 25 + 10, c3, 5 );
	memcpy( s_Font + c * 25 + 15, c4, 5 );
	memcpy( s_Font + c * 25 + 20, c5, 5 );
}

void Surface::Colorize( Pixel a_Color, int a_Multiply, bool a_Alpha )
{
	int rm = (a_Color & REDMASK) >> 11;
	int gm = (a_Color & GREENMASK) >> 5;
	int bm = a_Color & BLUEMASK;
	int i;
	for ( i = 0; i < m_Width * m_Height; i++ )
	{
		Pixel c = m_Buffer[i];
		if ((!a_Alpha) || (c != (REDMASK + BLUEMASK)))
		{
			int red = (c & REDMASK) >> 11;
			int green = (c & GREENMASK) >> 5;
			int blue = c & BLUEMASK;
			red = (int)((red * rm * a_Multiply) >> 15);
			green = (int)((green * gm * a_Multiply) >> 16);
			blue = (int)((blue * bm * a_Multiply) >> 15);
			red = (red > 31)?31:red;
			green = (green > 63)?63:green;
			blue = (blue > 31)?31:blue;
			m_Buffer[i] = (Pixel)((red << 11) + (green << 5) + blue);
		}
	}
}

void Surface::Plot( int x, int y, Pixel a_Color )
{
	m_Buffer[x + y * m_Width] = a_Color;
}

void Surface::InitPolyOutline()
{
	delete left;
	delete right;
	left = new int[m_Height];
	right = new int[m_Height];
	for ( int i = 0; i < m_Height; i++ )
	{
		left[i] = (m_Width - 1) << 10;
		right[i] = 0;
	}
}

void Surface::Blur()
{
	Surface* t = new Surface( m_Width, m_Height );
	t->Box( 0, 0, m_Width - 1, m_Height - 1, 0 );
	Pixel* s = GetBuffer() + m_Width + 1;
	Pixel* d = t->GetBuffer() + m_Width + 1;
	int x, y;
	for ( y = 1; y < (m_Height - 1); y++ )
	{
		for ( x = 1; x < (m_Width - 1); x++ )
		{
			int rb = (*(s - 1) & (REDMASK + BLUEMASK)) + (*(s + 1) & (REDMASK + BLUEMASK)) + (*(s + m_Width) & (REDMASK + BLUEMASK)) + (*(s - m_Width) & (REDMASK + BLUEMASK));
			int g = (*(s - 1) & GREENMASK) + (*(s + 1) & GREENMASK) + (*(s + m_Width) & GREENMASK) + (*(s - m_Width) & GREENMASK);
			s++;
			*d++ = (Pixel)(((rb >> 2) & (REDMASK + BLUEMASK)) + ((g >> 2) & GREENMASK));
		}
		d += 2;
		s += 2;
	}
	memcpy( GetBuffer(), t->GetBuffer(), m_Width * m_Height * 2 );
	delete t;
}

void Surface::Bloom()
{
	Surface* t = new Surface( m_Width, m_Height );
	t->Box( 0, 0, m_Width - 1, m_Height - 1, 0 );
	Pixel* s = GetBuffer() + 2 * m_Width + 2;
	Pixel* d = t->GetBuffer() + 2 * m_Width + 2;
	int x, y;
	for ( y = 2; y < (m_Height - 2); y++ )
	{
		for ( x = 2; x < (m_Width - 2); x++ )
		{
			int rb = (*(s - 2) & (REDMASK + BLUEMASK)) + (*(s + 2) & (REDMASK + BLUEMASK)) + (*(s + 2 * m_Width) & (REDMASK + BLUEMASK)) + (*(s - 2 * m_Width) & (REDMASK + BLUEMASK));
			int g = (*(s - 2) & GREENMASK) + (*(s + 2) & GREENMASK) + (*(s + 2 * m_Width) & GREENMASK) + (*(s - 2 * m_Width) & GREENMASK);
			s++;
			*d++ = (Pixel)(((rb >> 2) & (REDMASK + BLUEMASK)) + ((g >> 2) & GREENMASK));
		}
		d += 4;
		s += 4;
	}
	memcpy( GetBuffer(), t->GetBuffer(), m_Width * m_Height * 2 );
	delete t;
}

void Surface::HBlur()
{
	Pixel* t = GetBuffer();
	Pixel* temp = new Pixel[SCRWIDTH];
	for ( int y = 0; y < m_Height; y++ )
	{
		temp[0] = (Pixel)(((t[0] & (31 << 6)) + (t[1] & (31 << 6))) >> 1);
		temp[1] = (Pixel)(((t[1] & (31 << 6)) + (t[2] & (31 << 6))) >> 1);
		for ( int x = 2; x < (m_Width - 2); x++ )
		{
			temp[x] = (Pixel)(((t[x - 2] & (15 << 7)) + (t[x - 1] & (15 << 7)) + (t[x + 1] & (15 << 7)) + (t[x + 2] & (15 << 7))) >> 2);
		}
		temp[m_Width - 2] = (Pixel)(((t[m_Width - 3] & (31 << 6)) + (t[m_Width - 2] & (31 << 6))) >> 1);
		temp[m_Width - 1] = (Pixel)(((t[m_Width - 2] & (31 << 6)) + (t[m_Width - 1] & (31 << 6))) >> 1);
		memcpy( t, temp, SCRWIDTH * 2 );
		t += m_Width;
	}
	delete temp;
}

void Surface::Fade( int a_Level, int a_Start, int a_End )
{
	int i, size = m_Width * m_Height;
	if (a_End != 0) size = a_End - a_Start;
	Pixel* b = m_Buffer + a_Start;
	int lines = size / m_Width;
	int v;
	for ( v = 0; v < lines; v++ )
	{
		int level = (a_Level >> 1) + (v & 1);
		switch (level)
		{
		case 0:
			memset( b, 0, m_Width * 2 );
			break;
		case 1:
			for ( i = 0; i < m_Width; i++ ) b[i] = (Pixel)((b[i] >> 3) & TSHFTMASK);
			break;
		case 2:
			for ( i = 0; i < m_Width; i++ ) b[i] = (Pixel)((b[i] >> 2) & DSHFTMASK);
			break;
		case 3:
			for ( i = 0; i < m_Width; i++ ) b[i] = (Pixel)(((b[i] >> 2) & DSHFTMASK) + ((b[i] >> 3) & TSHFTMASK));
			break;
		case 4:
			for ( i = 0; i < m_Width; i++ ) b[i] = (Pixel)((b[i] >> 1) & SHFTMASK);
			break;
		case 5:
			for ( i = 0; i < m_Width; i++ ) b[i] = (Pixel)(((b[i] >> 1) & SHFTMASK) + ((b[i] >> 3) & TSHFTMASK));
			break;
		case 6:
			for ( i = 0; i < m_Width; i++ ) b[i] = (Pixel)(b[i] - ((b[i] >> 2) & DSHFTMASK));
			break;
		case 7:
			for ( i = 0; i < m_Width; i++ ) b[i] = (Pixel)(b[i] - (Pixel)((b[i] >> 3) & TSHFTMASK));
			break;
		default:
			break;
		}
		b += m_Width;
	}
}

void Surface::Resize( int a_Width, int a_Height )
{
	Pixel* p = new Pixel[a_Width * a_Height];
	int dx = (m_Width << 10) / a_Width, dy = (m_Height << 10) / a_Height;
	int u, v;
	for ( v = 0; v < a_Height; v++ )
	{
		for ( u = 0; u < a_Width; u++ )
		{
			int su = u * dx, sv = v * dy;
			Pixel* s = m_Buffer + (su >> 10) + (sv >> 10) * m_Width;
			int ufrac = su & 1023, vfrac = sv & 1023;
			int w4 = (ufrac * vfrac) >> 10;
			int w3 = ((1023 - ufrac) * vfrac) >> 10;
			int w2 = (ufrac * (1023 - vfrac)) >> 10;
			int w1 = ((1023 - ufrac) * (1023 - vfrac)) >> 10;
			int x2 = ((su + dx) > ((m_Width - 1) << 10))?0:1;
			int y2 = ((sv + dy) > ((m_Height - 1) << 10))?0:1;
			Pixel p1 = *s, p2 = *(s + x2), p3 = *(s + m_Width * y2), p4 = *(s + m_Width * y2 + x2);
			int r = (((p1 & 0xf800) * w1 + (p2 & 0xf800) * w2 + (p3 & 0xf800) * w3 + (p4 & 0xf800) * w4) >> 10) & 0xf800;
			int g = (((p1 & 0x7e0) * w1 + (p2 & 0x7e0) * w2 + (p3 & 0x7e0) * w3 + (p4 & 0x7e0) * w4) >> 10) & 0x7e0;
			int b = (((p1 & 0x1f) * w1 + (p2 & 0x1f) * w2 + (p3 & 0x1f) * w3 + (p4 & 0x1f) * w4) >> 10) & 0x1f;
			*(p + u + v * a_Width) = (Pixel)(r + g + b);
		}
	}
	delete m_Buffer;
	m_Buffer = p;
	m_Width = a_Width, m_Height = a_Height;
}

void Surface::ResizeTo( Surface* a_Dest )
{
	Pixel* p = a_Dest->GetBuffer();
	int w = a_Dest->GetWidth(), h = a_Dest->GetHeight();
	int dx = (m_Width << 10) / w, dy = (m_Height << 10) / h;
	int u, v;
	for ( v = 0; v < h; v++ )
	{
		for ( u = 0; u < w; u++ )
		{
			int su = u * dx, sv = v * dy;
			Pixel* s = m_Buffer + (su >> 10) + (sv >> 10) * m_Width;
			int ufrac = su & 1023, vfrac = sv & 1023;
			int w4 = (ufrac * vfrac) >> 10;
			int w3 = ((1023 - ufrac) * vfrac) >> 10;
			int w2 = (ufrac * (1023 - vfrac)) >> 10;
			int w1 = ((1023 - ufrac) * (1023 - vfrac)) >> 10;
			int x2 = ((su + dx) > ((m_Width - 1) << 10))?0:1;
			int y2 = ((sv + dy) > ((m_Height - 1) << 10))?0:1;
			Pixel p1 = *s, p2 = *(s + x2), p3 = *(s + m_Width * y2), p4 = *(s + m_Width * y2 + x2);
			int r = (((p1 & 0xf800) * w1 + (p2 & 0xf800) * w2 + (p3 & 0xf800) * w3 + (p4 & 0xf800) * w4) >> 10) & 0xf800;
			int g = (((p1 & 0x7e0) * w1 + (p2 & 0x7e0) * w2 + (p3 & 0x7e0) * w3 + (p4 & 0x7e0) * w4) >> 10) & 0x7e0;
			int b = (((p1 & 0x1f) * w1 + (p2 & 0x1f) * w2 + (p3 & 0x1f) * w3 + (p4 & 0x1f) * w4) >> 10) & 0x1f;
			*(p + u + v * w) = (Pixel)(r + g + b);
		}
	}
}

void Surface::TgaLoad(const char* a_File)
{

	FILE* tga;
	if( fopen_s(&tga,a_File,"rb") != 0 )
	{
		throw new std::exception("Cannot load graphic file");
		return;
	}

	const unsigned char CompressedHeader[12] = {0,0,10,0,0,0,0,0,0,0,0,0};
	unsigned char tgaheader[12];
	fread(&tgaheader, sizeof(tgaheader), 1, tga);

	unsigned char header[6];
	fread(header, sizeof(header), 1, tga);
	unsigned char bpp=header[4]/8;

	if (bpp==1)
	{
		// 8 bit images
		fclose(tga);
		Surface8* s = new Surface8(a_File);
		Resize(s->GetWidth(),s->GetHeight());
		s->CopyTo(this);
		delete s;
	} 
	else 
	{
		Resize(header[1] * 256 + header[0], header[3] * 256 + header[2]);

		// uncompressed 24 bit images
		if (!memcmp(CompressedHeader, &tgaheader, sizeof(tgaheader))==0)
		{
			int imgdatasize=m_Width*m_Height*bpp;
			unsigned char* buffer = (unsigned char*)malloc(imgdatasize);
			fread(buffer, 1, imgdatasize, tga);
			for (int xx=0;xx<m_Width;xx++)
			{
				for (int yy=0;yy<m_Height;yy++)	
				{
					int idx = (m_Height-yy-1)*m_Width+xx;
					m_Buffer[yy*m_Width+xx] = (((buffer[idx*bpp+2]/8) << 11) + ((buffer[idx*bpp+1]/4) << 5) + (buffer[idx*bpp]/8));
				}
			}
			free(buffer);
		}
		// compressed 24 bit images
		else
		{
			unsigned int pixelcount = m_Width*m_Height;
			unsigned int currentpixel = 0;
			unsigned char* colorbuffer = (unsigned char*)malloc(bpp);
			do
			{
				unsigned char chunkheader;
				fread(&chunkheader, sizeof(unsigned char), 1, tga);
				if(chunkheader < 128)
				{
					chunkheader++;
					for(int counter = 0; counter < chunkheader; counter++)
					{
						fread(colorbuffer, 1, bpp, tga);
						m_Buffer[currentpixel] = (((colorbuffer[2]/8) << 11) + ((colorbuffer[1]/4) << 5) + (colorbuffer[0]/8));
						currentpixel++;
					}
				}
				else
				{
					chunkheader -= 127;
					fread(colorbuffer, 1, bpp, tga);
					for(int counter = 0; counter < chunkheader; counter++)
					{
						m_Buffer[currentpixel] = (((colorbuffer[2]/8) << 11) + ((colorbuffer[1]/4) << 5) + (colorbuffer[0]/8));
						currentpixel++;
					}
				}
			}
			while(currentpixel < pixelcount);	
			free(colorbuffer);
		}
		fclose(tga);
	}
}


void Surface::TgaSave( char* a_File )
{
	FILE* tga;
	fopen_s( &tga, a_File, "wb" );
	unsigned char* header = new unsigned char[18];
	memset( header, 0, 18 );
	header[2] = 2;
	header[12] = (unsigned char)(m_Width & 65535);
	header[13] = (unsigned char)(m_Width >> 16);
	header[14] = (unsigned char)(m_Height & 65535);
	header[15] = (unsigned char)(m_Height >> 16);
	header[16] = 16;
	header[17] = (1 << 5) | 7;
	Pixel* b = m_Buffer;
	int size = m_Width * m_Height;
	Pixel* buffer = new Pixel[size];
	for ( int i = 0; i < size; i++ ) buffer[i] = ((b[i] & 0xf00) << 3) + ((b[i] & 0x0f0) << 2) + ((b[i] & 15) << 1);
	fwrite( header, 18, 1, tga );
	fwrite( buffer, size * 2, 1, tga );
	delete buffer;
	delete header;
	fclose( tga );
}

void Surface::InitCharset()
{
	s_Font = new unsigned char[57 * 5 * 5];
	SetChar( 0, ":ooo:", "o:::o", "ooooo", "o:::o", "o:::o" );
	SetChar( 1, "oooo:", "o:::o", "oooo:", "o:::o", "oooo:" );
	SetChar( 2, ":oooo", "o::::", "o::::", "o::::", ":oooo" );
	SetChar( 3, "oooo:", "o:::o", "o:::o", "o:::o", "oooo:" );
	SetChar( 4, "ooooo", "o::::", "oooo:", "o::::", "ooooo" );
	SetChar( 5, "ooooo", "o::::", "ooo::", "o::::", "o::::" );
	SetChar( 6, ":oooo", "o::::", "o:ooo", "o:::o", ":ooo:" );
	SetChar( 7, "o:::o", "o:::o", "ooooo", "o:::o", "o:::o" );
	SetChar( 8, "::o::", "::o::", "::o::", "::o::", "::o::" );
	SetChar( 9, ":::o:", ":::o:", ":::o:", ":::o:", "ooo::" );
	SetChar(10, "o::o:", "o:o::", "oo:::", "o:o::", "o::o:" );
	SetChar(11, "o::::", "o::::", "o::::", "o::::", "ooooo" );
	SetChar(12, "oo:o:", "o:o:o", "o:o:o", "o:::o", "o:::o" );
	SetChar(13, "o:::o", "oo::o", "o:o:o", "o::oo", "o:::o" );
	SetChar(14, ":ooo:", "o:::o", "o:::o", "o:::o", ":ooo:" );
	SetChar(15, "oooo:", "o:::o", "oooo:", "o::::", "o::::" );
	SetChar(16, ":ooo:", "o:::o", "o:::o", "o::oo", ":oooo" );
	SetChar(17, "oooo:", "o:::o", "oooo:", "o:o::", "o::o:" );
	SetChar(18, ":oooo", "o::::", ":ooo:", "::::o", "oooo:" );
	SetChar(19, "ooooo", "::o::", "::o::", "::o::", "::o::" );
	SetChar(20, "o:::o", "o:::o", "o:::o", "o:::o", ":oooo" );
	SetChar(21, "o:::o", "o:::o", ":o:o:", ":o:o:", "::o::" );
	SetChar(22, "o:::o", "o:::o", "o:o:o", "o:o:o", ":o:o:" );
	SetChar(23, "o:::o", ":o:o:", "::o::", ":o:o:", "o:::o" );
	SetChar(24, "o:::o", "o:::o", ":oooo", "::::o", ":ooo:" );
	SetChar(25, "ooooo", ":::o:", "::o::", ":o:::", "ooooo" );
	SetChar(26, ":ooo:", "o::oo", "o:o:o", "oo::o", ":ooo:" );
	SetChar(27, "::o::", ":oo::", "::o::", "::o::", ":ooo:" );
	SetChar(28, ":ooo:", "o:::o", "::oo:", ":o:::", "ooooo" );
	SetChar(29, "oooo:", "::::o", "::oo:", "::::o", "oooo:" );
	SetChar(30, "o::::", "o::o:", "ooooo", ":::o:", ":::o:" );
	SetChar(31, "ooooo", "o::::", "oooo:", "::::o", "oooo:" );
	SetChar(32, ":oooo", "o::::", "oooo:", "o:::o", ":ooo:" );
	SetChar(33, "ooooo", "::::o", ":::o:", "::o::", "::o::" );
	SetChar(34, ":ooo:", "o:::o", ":ooo:", "o:::o", ":ooo:" );
	SetChar(35, ":ooo:", "o:::o", ":oooo", "::::o", ":ooo:" );
	SetChar(36, "::o::", "::o::", "::o::", ":::::", "::o::" );
	SetChar(37, ":ooo:", "::::o", ":::o:", ":::::", "::o::" );
	SetChar(38, ":::::", ":::::", "::o::", ":::::", "::o::" );
	SetChar(39, ":::::", ":::::", ":ooo:", ":::::", ":ooo:" );
	SetChar(40, ":::::", ":::::", ":::::", ":::o:", "::o::" );
	SetChar(41, ":::::", ":::::", ":::::", ":::::", "::o::" );
	SetChar(42, ":::::", ":::::", ":ooo:", ":::::", ":::::" );
	SetChar(43, ":::o:", "::o::", "::o::", "::o::", ":::o:" );
	SetChar(44, "::o::", ":::o:", ":::o:", ":::o:", "::o::" );
	SetChar(45, ":::::", ":::::", ":::::", ":::::", ":::::" );
	SetChar(46, "ooooo", "ooooo", "ooooo", "ooooo", "ooooo" );
	SetChar(47, "::::o", ":::o:", "::o::", ":::o:", "::::o" );
	SetChar(48, "o::::", ":o:::", "::o::", ":o:::", "o::::" );
	SetChar(49, "::oo:", "::o::", ":oo::", "::o::", "::oo:" );
	SetChar(50, "::::o", ":::o:", "::o::", ":o:::", "o::::" );
	SetChar(51, ":::::", "::o::", ":::::", "::o::", ":o:::" );
	SetChar(52, ":::::", ":::::", ":::::", ":::::", "ooooo" );
	SetChar(53, "::o::", "::o::", "ooooo", "::o::", "::o::" );
	SetChar(54, "o:o:o", ":ooo:", "ooooo", ":ooo:", "o:o:o" );
	SetChar(55, ":ooo:", ":o:::", ":o:::", ":o:::", ":ooo:" );
	SetChar(56, ":ooo:", ":::o:", ":::o:", ":::o:", ":ooo:" );
	char c[] = "abcdefghijklmnopqrstuvwxyz0123456789!?:=,.-() #><{/;_+*[]";
	int i;
	for ( i = 0; i < 256; i++ ) s_Transl[i] = 45;
	for ( i = 0; i < 57; i++ ) s_Transl[(unsigned char)c[i]] = i;
}

Sprite::Sprite( Surface* a_Surface, unsigned int a_NumFrames ) :
	m_Width(  a_Surface->GetWidth() / a_NumFrames ),
	m_Height( a_Surface->GetHeight() ),
	m_Pitch(  a_Surface->GetWidth() ),
	m_NumFrames( a_NumFrames ),
	m_CurrentFrame( 0 ),
	m_Flags( 0 ),
	m_Start( new unsigned int*[a_NumFrames] ),
	m_Surface( a_Surface ), m_Surf8( 0 ), m_RLE( 0 ), m_RLE16( 0 ),
	m_CX1( 0 ), m_CY1( 0 ), m_CX2( SCRWIDTH - 1 ), m_CY2( SCRHEIGHT - 1 )
{
	if (GetAlpha()) InitializeStartDataAlpha(); else InitializeStartData();
}

Sprite::Sprite( Surface8* a_Surface, unsigned int a_NumFrames ) :
	m_Width(  a_Surface->GetWidth() / a_NumFrames ),
	m_Height( a_Surface->GetHeight() ),
	m_Pitch(  a_Surface->GetWidth() ),
	m_NumFrames( a_NumFrames ),
	m_CurrentFrame( 0 ),
	m_Flags( 0 ),
	m_Start( new unsigned int*[a_NumFrames] ),
	m_Surface( 0 ), m_Surf8( a_Surface ), m_RLE( 0 ), m_RLE16( 0 ),
	m_CX1( 0 ), m_CY1( 0 ), m_CX2( SCRWIDTH - 1 ), m_CY2( SCRHEIGHT - 1 )
{
	if (GetAlpha()) InitializeStartDataAlpha(); else InitializeStartData8();
}

Sprite::~Sprite()
{
	delete m_Surface;
	for ( unsigned int i = 0; i < m_NumFrames; i++ ) delete m_Start[i];
	delete m_Start;
}

void Sprite::ResetClipWindow() 
{ 
	m_CX1 = m_CY1 = 0; 
	m_CX2 = SCRWIDTH - 1; m_CY2 = SCRHEIGHT - 1; 
}

void Sprite::Draw16( int a_X, int a_Y, Surface* a_Target )
{
	if (m_Flags & DISABLED) return;
	if (!(m_Flags & NOCLIP))
	{
		if ((a_X < (m_CX1 - m_Width)) || (a_X > (m_CX2 + m_Width)) || (m_CX1 == m_CX2)) return;
		if ((a_Y < (m_CY1 - m_Height)) || (a_Y > (m_CY2 + m_Height))) return;
	}
	int x1 = a_X, x2 = a_X + m_Width;
	int y1 = a_Y, y2 = a_Y + m_Height;
	Pixel* src = GetBuffer() + m_CurrentFrame * m_Width;
	unsigned char* asrc = 0, *alpha = m_Surface->GetAlpha();
	if (alpha) asrc = alpha + m_CurrentFrame * m_Width;
	if (!(m_Flags & NOCLIP))
	{
		if (x1 < m_CX1)
		{
			src += m_CX1 - x1;
			asrc += m_CX1 - x1;
			x1 = m_CX1;
		}
		if (x2 > (m_CX2 + 1)) x2 = m_CX2 + 1;
		if (y1 < m_CY1) 
		{ 
			src += ((m_CY1 - y1) * m_Pitch);
			asrc += ((m_CY1 - y1) * m_Pitch);
			y1 = m_CY1;
		}
		if (y2 > (m_CY2 + 1)) y2 = m_CY2 + 1;
	}
	Pixel* dest = a_Target->GetBuffer();
	int xs, dpitch = a_Target->GetWidth();
	if ((x2 > x1) && (y2 > y1))
	{
		int addr = y1 * dpitch + x1;
		int width = x2 - x1;
		int height = y2 - y1;
		for ( int y = 0; y < height; y++ )
		{
			int line = y + (y1 - a_Y);
			int lsx = m_Start[m_CurrentFrame][line] + a_X;
			if (alpha)
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					Pixel c1 = *(src + x);
					unsigned char a = *(asrc + x);
					if (a) 
					{
						if (a == 31) *(dest + addr + x) = c1; else
						{
							Pixel c2 = ScaleColor( *(dest + addr + x), 31 - a );
							*(dest + addr + x) = ScaleColor( c1, a ) + c2;
						}
					}
				}
			}
			else if (m_Flags & FLASH)
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					Pixel c1 = *(src + x);
					if (c1) *(dest + addr + x) = 0xffff;
				}
			}
			else if (m_Flags & FLARE)
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					Pixel c1 = *(src + x);
					if (c1) 
					{
						Pixel c2 = *(dest + addr + x);
						*(dest + addr + x) = AddBlend( c1, c2 );
					}
				}
			}
			else if (m_Flags & BLACKFLARE)
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					Pixel c1 = *(src + x);
					if (c1) 
					{
						Pixel c2 = *(dest + addr + x);
						*(dest + addr + x) = SubBlend( c2, c1 );
					}
				}
			}
			else 
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					Pixel c1 = *(src + x);
					if (c1) *(dest + addr + x) = c1;
				}
			}
		#ifdef _DEBUG
			if (y < (height - 1)) addr += dpitch, src += m_Pitch, asrc += m_Pitch;
		#else
			addr += dpitch;
			src += m_Pitch;
			asrc += m_Pitch;
		#endif
		}
	}
}

void Sprite::Draw8( int a_X, int a_Y, Surface* a_Target )
{
	if (m_Flags & DISABLED) return;
	if (!(m_Flags & NOCLIP))
	{
		if ((a_X < (m_CX1 - m_Width)) || (a_X > (m_CX2 + m_Width)) || (m_CX1 == m_CX2)) return;
		if ((a_Y < (m_CY1 - m_Height)) || (a_Y > (m_CY2 + m_Height))) return;
	}
	int x1 = a_X, x2 = a_X + m_Width;
	int y1 = a_Y, y2 = a_Y + m_Height;
	unsigned char* src = m_Surf8->GetBuffer() + m_CurrentFrame * m_Width;
	unsigned char* alpha = m_Surf8->GetAlpha();
	if (alpha) alpha += m_CurrentFrame * m_Width;
	Pixel* pal = m_Surf8->GetPalette();
	if (!(m_Flags & NOCLIP))
	{
		if (x1 < m_CX1)
		{
			src += m_CX1 - x1;
			if (alpha) alpha += m_CX1 - x1;
			x1 = m_CX1;
		}
		if (x2 > (m_CX2 + 1)) x2 = m_CX2 + 1;
		if (y1 < m_CY1) 
		{ 
			src += ((m_CY1 - y1) * m_Pitch);
			if (alpha) alpha += ((m_CY1 - y1) * m_Pitch);
			y1 = m_CY1;
		}
		if (y2 > (m_CY2 + 1)) y2 = m_CY2 + 1;
	}
	Pixel* dest = a_Target->GetBuffer();
	int xs, dpitch = a_Target->GetWidth();
	if ((x2 > x1) && (y2 > y1))
	{
		int addr = y1 * dpitch + x1;
		int width = x2 - x1;
		int height = y2 - y1;
		for ( int y = 0; y < height; y++ )
		{
			int line = y + (y1 - a_Y);
			int lsx = m_Start[m_CurrentFrame][line] + a_X;
			if (alpha)
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					Pixel c1 = pal[*(src + x)];
					unsigned char a = *(alpha + x);
					if (a) 
					{
						if (a == 31) *(dest + addr + x) = c1; else
						{
							Pixel c2 = ScaleColor( *(dest + addr + x), 31 - a );
							*(dest + addr + x) = c1 + c2;
						}
					}
				}
			}
			else if (m_Flags & FLASH)
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					Pixel c1 = pal[*(src + x)];
					if (c1) *(dest + addr + x) = 0xffff;
				}
			}
			else if (m_Flags & FLARE)
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					Pixel c1 = pal[*(src + x)];
					if (c1) 
					{
						Pixel c2 = *(dest + addr + x);
						*(dest + addr + x) = AddBlend( c1, c2 );
					}
				}
			}
			else if (m_Flags & GMUL)
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					Pixel c1 = (pal[*(src + x)] >> 6) & 31;
					Pixel c2 = *(dest + addr + x);
					*(dest + addr + x) = ScaleColor( c2, c1 );
				}
			}
			else if (m_Flags & BLACKFLARE)
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					Pixel c1 = pal[*(src + x)];
					if (c1) 
					{
						Pixel c2 = *(dest + addr + x);
						*(dest + addr + x) = SubBlend( c2, c1 );
					}
				}
			}
			else if (m_Flags & BRIGHTEST)
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					Pixel c1 = pal[*(src + x)];
					Pixel c2 = *(dest + addr + x) & 31;
					if ((c1 & 31) > c2) *(dest + addr + x) = c1;
				}
			}
			else 
			{
				xs = (lsx > x1)?lsx - x1:0;
				for ( int x = xs; x < width; x++ )
				{
					Pixel c1 = pal[*(src + x)];
					if (c1) *(dest + addr + x) = c1;
				}
			}
		#ifdef _DEBUG
			if (y < (height - 1)) 
			{
				addr += dpitch, src += m_Pitch;
				if (alpha) alpha += m_Pitch;
			}
		#else
			addr += dpitch;
			src += m_Pitch;
			if (alpha) alpha += m_Pitch;
		#endif
		}
	}
}

void Sprite::DrawScaled( int a_X, int a_Y, int a_Width, int a_Height, Surface* a_Target )
{
	if ((a_Width == 0) || (a_Height == 0)) return;
	int v = 0;
	int du = (m_Pitch << 10) / a_Width;
	int dv = (m_Height << 10) / a_Height;
	Pixel* dest = a_Target->GetBuffer() + a_X + a_Y * a_Target->GetWidth();
	Pixel* src = GetBuffer() + m_CurrentFrame * m_Width;
	int x, y;
	for ( y = 0; y < a_Height; y++ )
	{
		int u = 0;
		int cv = (v >> 10) * m_Pitch;
		for ( x = 0; x < a_Width; x++ )
		{
			*(dest + x) = *(src + (u >> 10) + cv);
			u += du;
		}
		v += dv;
		dest += a_Target->GetWidth();
	}
}

void Sprite::InitializeStartData()
{
    for ( unsigned int f = 0; f < m_NumFrames; ++f )
    {
        m_Start[f] = new unsigned int[m_Height];
     	for ( int y = 0; y < m_Height; ++y )
     	{
      	    m_Start[f][y] = m_Width;
			Pixel* addr = GetBuffer() + f * m_Width + y * m_Pitch;
     	    for ( int x = 0; x < m_Width; ++x )
     	    {
                if (addr[x])
     	        {
     	            m_Start[f][y] = x;
                    break;
                }
            }
		}
	}
}

void Sprite::InitializeStartData8()
{
    for ( unsigned int f = 0; f < m_NumFrames; ++f )
    {
        m_Start[f] = new unsigned int[m_Height];
     	for ( int y = 0; y < m_Height; ++y )
     	{
      	    m_Start[f][y] = m_Width;
			unsigned char* addr = GetBuffer8() + f * m_Width + y * m_Pitch;
     	    for ( int x = 0; x < m_Width; ++x )
     	    {
                if (addr[x])
     	        {
     	            m_Start[f][y] = x;
                    break;
                }
            }
		}
	}
}

void Sprite::InitializeStartDataAlpha()
{
    for ( unsigned int f = 0; f < m_NumFrames; ++f )
    {
        m_Start[f] = new unsigned int[m_Height];
     	for ( int y = 0; y < m_Height; ++y )
     	{
      	    m_Start[f][y] = m_Width;
			unsigned char* addr = GetAlpha() + f * m_Width + y * m_Pitch;
     	    for ( int x = 0; x < m_Width; ++x )
     	    {
                if (addr[x])
     	        {
     	            m_Start[f][y] = x;
                    break;
                }
            }
		}
	}
}

void Sprite::DrawRLE( int a_X, int a_Y, Surface* a_Target )
{
	if (((a_X + m_Surf8->GetWidth()) < m_CX1) || (a_X >= m_CX2)) return;
	if (((a_Y + m_Surf8->GetHeight()) < m_CY1) || (a_Y >= m_CY2)) return;
	Pixel* pal = m_Surf8->GetPalette();
	int rside = m_Surf8->GetWidth() + a_X;
	if (rside > (m_CX2 + 1)) rside = m_CX2 + 1;
	int y1 = 0, y2 = m_Surf8->GetHeight();
	if (a_Y < m_CY1) y1 += m_CY1 - a_Y;
	if ((a_Y + m_Surf8->GetHeight()) > a_Target->GetHeight()) 
		y2 -= (a_Y + m_Surf8->GetHeight() - a_Target->GetHeight());
	Pixel* dst = a_Target->GetBuffer() + (a_Y + y1) * a_Target->GetWidth();
	for ( int y = y1; y < y2; y++ )
	{
		int x = a_X;
		unsigned char* src = m_RLE[y];
		if (src) 
		{
			int sdata = *src++;
			int len = sdata & 63, tp = sdata >> 6;
			if (a_X < m_CX1)
			{
				while ((x + len) < m_CX1)
				{
					if (tp == 3) x += len;
					else if (tp == 0) x += len, src += len;
					else x += len, src += 2 * len;
					sdata = *src++;
					len = sdata & 63, tp = sdata >> 6;
				}
				len -= m_CX1 - x;
				if (tp == 0) src += (m_CX1 - x); else if (tp == 2) src += 2 * (m_CX1 - x);
				x = m_CX1;
			}
			while ((x + len) < rside)
			{
				if (tp == 3) x += len;
				else if (tp == 0) for ( ; len > 0; len-- ) dst[x++] = pal[*src++];
				else for ( ; len > 0; len--, x++ ) dst[x] = ScaleColor( dst[x], 31 - *src++ ) + pal[*src++];
				sdata = *src++;
				len = sdata & 63, tp = sdata >> 6;
			}
			if (len > 0)
			{
				len = rside - x;
				if (tp == 0) for ( ; len > 0; len-- ) dst[x++] = pal[*src++];
				else if (tp == 2) for ( ; len > 0; len--, x++ ) dst[x] = ScaleColor( dst[x], 31 - *src++ ) + pal[*src++];
			}
		}
		dst += a_Target->GetWidth();
	}
}

void Sprite::DrawShadow( int a_X, int a_Y, Surface* a_Target, Pixel scol )
{
	int rside = m_Surface->GetWidth() + a_X;
	if (rside > (a_Target->GetWidth() * 2)) rside = a_Target->GetWidth() * 2;
	int y1 = 0, y2 = m_Surface->GetHeight();
	if (a_Y < 0) y1 -= a_Y;
	if ((a_Y + m_Surface->GetHeight()) > (a_Target->GetHeight() * 2)) 
		y2 -= (a_Y + m_Surface->GetHeight() - (a_Target->GetHeight() * 2));
	Pixel* dst = a_Target->GetBuffer() + ((a_Y + y1) >> 1) * a_Target->GetWidth();
	for ( int y = y1; y < y2; y++ ) if (y & 1)
	{
		int x = a_X;
		unsigned short* src = m_RLE16[y];
		if (src) 
		{
			int sdata = *src++;
			int len = sdata & 16383, tp = sdata >> 14, offs = (len + 1) >> 1;
			if (a_X < 0)
			{
				while ((x + len) < 0)
				{
					if (tp == 3) x += len;
					else if (tp == 0) x += len, src += len;
					else x += len, src += len + offs;
					sdata = *src++;
					len = sdata & 16383, tp = sdata >> 14, offs = (len + 1) >> 1;
				}
				len += x;
				if (tp == 3) x = len; else if (tp == 0) 
				{
					for ( src -= x, x = 0; len > 0; len--, src++, x++ ) dst[x >> 1] = scol;
				}
				else 
				{
					src -= x;
					unsigned char* asrc = (unsigned char*)(src + len) - x;
					for ( x = 0; len > 0; len--, x++, asrc++, src++ ) dst[x >> 1] = scol;
					src += offs;
				}

				if (x >= rside)
				{
					dst += a_Target->GetWidth();
					continue;			
				}
				sdata = *src++;
				len = sdata & 16383, tp = sdata >> 14, offs = (len + 1) >> 1;
			}
			while ((x + len) < rside)
			{
				if (tp == 3) x += len;
				else if (tp == 0) for ( ; len > 0; len--, src++, x++ ) dst[x >> 1] = scol;
				else 
				{
					unsigned char* asrc = (unsigned char*)(src + len);
					for ( ; len > 0; len--, x++, asrc++, src++ ) dst[x >> 1] = scol;
					src += offs;
				}
				sdata = *src++;
				len = sdata & 16383, tp = sdata >> 14, offs = (len + 1) >> 1;
			}
			if (len > 0)
			{
				int add = len, len = rside - x;
				if (tp == 0) for ( ; len > 0; len--, src++, x++ ) dst[x >> 1] = scol;
				else if (tp == 2) 
				{
					unsigned char* asrc = (unsigned char*)(src + add);
					for ( ; len > 0; len--, x++, asrc++, src++ ) dst[x >> 1] = scol;
				}
			}
		}
		dst += a_Target->GetWidth();
	}
}

void Sprite::DrawRLE16( int a_X, int a_Y, Surface* a_Target )
{
	if (((a_X + m_Surface->GetWidth()) < 0) || (a_X >= a_Target->GetWidth())) return;
	if (((a_Y + m_Surface->GetHeight()) < 0) || (a_Y >= a_Target->GetHeight())) return;
	int rside = m_Surface->GetWidth() + a_X;
	if (rside > a_Target->GetWidth()) rside = a_Target->GetWidth();
	int y1 = 0, y2 = m_Surface->GetHeight();
	if (a_Y < 0) y1 -= a_Y;
	if ((a_Y + m_Surface->GetHeight()) > a_Target->GetHeight()) 
		y2 -= (a_Y + m_Surface->GetHeight() - a_Target->GetHeight());
	Pixel* dst = a_Target->GetBuffer() + (a_Y + y1) * a_Target->GetWidth();
	for ( int y = y1; y < y2; y++ )
	{
		int x = a_X;
		unsigned short* src = m_RLE16[y];
		if (src) 
		{
			int sdata = *src++;
			int len = sdata & 16383, tp = sdata >> 14, offs = (len + 1) >> 1;
			if (a_X < 0)
			{
				while ((x + len) < 0)
				{
					if (tp == 3) x += len;
					else if (tp == 0) x += len, src += len;
					else x += len, src += len + offs;
					sdata = *src++;
					len = sdata & 16383, tp = sdata >> 14, offs = (len + 1) >> 1;
				}
				len += x;
				if (tp == 3) x = len; else if (tp == 0) 
				{
					for ( src -= x, x = 0; len > 0; len-- ) dst[x++] = *src++;
				}
				else 
				{
					src -= x;
					unsigned char* asrc = (unsigned char*)(src + len) - x;
					for ( x = 0; len > 0; len--, x++ ) dst[x] = ScaleColor( dst[x], 31 - *asrc++ ) + *src++;
					src += offs;
				}

				if (x >= rside)
				{
					dst += a_Target->GetWidth();
					continue;			
				}
				sdata = *src++;
				len = sdata & 16383, tp = sdata >> 14, offs = (len + 1) >> 1;
			}
			while ((x + len) < rside)
			{
				if (tp == 3) x += len;
				else if (tp == 0) for ( ; len > 0; len-- ) dst[x++] = *src++;
				else 
				{
					unsigned char* asrc = (unsigned char*)(src + len);
					for ( ; len > 0; len--, x++ ) dst[x] = ScaleColor( dst[x], 31 - *asrc++ ) + *src++;
					src += offs;
				}
				sdata = *src++;
				len = sdata & 16383, tp = sdata >> 14, offs = (len + 1) >> 1;
			}
			if (len > 0)
			{
				int add = len, len = rside - x;
				if (tp == 0) for ( ; len > 0; len-- ) dst[x++] = *src++;
				else if (tp == 2) 
				{
					unsigned char* asrc = (unsigned char*)(src + add);
					for ( ; len > 0; len--, x++ ) dst[x] = ScaleColor( dst[x], 31 - *asrc++ ) + *src++;
				}
			}
		}
		dst += a_Target->GetWidth();
	}
}

void Sprite::BuildRLEData16()
{
	Surface* s = m_Surface;
	Pixel* pixels = s->GetBuffer();
	unsigned char* alpha = s->GetAlpha();
	unsigned short* temp = new unsigned short[8192];
	unsigned char* atmp = new unsigned char[512];
	int width = s->GetWidth();
	m_RLE16 = new unsigned short*[s->GetHeight()];
	for ( int y = 0; y < s->GetHeight(); y++ )
	{
		int spans = 0;
		int dstpos = 0;
		int srcpos = 0;
		// find first filled pixel
		while (srcpos < width)
		{
			if (alpha[srcpos] > 0)
			{
				int lenpos = dstpos++;
				int len = 0;
				if (alpha[srcpos] == 31)
				{
					// opaque span
					while ((srcpos < width) && (alpha[srcpos] == 31))
					{
						temp[dstpos++] = pixels[srcpos++];
						len++;
					}
					temp[lenpos] = (unsigned short)(len);
					spans++;
				}
				else
				{
					// alpha span
					int apos = 0;
					while ((srcpos < width) && (alpha[srcpos] > 0) && (alpha[srcpos] < 31))
					{
						temp[dstpos++] = ScaleColor( pixels[srcpos], alpha[srcpos] );
						atmp[apos++] = alpha[srcpos++];
						len++;
					}
					temp[lenpos] = (unsigned short)(len | 32768);
					unsigned char* adst = (unsigned char*)(temp + dstpos);
					for ( int i = 0; i < len; i++ ) adst[i] = atmp[i];
					dstpos += (len + 1) >> 1;
					spans++;
				}
			}
			else
			{
				int len = 0;
				while ((srcpos < width) && (alpha[srcpos] == 0))
				{
					srcpos++;
					len++;
				}
				temp[dstpos++] = (unsigned short)(len | 32768 | 16384);
				spans++;
			}
		}
		if (spans > 0)
		{
			temp[dstpos++] = 0;
			m_RLE16[y] = new unsigned short[dstpos];
			memcpy( m_RLE16[y], temp, dstpos * 2 );
		}
		else
		{
			m_RLE16[y] = 0;
		}
		pixels += s->GetWidth();
		alpha += s->GetWidth();
	}
	delete temp;
	delete atmp;
	delete m_Surface->GetBuffer();
}

void Sprite::BuildRLEData()
{
	Surface8* s = m_Surf8;
	unsigned char* pixels = s->GetBuffer();
	unsigned char* alpha = s->GetAlpha();
	unsigned char* temp = new unsigned char[2048];
	int width = s->GetWidth();
	m_RLE = new unsigned char*[s->GetHeight()];
	for ( int y = 0; y < s->GetHeight(); y++ )
	{
		int spans = 0;
		int dstpos = 0;
		int srcpos = 0;
		// find first filled pixel
		while (srcpos < width)
		{
			if (alpha[srcpos] > 0)
			{
				int lenpos = dstpos++;
				int len = 0;
				if (alpha[srcpos] > 30)
				{
					// opaque span
					while ((srcpos < width) && (alpha[srcpos] > 30) && (len < 63))
					{
						temp[dstpos++] = pixels[srcpos++];
						len++;
					}
					temp[lenpos] = (unsigned char)(len);
					spans++;
				}
				else
				{
					// alpha span
					while ((srcpos < width) && (alpha[srcpos] > 0) && (alpha[srcpos] < 31) && (len < 63))
					{
						temp[dstpos++] = alpha[srcpos];
						temp[dstpos++] = pixels[srcpos++];
						len++;
					}
					temp[lenpos] = (unsigned char)(len | 128);
					spans++;
				}
			}
			else
			{
				int len = 0;
				while ((srcpos < width) && (alpha[srcpos] == 0) && (len < 63))
				{
					srcpos++;
					len++;
				}
				temp[dstpos++] = (unsigned char)(len | 128 | 64);
				spans++;
			}
		}
		if (spans > 0)
		{
			temp[dstpos++] = 0;
			m_RLE[y] = new unsigned char[dstpos];
			memcpy( m_RLE[y], temp, dstpos );
		}
		else
		{
			m_RLE[y] = 0;
		}
		pixels += s->GetWidth();
		alpha += s->GetWidth();
	}
	delete temp;
	delete m_Surf8->GetBuffer();
}

Font::Font( char* a_File, char* a_Chars )
{
	m_Surface = new Surface( a_File );
	Pixel* b = m_Surface->GetBuffer();
	int w = m_Surface->GetWidth();
	int h = m_Surface->GetHeight();
	unsigned int charnr = 0, width = 0, start = 0;
	m_Trans = new int[256];
	memset( m_Trans, 0, 1024 );
	unsigned int i;
	for ( i = 0; i < strlen( a_Chars ); i++ ) m_Trans[(unsigned char)a_Chars[i]] = i;
	m_Offset = new int[strlen( a_Chars )];
	m_Width = new int[strlen( a_Chars )];
	m_Height = h;
	m_CY1 = 0, m_CY2 = SCRHEIGHT - 1;
	int x, y;
	bool lastempty = true;
	for ( x = 0; x < w; x++ )
	{
		bool empty = true;
		for ( y = 0; y < h; y++ ) if (*(b + x + y * w)) 
		{
			if (lastempty)
			{
				width = 0;
				start = x;
			}
			empty = false;
		}
		if ((empty) && (!lastempty))
		{
			m_Width[charnr] = x - start;
			m_Offset[charnr] = start;
			if (++charnr == strlen( a_Chars )) break;
		}
		lastempty = empty;
	}
}

Font::~Font()
{
	delete m_Surface;
	delete m_Trans;
	delete m_Width;
	delete m_Offset;
}

int Font::Width( char* a_Text )
{
	int w = 0;
	unsigned int i;
	for ( i = 0; i < strlen( a_Text ); i++ )
	{
		unsigned char c = (unsigned char)a_Text[i];
		if (c == 32) w += 4; else w += m_Width[m_Trans[c]] + 2;
	}
	return w;
}

void Font::Centre( Surface* a_Target, char* a_Text, int a_Y )
{
	int x = (a_Target->GetWidth() - Width( a_Text )) / 2;
	Print( a_Target, a_Text, x, a_Y );
}
 
void Font::Print( Surface* a_Target, char* a_Text, int a_X, int a_Y, bool clip )
{
	Pixel* b = a_Target->GetBuffer() + a_X + a_Y * a_Target->GetWidth();
 	Pixel* s = m_Surface->GetBuffer();
	unsigned int i, cx;
	int x, y;
	if (((a_Y + m_Height) < m_CY1) || (a_Y > m_CY2)) return;
	for ( cx = 0, i = 0; i < strlen( a_Text ); i++ )
	{
		if (a_Text[i] == ' ') cx += 4; else
		{
			int c = m_Trans[(unsigned char)a_Text[i]];
			Pixel* t = s + m_Offset[c], *d = b + cx;
			if (clip)
			{
				for ( y = 0; y < m_Height; y++ )
				{
					if (((a_Y + y) >= m_CY1) && ((a_Y + y) <= m_CY2))
					{
						for ( x = 0; x < m_Width[c]; x++ ) 
							if ((t[x]) && ((x + (int)cx + a_X) < a_Target->GetWidth())) 
								d[x] = AddBlend( t[x], d[x] );
					}
					t += m_Surface->GetWidth(), d += a_Target->GetWidth();
				}
			}
			else
			{
				for ( y = 0; y < m_Height; y++ )
				{
					if (((a_Y + y) >= m_CY1) && ((a_Y + y) <= m_CY2))
						for ( x = 0; x < m_Width[c]; x++ ) if (t[x]) d[x] = AddBlend( t[x], d[x] );
					t += m_Surface->GetWidth(), d += a_Target->GetWidth();
				}
			}
			cx += m_Width[c] + 2;
			if ((int)(cx + a_X) >= a_Target->GetWidth()) break;
		}
	}
}

void Font::NegPrint( Surface* a_Target, char* a_Text, int a_X, int a_Y )
{
	Pixel* b = a_Target->GetBuffer() + a_X + a_Y * a_Target->GetWidth();
	Pixel* s = m_Surface->GetBuffer();
	unsigned int i, cx;
	int x, y;
	if (((a_Y + m_Height) < m_CY1) || (a_Y > m_CY2)) return;
	for ( cx = 0, i = 0; i < strlen( a_Text ); i++ )
	{
		if (a_Text[i] == ' ') cx += 4; else
		{
			int c = m_Trans[(unsigned char)a_Text[i]];
			Pixel* t = s + m_Offset[c], *d = b + cx;
			for ( y = 0; y < m_Height; y++ )
			{
				if (((a_Y + y) >= m_CY1) && ((a_Y + y) <= m_CY2))
				{
					for ( x = 0; x < m_Width[c]; x++ ) if (t[x]) d[x] = SubBlend( t[x], d[x] );
				}
			#ifdef _DEBUG
				if (y < (m_Height - 1)) t += m_Surface->GetWidth(), d += a_Target->GetWidth();
			#else
				t += m_Surface->GetWidth(), d += a_Target->GetWidth();
			#endif
			}
			cx += m_Width[c] + 2;
		}
	}
}

void Font::NegCentre( Surface* a_Target, char* a_Text, int a_Y )
{
	int x = (a_Target->GetWidth() - Width( a_Text )) >> 1;
	NegPrint( a_Target, a_Text, x, a_Y );
}

void Font::FadeCentre( Surface* a_Target, char* a_Text, int a_X, int a_Y, int a_Scale )
{
	FadePrint( a_Target, a_Text, a_X - (Width( a_Text ) >> 1), a_Y, a_Scale );
}

void Font::FadeCentre( Surface* a_Target, char* a_Text, int a_Y, int a_Scale )
{
	int x = (a_Target->GetWidth() - Width( a_Text )) >> 1;
	FadePrint( a_Target, a_Text, x, a_Y, a_Scale );
}

void Font::FadePrint( Surface* a_Target, char* a_Text, int a_X, int a_Y, int a_Scale )
{
	Pixel* b = a_Target->GetBuffer() + a_X + a_Y * a_Target->GetWidth();
	Pixel* s = m_Surface->GetBuffer();
	unsigned int i, cx;
	int x, y;
	// Pixel color = (a_Scale << 11) + (a_Scale << 6) + a_Scale;
	Pixel color = 0;
	for ( cx = 0, i = 0; i < strlen( a_Text ); i++ )
	{
		if (a_Text[i] == ' ') cx += 4; else
		{
			int c = m_Trans[(unsigned char)a_Text[i]];
			Pixel* t = s + m_Offset[c], *d = b + cx;
			for ( y = 0; y < m_Height; y++ )
			{
				for ( x = 0; x < m_Width[c]; x++ ) if (t[x]) 
				{
					color = (Pixel)(((t[x] & 31) * a_Scale) >> 5);
					d[x] = AddBlend( d[x], color + (color << 6) + (color << 11) );
				}
			#ifdef _DEBUG
				if (y < (m_Height - 1)) t += m_Surface->GetWidth(), d += a_Target->GetWidth();
			#else
				t += m_Surface->GetWidth(), d += a_Target->GetWidth();
			#endif
			}
			cx += m_Width[c] + 2;
		}
	}
}
