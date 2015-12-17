/*
 * GrabCut implementation source code Copyright(c) 2005-2006 Justin Talbot
 *
 * All Rights Reserved.
 * For educational use only; commercial use expressly forbidden.
 * NO WARRANTY, express or implied, for this software.
 */

#ifndef GMMINC_H
#define GMMINC_H

#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

// Choose accuracy of computations (double doesn't seem to work for me yet.)
#define Real double

// User supplied Trimap values
enum TrimapValue { TrimapUnknown, TrimapForeground, TrimapBackground };

// Grabcut derived hard segementation values
enum SegmentationValue { SegmentationForeground, SegmentationBackground };


// Helper function, finds distance between two pixels
inline Real distance(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	return sqrt((Real)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}

// Storage for N-link weights, each pixel stores links to only four of its 8-neighborhood neighbors.
// This avoids duplication of links, while still allowing for relatively easy lookup.
struct NLinks
{
	Real upleft;
	Real up;
	Real upright;
	Real right;
};

// Images, really just a templatized 2D array. We use this for all the image variables.

template<class T>
class GmmImage
{

public:
	GmmImage(unsigned int width, unsigned int height);
	~GmmImage();

	T* ptr() { return m_image; }

	T& operator() (int x, int y) { clampX(x); clampY(y); return m_image[y*m_width+x]; }
	const T& operator() (int x, int y) const { clampX(x); clampY(y); return m_image[y*m_width+x]; }

	void fillRectangle(int x1, int y1, int x2, int y2, const T& t);
	void fill(const T& t);

	unsigned int width() const { return m_width; }
	unsigned int height() const { return m_height; }

private:

	void clampX(int& x) const { if (x < 0) x = 0; if (x >= (int)m_width)  x = m_width-1; }
	void clampY(int& y) const { if (y < 0) y = 0; if (y >= (int)m_height) y = m_height-1; }

	T* m_image;
	unsigned int m_width, m_height;
};


// GmmImage member functions
template<class T>
GmmImage<T>::GmmImage(unsigned int width, unsigned int height) : m_width(width), m_height(height)
{
	m_image = new T[m_width*m_height];
}

template<class T>
GmmImage<T>::~GmmImage()
{
	if (m_image)
		delete [] m_image;
}

template<class T>
void GmmImage<T>::fillRectangle(int x1, int y1, int x2, int y2, const T& t)
{
	clampX(x1); clampY(y1);
	clampX(x2); clampY(y2);
	
	if(y1>y2) {int t=y1; y1=y2; y2=t;}
	if(x1>x2) {int t=x1; x1=x2; x2=t;}

	for (int i = y1; i <= y2; ++i)
	{
		for (int j = x1; j <= x2; ++j)
		{
			m_image[i*m_width+j] = t;
		}
	}
}

template<class T>
void GmmImage<T>::fill(const T& t)
{
	for (unsigned int i = 0; i < m_width*m_height; ++i) 
	{
		m_image[i] = t;
	}
}

class GmmColor {

public:

	GmmColor() : r(0), g(0), b(0) {}
	GmmColor(Real _r, Real _g, Real _b) : r(_r), g(_g), b(_b) {}

	Real r, g, b;
};

// Compute squared distance between two colors
inline Real distance2( const GmmColor& c1, const GmmColor& c2 )
{
	return ((c1.r-c2.r)*(c1.r-c2.r)+(c1.g-c2.g)*(c1.g-c2.g)+(c1.b-c2.b)*(c1.b-c2.b));
}
#endif
