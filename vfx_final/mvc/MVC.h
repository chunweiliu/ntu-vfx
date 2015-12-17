/*
   Implemet Mean-Value Coordinates 
*/

#ifndef __MVC_H__
#define __MVC_H__

#include <cmlib/image.h>
#include <cmlib/imageio/all.h>
#include <cmlib/imageio.h>

using namespace cmlib::image;
using namespace std;

#include <vector>
#include "Vec2.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

class MVC
{
public:
    
    MVC() { }
    MVC(const MVC& m) { }


    ~MVC();
    

    MVC( const FloatImage3& srcImg, const ByteImage1& srcMask, const vector< Vec2 >& srcEdge );

    MVC( const FloatImage3& srcImg, const ByteImage1& srcMask );

    inline void get_mvc( const int& x, const int& y, const int& edgeLen );
    
    FloatImage3 cloning( const FloatImage3& tarImg,
			 const ByteImage1& tarMask, const vector< Vec2 >& tarEdge ) const;

    FloatImage3 cloning( const FloatImage3& tarImg, const Vec2& offset ) const;

    inline void find_edge( const ByteImage1& mask, vector< Vec2 >& edge, ByteImage1& edgeMask );

    //inline void bfs_edge( const Vec2& startPoint, ByteImage1& bfsMask, int ptr[][2] );

    
private:
    FloatImage3		_srcImg;
    ByteImage1		_srcMask;	// binary index of inner
    ByteImage1		_srcEdgeMask;	// binary index of edge
    vector< Vec2 >	_srcEdge;	// vector of edge, convient for retrival
    //FloatImage1	    *mvc;
    vector< float >	_mvc;	    // the mvc values !!!!! 2D array implement by 1D !!!!!
    vector< Vec2 >	_srcIner;	// the pixel of P_s
    //float		*weight;
    vector< float >	_weight;
    //static const int		_pointDelta[8][2] = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0},
    //	    	                              {1, 0}, {-1, 1}, {0, 1}, {1, 1} };

};

#endif
