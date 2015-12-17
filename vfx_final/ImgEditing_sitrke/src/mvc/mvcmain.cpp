#include <iostream>
#include <fstream>
using namespace std;

#include "Vec2.h"
#include "MVC.h"
#include <vector>

#include <ctime>


int main( int argc, char** argv )
{
    if( argc < 3 ) {
	cout << "Usage: ./main src.jpg tar.jpg" << endl;
	cout << acos(float(1.000000)) << endl;
	exit(-1);
    }

    FloatImage3 srcImg;
    FloatImage3 tarImg;
    read( srcImg, argv[1] );
    read( tarImg, argv[2] );

    Vec2 srcStart( 50, 40 );
    Vec2 srcEnd( 360, 250 );
    Vec2 offset( 60, 50 );
    Vec2 tarStart = srcStart + offset;
    Vec2 tarEnd = srcEnd + offset;
     
    ByteImage1 srcMask(srcImg.width(), srcImg.height());
    for( int y = srcStart.y + 1; y < srcEnd.y; ++y ) {
	for( int x = srcStart.x + 1; x < srcEnd.x; ++x ) {
	    srcMask(x, y) = 255;
	}
    }
    
    ByteImage1 tarMask(tarImg.width(), tarImg.height());
    for( int y = tarStart.y + 1; y < tarEnd.y; ++y ) {
	for( int x = tarStart.x + 1; x < tarEnd.x; ++x ) {
	    tarMask(x, y) = 255;
	}
    }

    vector< Vec2 > srcEdge;
    {
	int x = srcStart.x;
	int y = srcStart.y;
	for( ; x < srcEnd.x; ++x ) {
	    Vec2 a(x, y);
	    srcEdge.push_back(a);
	}

	for( ; y < srcEnd.y; ++y ) {
	    Vec2 a(x, y);
	    srcEdge.push_back(a);
	}

	for( ; x > srcStart.x; --x ) {
	    Vec2 a(x, y);
	    srcEdge.push_back(a);
	}

	for( ; y > srcStart.y; --y ) {
	    Vec2 a(x, y);
	    srcEdge.push_back(a);
	}
    }
    vector< Vec2 > tarEdge;
    {
	int x = tarStart.x;
	int y = tarStart.y;
	for( ; x < tarEnd.x; ++x ) {
	    Vec2 a(x, y);
	    tarEdge.push_back(a);
	}

	for( ; y < tarEnd.y; ++y ) {
	    Vec2 a(x, y);
	    tarEdge.push_back(a);
	}

	for( ; x > tarStart.x; --x ) {
	    Vec2 a(x, y);
	    tarEdge.push_back(a);
	}

	for( ; y > tarStart.y; --y ) {
	    Vec2 a(x, y);
	    tarEdge.push_back(a);
	}
    }
 
    clock_t start,end;
    
    start = clock();
    MVC *srcMVC1 = new MVC( srcImg, srcMask, srcEdge );
    end = clock();
    fprintf( stderr, "MVC: %lf seconds.\n", double(end - start) / CLOCKS_PER_SEC);
    
    start = clock();
    FloatImage3 cloningImg1 = srcMVC1->cloning( tarImg, tarMask, tarEdge );
    end = clock();
    printf( "cloning: %lf seconds.\n", double(end - start) / CLOCKS_PER_SEC);
    
    write( cloningImg1, "../../../../public-html/show/vfx/out_time_vector.jpg" );
    
    
    start = clock();
    MVC *srcMVC2 = new MVC( srcImg, srcMask );
    end = clock();
    fprintf( stderr, "MVC: %lf seconds.\n", double(end - start) / CLOCKS_PER_SEC);

    start = clock();
    vector< Vec2 > ttarEdge;
    ByteImage1 tarEdgeMask;
    srcMVC2->find_edge( tarMask, ttarEdge, tarEdgeMask );
    Vec2 ooffset = ttarEdge[0] - srcEdge[0];
    FloatImage3 cloningImg2 = srcMVC2->cloning( tarImg, ooffset );
    end = clock();
    printf( "cloning: %lf seconds.\n", double(end - start) / CLOCKS_PER_SEC);
    
    write( cloningImg2, "../../../../public-html/show/vfx/out_time_vector_srcEdge.jpg" );
 /*
    int edgeLen = srcMVC1->_srcEdge.size();
    int counter = 0;
    for( int i = 0; i < edgeLen; ++i ) {
	if( srcMVC1->_srcEdge[i] != srcMVC2->_srcEdge[i] ) {
	    if ( counter > 400 ) break;
	    fprintf( stderr, "MVC1[%d] = (%.0f, %.0f)         ", i, srcMVC1->_srcEdge[i].x, srcMVC1->_srcEdge[i].y );
	    fprintf( stderr, "MVC2[%d] = (%.0f, %.0f)\n", i, srcMVC2->_srcEdge[i].x, srcMVC2->_srcEdge[i].y );
	    counter += 1;
	}
    }
    fprintf( stderr, "counter = %d\n", counter );
   */ 
    return 0;
}
