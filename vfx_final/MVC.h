/*
    Implentmentation Mean Value Coordinates
*/

#ifndef __MVC_H__
#define __MVC_H__

#include "main.h"
#include <cmlib/image.h>
#include <cmlib/imageio/all.h>
#include <cmlib/imageio.h>

using namespace cmlib::image;

using namespace std;

class MVC
{
    public:
	MVC();
	~MVC();
	
	MVC( FloatImage3 _img, float **_patch, int _patch_num, int _patch_dim );
	FloatImage3 cloning( FloatImage3 _tarimg, float **_patch );

    private:
	FloatImage3 srcimg;
	float **patch;	    // edge points posision, counterclock-wise
	int patch_num;	    // number of edge points,			    e.g. 0, 1, ..., m-1
	int patch_dim;	    // number of dimensionalities of edge point,    e.g. 2
	FloatImage1 *mvc;   // pointer of MVC; all mvc value should >=0
};

#endif
