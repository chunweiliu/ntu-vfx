/*
    MVC.cpp 
*/
//#define DEBUG
//#define MESH
//#define EDGE
#include "MVC.h"

MVC::MVC( const FloatImage3& srcImg, const ByteImage1& srcMask, const vector< Vec2 >& srcEdge )
:   _srcImg( srcImg ),
    _srcMask( srcMask ),
    _srcEdge( srcEdge )
{
    int width	= _srcImg.width();
    int height	= _srcImg.height();

    // initial weight
    int edgeLen	= _srcEdge.size();
    _weight = vector<float>(edgeLen);

    // compute the mean-value coordinates of (x, y) in srcMask w.r.t srcEdge
    for( int y = 0; y < height; ++y ) {
	for( int x = 0; x < width; ++x ) {
	    
	    if( _srcMask(x, y) != 255 ) continue;
	    
	    get_mvc( x, y, edgeLen );
	}
    }
}

MVC::MVC( const FloatImage3& srcImg, const ByteImage1& srcMask )
    : _srcImg( srcImg ),
    _srcMask( srcMask )
{
    int width	= _srcImg.width();
    int height	= _srcImg.height();

    //_srcEdgeMask.resize( width, height );
    find_edge( _srcMask, _srcEdge, _srcEdgeMask );
    int edgeLen = _srcEdge.size();
    _weight = vector<float>(edgeLen);

    //fprintf( stderr, "edgeLen = %d\n", edgeLen );
    for( int y = 0; y < height; ++y ) {
	for( int x = 0; x < width; ++x ) {

	    if( _srcMask(x, y) != 255 || _srcEdgeMask(x, y) == 255 ) continue;

	    get_mvc( x, y, edgeLen );
	}
    }
}

inline void MVC::find_edge( const ByteImage1& mask, vector< Vec2 >& edge, ByteImage1& edgeMask )
{
    int width = mask.width();
    int height = mask.height();    
    edgeMask.resize( width, height );

    IplImage *contourMap = cvCreateImage( cvSize( width, height ), 8, 1 ); // each pixel = 8 bits, 1 chanel
    for( int y = 0; y < height; ++y ) {
	for( int x = 0; x < width; ++x ) {

	    if( mask(x, y) == 255 )
		cvSetReal2D( contourMap, y, x, 255 );	    
	    else
		cvSetReal2D( contourMap, y, x, 0 );		
	    
	}
    }

    CvMemStorage *storage = cvCreateMemStorage( 0 );
    CvSeq *contour = NULL;
    cvFindContours( contourMap, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
    cvZero(contourMap);
    
    // storage line boundary in a sequence    
    CvPoint *pt = (CvPoint *)cvGetSeqElem( contour, 0 );
    Vec2 a( pt->x, pt->y );
    edge.push_back( a );

    edgeMask( (int)a.x, (int)a.y ) = 255;
 
    for( int i = 0; contour != NULL && i < contour->total; ++i ) {
	
	// there may be points between the line
	// we must go along the line
	CvPoint* pt1 = (CvPoint *)cvGetSeqElem(contour, i);
	CvPoint* pt2 = (i == contour->total - 1)
	    ? (CvPoint *)cvGetSeqElem(contour, 0)
	    : (CvPoint *)cvGetSeqElem(contour, i + 1);

	CvLineIterator iterator;
	int count = cvInitLineIterator(contourMap, *pt1, *pt2, &iterator, 8, 0);
	const unsigned char* start = reinterpret_cast<unsigned char *>(contourMap->imageData);

	CV_NEXT_LINE_POINT(iterator);
	for(int j = 1; j < count; j++) {
	    int coord = iterator.ptr - start;
	    int y = coord / contourMap->widthStep;
            int x = coord - y * contourMap->widthStep;

	    Vec2 a(x, y);
	    edge.push_back( a );
	    edgeMask( x, y ) = 255;
	    CV_NEXT_LINE_POINT(iterator);	    
	}
    }

    // the first point is already in the vector
    // pop the last element for duplication
    edge.pop_back();
    cvReleaseImage( &contourMap );
}

MVC::~MVC()
{
    //delete [] weight;
}

inline void MVC::get_mvc( const int& x, const int& y, const int& edgeLen )
{
    int p_idx[3];
    Vec2 p[3];
    float tan_alpha, dp;
    Vec2 v1, v2;

    Vec2 a(x, y);
    float weight_sum = 1e-10;
    for( int i = 0; i < edgeLen; ++i ) {

#ifdef MESH
	if( i % 2 < 1 ) continue;
#endif

#ifdef EDGE
	if( (a - _srcEdge[i]).length()  > (edgeLen * 0.025) ) {
	    continue;
	}
#endif

	// decide point pairs p_{i-1}, p_i, p_{i+1} of each iteration
	for( int j = 0; j < 3; ++j )
	    p_idx[j] = (i - 1) + j;

	if( i == 0 ) p_idx[0] = edgeLen - 1;
	else if( i == edgeLen - 1 ) p_idx[2] = 0;

	for( int j = 0; j < 3; ++j )
	    p[j] = _srcEdge[ p_idx[j] ];

	// compute tan_alpha
	tan_alpha = 0;
	for( int j = 0; j < 2; ++j ) {
	    
	    v1 = p[j] - a;
	    v2 = p[j+1] - a;

	    v1.normalise();
	    v2.normalise();
	    //theta = angle_between_normalised( v1, v2 );
	    //tan_alpha += tan( theta * 0.5 );
	    dp = v1.dot(v2);
	    if( 1-dp < 0 ) dp = 1.;
	    tan_alpha += sqrt((1-dp)/(1+dp));
	}

	// sum up weight for normalization
	_weight[i] = tan_alpha / (_srcEdge[i] - a).length();
	weight_sum += _weight[i];
    }

    weight_sum = 1.0 / weight_sum;
    for( int i = 0; i < edgeLen; ++i ) {
	_weight[i] = _weight[i] * weight_sum;
	_mvc.push_back( _weight[i] );
    }

    _srcIner.push_back( a );
}

FloatImage3 MVC::cloning( const FloatImage3& tarImg,
			  const ByteImage1& tarMask, const vector< Vec2 >& tarEdge ) const
{
  
    // compute the difference along the boundary
    FloatImage3 ret = tarImg;
    
    int edgeLen = _srcEdge.size();
    float *diff = new float[ edgeLen ];

    int width	= tarImg.width();
    int height	= tarImg.height();
    Vec2 offset	= tarEdge[0] - _srcEdge[0];

    for( unsigned int c = 0; c < tarImg.channels(); ++c ) {
	for( int i = 0; i < edgeLen; ++i ) {	    
	    diff[i] = tarImg( (int)tarEdge[i].x, (int)tarEdge[i].y )[c] - 
		      _srcImg( (int)_srcEdge[i].x, (int)_srcEdge[i].y )[c];
	}

	FloatImage1 r( width, height );
	for( unsigned int v = 0; v < _srcIner.size(); ++v ){

            int x = (int)_srcIner[v].x + (int)offset.x;
            int y = (int)_srcIner[v].y + (int)offset.y;
	    // evaluate the mean-value interpolant at (x, y)
	    for( int i = 0; i < edgeLen; ++i ) 
		r(x, y) += ( _mvc[v*edgeLen + i] * diff[i] );
	    
            ret(x, y)[c] = _srcImg( (int)_srcIner[v].x, (int)_srcIner[v].y )[c] + r(x, y);
	    // color clipping
	    if( ret(x, y)[c] > 1.0 ) ret(x, y)[c] = 1.0;
	    else if( ret(x, y)[c] < 0.0 ) ret(x, y)[c] = 0.0;
	}
    }
    delete [] diff;

    return ret;
    
}

FloatImage3 MVC::cloning( const FloatImage3& tarImg, const Vec2& offset ) const
{
    // compute the difference along the boundary
    FloatImage3 ret = tarImg;
    
    int edgeLen = _srcEdge.size();
    float *diff = new float[ edgeLen ];

    int width	= tarImg.width();
    int height	= tarImg.height();


    for( unsigned int c = 0; c < tarImg.channels(); ++c ) {
	for( int i = 0; i < edgeLen; ++i ) {	    
            diff[i] = tarImg( (int)_srcEdge[i].x + (int)offset.x, (int)_srcEdge[i].y + (int)offset.y )[c] -
		      _srcImg( (int)_srcEdge[i].x, (int)_srcEdge[i].y )[c];
	}
	
	FloatImage1 r( width, height );
	for( unsigned int v = 0; v < _srcIner.size(); ++v ){

            int x = (int)_srcIner[v].x + (int)offset.x;
            int y = (int)_srcIner[v].y + (int)offset.y;
	    
	    // evaluate the mean-value interpolant at (x, y)
	    for( int i = 0; i < edgeLen; ++i ) 
		r(x, y) += ( _mvc[v*edgeLen + i] * diff[i] );
	    
            ret(x, y)[c] = _srcImg((int)_srcIner[v].x, (int)_srcIner[v].y )[c] + r(x, y);
	    // color clipping
	    if( ret(x, y)[c] > 1.0 ) ret(x, y)[c] = 1.0;
	    else if( ret(x, y)[c] < 0.0 ) ret(x, y)[c] = 0.0;
	}
    }
    delete [] diff;

    return ret;
    
}
