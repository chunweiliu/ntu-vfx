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
    
    FloatImage1 srcMask(srcImg.width(), srcImg.height());
    for( int y = srcStart.y + 1; y < srcEnd.y; ++y ) {
	for( int x = srcStart.x + 1; x < srcEnd.x; ++x ) {
	    srcMask(x, y) = 1.0;
	}
    }
    
    FloatImage1 tarMask(tarImg.width(), tarImg.height());
    for( int y = tarStart.y + 1; y < tarEnd.y; ++y ) {
	for( int x = tarStart.x + 1; x < tarEnd.x; ++x ) {
	    tarMask(x, y) = 1.0;
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
    
    time_t start,end;
    time( &start );
    MVC *srcMVC = new MVC( srcImg, srcMask, srcEdge );
    time( &end );
    printf( "MVC: %.2lf seconds.\n", difftime( end, start ) );
    
    time( &start );
    FloatImage3 cloningImg = srcMVC->cloning( tarImg, tarMask, tarEdge );
    time( &end );
    printf( "cloning: %.2lf seconds.\n", difftime( end, start ) );
    
    write( cloningImg, "../../../../public-html/show/vfx/out_time_edge.jpg" );
    return 0;
}
