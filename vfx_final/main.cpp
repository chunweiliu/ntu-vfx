#include <iostream>
#include <fstream>
using namespace std;

#include "MVC.h"
#include <vector>

float** read_array( int&, int& );

int main(int argc, char** argv)
{
    if( argc < 2 ){
        cout << "Usage: ./main src.jpg tar.jpg" << endl;
        return 0;
    }

    FloatImage3 srcimg;
    FloatImage3 tarimg;
    read(srcimg, argv[1]);
    read(tarimg, argv[2]);

    int num = 0;
    int dim = 0;
    float **p = read_array( num, dim );
    
    MVC *src_mvc = new MVC( srcimg, p, num, dim );
    
    float **tarp = p;
    FloatImage3 cloning_img = src_mvc->cloning( tarimg, tarp );

    write( cloning_img, "../../../public-html/show/vfx/out.jpg" );

    return 0;
}

float** read_array( int &num, int &dim ){
    ifstream in("ps.txt");
    if(!in){
	cout << "Cannot open file ps.txt" << endl;
	return 0;
    }

    in >> num >> dim;

    float **p = new float *[num];
    for( int i=0; i<num; ++i ){
	p[i] = new float[dim];
	for( int j=0; j<dim; ++j ){
	    in >> p[i][j];
	}
    }
    //vector<vector<int> > Array(num_ps, vector<int>(2)); 
    
    in.close();
    return p;
}
