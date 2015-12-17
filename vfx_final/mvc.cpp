/*
    Implentmentation Mean Value Coordinates
*/

#include "MVC.h"
#include <cmath>

MVC::MVC()
{
}

MVC::~MVC()
{
}

MVC::MVC( FloatImage3 _img, float **_patch, int _patch_num, int _patch_dim )
{
    srcimg	= _img;
    patch	= _patch;
    patch_num	= _patch_num;
    patch_dim	= _patch_dim;

    int width	= srcimg.width();
    int height	= srcimg.height();
    mvc = new FloatImage1[ patch_num ];
    for( int i = 0; i < patch_num; ++i ) {
	mvc[i].resize( width, height );
    }
    //for( int y = 0; y < height; ++y ) {
    //	for( int x = 0; x < width; ++x ) {
	    // X for every pixel in the source image, compute mvc coordinates.
	    // O for every pixel in the source patch (P_s), compute mvc coordinates.

    for( int y = 50; y < 250; ++y ) {
    	for( int x = 50; x < 350; ++x ) {

	    float weight_sum = 0;
	    float weight[patch_num];
	    for( int i = 0; i < patch_num; ++i ) {	

		int p[3] = {i-1, i, i+1};
		if( i-1 < 0 ) {
		    p[0] = patch_num - 1;
		}
		else if( i+1 > patch_num - 1) {
		    p[2] = 0;
		}
		
		float tan_alpha[2] = {0, 0};
		for( int j = 0; j < 2; ++j ) { 
		    float dx1 = patch[p[j]][0] - x;	    // vec1
		    float dy1 = patch[p[j]][1] - y;
		    float dx2 = patch[p[j+1]][0] - x;	    // vec2
		    float dy2 = patch[p[j+1]][1] - y;
		    float dot = ( dx1 * dx2 + dy1 * dy2 ) / 
				( pow((pow(dx1, 2) + pow(dy1, 2)), 0.5)*pow( (pow(dx2, 2)+ pow(dy2, 2)), 0.5) );
//		    cout << "dx1 = " << dx1 << " dy1 = " << dy1 <<" dot = " << dot << endl;
		    tan_alpha[j] = tan( acos(dot)*0.5 );
		}
//		cout << "tan_alpha[0] = " << tan_alpha[0] << " tan_alpha[1] = " << tan_alpha[1] << endl;	
		weight[i] = ( tan_alpha[0] + tan_alpha[1] )/
			    pow(( (patch[i][0] - x)*(patch[i][0] - x) +
			          (patch[i][1] - y)*(patch[i][1] - y) ), 0.5);
		weight_sum += weight[i];
	    }

	    for( int i = 0; i < patch_num; ++i ) {
		mvc[i](x, y) = weight[i] / weight_sum;	    // lambda_i(x)
	    }
	}
    }
}

FloatImage3 MVC::cloning( FloatImage3 tarimg, float **tar_patch )
{
    int width = tarimg.width();
    int height = tarimg.height();
    FloatImage3 ret = tarimg;

    for( unsigned int c = 0; c < tarimg.channels(); ++c ) {
	
	// compute the differences along the boundary.
	float *diff = new float[ patch_num ];
	for( int i = 0; i < patch_num; ++i ) {
	    
	    int tar_x = tar_patch[i][0];
	    int tar_y = tar_patch[i][1];
	    int src_x = patch[i][0];
	    int src_y = patch[i][1];
	    diff[i] = tarimg(tar_x, tar_y)[c] - srcimg(src_x, src_y)[c]; // intensity
	}
	
	FloatImage1 r( width, height );
	//for( int y = 0; y < height; ++y ) {
	//    for( int x = 0; x < width; ++x ) {
	for( int y = 50; y < 250; ++y ) {
	    for( int x = 50; x < 350; ++x ) {
		
		// evaluate the mean-value interpolant at each pixel in P_t.
		r(x, y) = 0;
		for( int i = 0; i < patch_num; ++i ) {
		    //if( mvc[i](x,y) < 0 ) mvc[i](x, y) = 0;
		    r(x, y) += mvc[i](x, y) * diff[i];
		}
		ret(x, y)[c] = srcimg(x, y)[c] + r(x, y); // intensity
	    }
	}

	delete [] diff;
    }
    return ret;
}
