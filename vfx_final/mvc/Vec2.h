#ifndef __Vec2_h__
#define __Vec2_h__

#include <cmath>

class Vec2
{
public:
    
    inline Vec2()
    {}

    inline ~Vec2()
    {}

    inline Vec2( float _x, float _y )
    : x(_x), y(_y)
    {}

    inline Vec2( const Vec2& rhs )  // copy constructor
    : x(rhs.x), y(rhs.y)
    {}

    inline void set( float newx, float newy )
    {
	x = newx;
	y = newy;
    }

    inline const Vec2 operator + ( const Vec2& rhs ) const
    {
	return Vec2( x + rhs.x, y + rhs.y );
    }

    inline const Vec2 operator - ( const Vec2& rhs ) const
    {
	return Vec2( x - rhs.x, y - rhs.y );
    }
    
    inline Vec2& operator += ( const Vec2& rhs)
    {
	x += rhs.x;
	y += rhs.y;
	return *this;
    }

    inline Vec2& operator -= ( const Vec2& rhs)
    {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
    }

    inline Vec2& operator = ( const Vec2& rhs )
    {
	x = rhs.x;
	y = rhs.y;
	return *this;
    }

    inline bool operator == ( const Vec2& rhs ) const
    {
	return ( (x == rhs.x) && (y == rhs.y) );
    }

    inline bool operator != ( const Vec2& rhs ) const
    {
	return ( (x != rhs.x) || (y != rhs.y) );
    }

    inline float length() const
    {
	return (float)sqrt(x*x + y*y);
    }

    inline float dot( const Vec2& rhs ) const
    {
	return x*rhs.x + y*rhs.y;
    }

    inline void normalise()
    {
	if(!x && !y)
	    return;

	const float inverseLength = 1.0f / length();
	
	x *= inverseLength;
	y *= inverseLength;
    }

    float x, y;
};

inline float dot_product( const Vec2& v1, const Vec2& v2 )
{
    return (v1.x * v2.x) + (v1.y * v2.y);
}

inline float angle_between_normalised( const Vec2& v1, const Vec2& v2 )
{
    float dp = dot_product( v1, v2 );
    if( dp > 1.0 ) dp = 1.0;
    if( dp < -1.0 ) dp = -1.0;
    return (float) acos( dp );
}

#endif
