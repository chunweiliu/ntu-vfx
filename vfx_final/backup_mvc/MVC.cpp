/*
    MVC.cpp 
*/
//#define DEBUG
#include "MVC.h"

MVC::MVC( const FloatImage3& _srcImg, const FloatImage1& _srcMask, const vector< Vec2 >& _srcEdge )
:   srcImg( _srcImg ),
    srcMask( _srcMask ),
    srcEdge( _srcEdge )
{
    int width	= srcImg.width();
    int height	= srcImg.height();
    int edgeLen	= srcEdge.size();

    // initial mvc coordinates
    mvc		= new FloatImage1[ edgeLen ];
    for( int i = 0; i < edgeLen; ++i ) {
	mvc[i].resize( width, height );
    }

    // compute the mean-value coordinates of (x, y) in srcMask w.r.t srcEdge
    for( int y = 0; y < height; ++y ) {
	for( int x = 0; x < width; ++x ) {
	    
	    if( srcMask(x, y) != 1.0 ) continue;
	    
	    get_mvc( x, y, edgeLen );
	}
    }
}

inline void MVC::get_mvc( const int& x, const int& y, const int& edgeLen )
{
    int p_idx[3];
    Vec2 p[3];
    float tan_alpha, theta;
    Vec2 v1, v2;
    float* weight = new float[ edgeLen ];

    Vec2 a(x, y);
    float weight_sum = 1e-10;
    for( int i = 0; i < edgeLen; ++i ) {

	// decide point pairs p_{i-1}, p_i, p_{i+1} of each iteration
	for( int j = 0; j < 3; ++j )
	    p_idx[j] = (i - 1) + j;

	if( i == 0 ) p_idx[0] = edgeLen - 1;
	else if( i == edgeLen - 1 ) p_idx[2] = 0;

	for( int j = 0; j < 3; ++j )
	    p[j] = srcEdge.at( p_idx[j] );

	// compute tan_alpha
	tan_alpha = 0;
	for( int j = 0; j < 2; ++j ) {
	    
	    v1 = p[j] - a;
	    v2 = p[j+1] - a;

	    v1.normalise();
	    v2.normalise();
#ifdef DEBUG
	    float dp = v1.dot(v2);
	    if( dp > 1 || dp < -1 )
		printf( "(%f, %f)dot(%f, %f) = %f\n", v1.x, v1.y, v2.x, v2.y, dp );
#endif
	    theta = angle_between_normalised( v1, v2 );
	    tan_alpha += tan( theta * 0.5 );
	}

	// sum up weight for normalization
	weight[i] = tan_alpha / (srcEdge.at(i) - a).length();
	weight_sum += weight[i];
    }

    weight_sum = 1.0 / weight_sum;
    for( int i = 0; i < edgeLen; ++i )
	mvc[i](a.x, a.y) = weight[i] * weight_sum;

    delete [] weight;
}

FloatImage3 MVC::cloning( const FloatImage3& tarImg,
			  const FloatImage1& tarMask, const vector< Vec2 >& tarEdge ) const
{
  
    // compute the difference along the boundary
    FloatImage3 ret = tarImg;
    
    int edgeLen = srcEdge.size();
    float *diff = new float[ edgeLen ];

    int width	= tarImg.width();
    int height	= tarImg.height();
    Vec2 offset	= srcEdge[0] - tarEdge[0];

    for( unsigned int c = 0; c < tarImg.channels(); ++c ) {
	for( int i = 0; i < edgeLen; ++i ) {	    
	    diff[i] = tarImg( (int)tarEdge[i].x, (int)tarEdge[i].y )[c] - 
	    	      srcImg( (int)srcEdge[i].x, (int)srcEdge[i].y )[c];
	}

	FloatImage1 r( width, height );
	for( int y = 0; y < height; ++y ) {
	    for( int x = 0; x < width; ++x ) {
		
		if( tarMask(x, y) != 1 ) continue;

		// evaluate the mean-value interpolant at (x, y)
		for( int i = 0; i < edgeLen; ++i ) {
		    r(x, y) += ( mvc[i](x+offset.x, y+offset.y) * diff[i] );
		}
	
		ret(x, y)[c] = srcImg(x+offset.x, y+offset.y)[c] + r(x, y);
		if( ret(x, y)[c] > 1.0 ) ret(x, y)[c] = 1.0;
		else if( ret(x, y)[c] < 0.0 ) ret(x, y)[c] = 0.0;
	    }
	}
    }
    delete [] diff;

    return ret;
    
}
