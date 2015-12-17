
#include "allinc.h"
#include "bicgstab.h"

// first the struct for the matrix 
struct PoissonMatrix {
    PoissonMatrix(int width, int height);
    ~PoissonMatrix();
    int n;
    int w;
    int h;
    double **odiag, *diag;
    int pos[4][2];

};

struct PoissonMaskMatrix {
    PoissonMaskMatrix(int width, int height, const ByteImage1& mask);
    
    int w;
    int h;
    int n;
    int nreal;
    vector<vector<int> > diag; 
    vector<int> diagidx;
};

// then we need the multiplication to a vector 
void mult( const PoissonMatrix &T, const double *v, double *w );   
void mult( const PoissonMaskMatrix &T, const double *v, double *w);

int linearsolve(int width, int height, double* b, double* x, double EPS = 1e-10);
int linearsolve(int width, int height, double* b, double* x, const ByteImage1& mask, double EPS = 1e-10);

class Poisson {
    public:
	Poisson(int gflag = 0, double _eps = 1e-4) {
	    gradient_flag = gflag;
	    lsolve_eps = _eps;
	    gradient_thres = 0.f;
	    if(gflag == 2) {
		gradient_thres = 0.2;
	    }
	}
	
	// cloning of whoe G to part of F in all channel
	FloatImage3& cloning(FloatImage3& F, FloatImage3& G, int x, int y);
	FloatImage3& cloning(FloatImage3& F, FloatImage3& G, const ByteImage1& mask, int x, int y);
	// set gradient threshold
	inline void setgradthres(const double& val) {gradient_thres = val;}
	// set gradient flag
	inline void setgradflag(const int& val) {gradient_flag = val;}
	// set linear solve eps
	inline void setlseps(const double& val) {lsolve_eps = val;}

    private:
	FloatImage3 tarimg;
	int gradient_flag;
        double gradient_thres;
	double lsolve_eps;
};



