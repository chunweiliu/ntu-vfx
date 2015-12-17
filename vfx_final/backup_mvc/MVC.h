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

class MVC
{
public:
    
    inline MVC()
    {}

    inline ~MVC()
    {}

    MVC( const FloatImage3& _srcImg, const FloatImage1& _srcMask, const vector< Vec2 >& _srcEdge );

    inline void get_mvc( const int& x, const int& y, const int& edgeLen );
    
    FloatImage3 cloning( const FloatImage3& _tarImg,
			 const FloatImage1& _tarMask, const vector< Vec2 >& _tarEdge ) const;
private:
    FloatImage3	    srcImg;
    FloatImage1	    srcMask;
    vector< Vec2 >  srcEdge;
    FloatImage1	    *mvc;

};

#endif
