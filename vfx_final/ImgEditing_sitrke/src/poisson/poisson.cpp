

#include "poisson.h"

PoissonMatrix::PoissonMatrix(int width, int height) : n(width*height), w(width), h(height) { 
    int _pos[8] = {0, 1, 0, w, 1, 0, w, 0};
    int _k = 0;
    for(int i = 0; i < 4; i++) {
	for(int j = 0; j < 2; j++) {
	    pos[i][j] = _pos[_k++];
	}
    }

    diag = new double[n];
    // initial diag
    for(int i = 0; i < n; i++) {
	diag[i] = 0.f;
    }

    odiag = new double*[4];
    // for upper and lower diag
    for(int i = 0; i < 4; i++) {
	odiag[i] = new double[n];
	if(pos[i][1] != 0) {	// upper diag
	    for(int j = 0; j < n; j++) {
		bool bound_flag = (j / w == 0) || (j / w == h - 1) || (j % w == 0) || (j % w == w - 1);
		if(bound_flag) {
		    diag[j] = 1.f;
		}
		if(j < n - pos[i][1] && !bound_flag) {
		    diag[j] += 1.f;
		    odiag[i][j] = -1.f;
		} else{
		    odiag[i][j] = 0.f;
		}
	    }
	} else {			// lower diag
	    for(int j = 0; j < n; j++) {
		bool bound_flag = (j / w == 0) || (j / w == h - 1) || (j % w == 0) || (j % w == w - 1);
		if(bound_flag) {
		    diag[j] = 1.f;
		}
		if(j >= pos[i][0] && !bound_flag) {
		    diag[j] += 1.f;
		    odiag[i][j] = -1.f;
		} else{
		    odiag[i][j] = 0.f;
		}
	    }
	}
    }
#ifdef DEBUG_POISSON
    for(int i = 0; i < n; i++) {
	printf("%.2lf %.2lf %.2lf %.2lf %.2lf\n", diag[i], odiag[0][i], odiag[1][i], odiag[2][i], odiag[3][i]);
    }
#endif
}

PoissonMatrix::~PoissonMatrix() {
    delete[] diag;
    delete[] odiag[0];
    delete[] odiag[1];
    delete[] odiag[2];
    delete[] odiag[3];
    delete[] odiag;
}

PoissonMaskMatrix::PoissonMaskMatrix(int width, int height, const ByteImage1& mask) : w(width), h(height), n(width*height) {
    diagidx.resize(n);
    int k = -1;
    for(int nowh = 0; nowh < h; nowh++) {
        for(int noww = 0; noww < w; noww++) {
	    k++;
	    if(mask(noww, nowh) == 0) {
		diagidx[k] = -1;
		continue;
	    }
	    vector<int> nowpix(7, -1);
	    bool bound_flag = (noww == 0) || (nowh == 0) || (noww == w - 1) || (nowh == h - 1);
	    if(bound_flag 
		    || (int)mask(noww - 1, nowh) * (int)mask(noww, nowh - 1) * (int)mask(noww + 1, nowh) * (int)mask(noww, nowh + 1) == 0) {
		nowpix[1] = noww;
		nowpix[2] = nowh;

		if(k > 0 && diagidx[k - 1] >= 0 && diag[diagidx[k - 1]][0] >= 0) {
		    diag[diagidx[k - 1]][5] = (int)diag.size();
		}
		if(k - w >= 0 && diagidx[k - w] >= 0 && diag[diagidx[k - w]][0] >= 0) {
		    diag[diagidx[k - w]][6] = (int)diag.size();
		}
	    } else {
		nowpix[0] = (int)diag.size();
		nowpix[1] = noww;
		nowpix[2] = nowh;
		nowpix[3] = diagidx[k - w];
		nowpix[4] = diagidx[k - 1];

		if(diag[nowpix[4]][0] >= 0) {
		  diag[nowpix[4]][5] = nowpix[0];
		}
		if(diag[nowpix[3]][0] >= 0) {
		    diag[nowpix[3]][6] = nowpix[0];
		}
	    }

	    diag.push_back(nowpix);
	    diagidx[k] = (int)diag.size() - 1;

	}
    }
    nreal = (int)diag.size();
}

void mult( const PoissonMatrix &T, const double *v, double *w ) {
    for(int i = 0; i < T.n; i++) {
	w[i] = v[i] * T.diag[i];
	for(int j = 0; j < 4; j++) {
	    if(T.pos[j][0] == 0) { // upper
		if(i < T.n - T.pos[j][1]) {
		    w[i] += v[i + T.pos[j][1]] * T.odiag[j][i];
		}
	    } else {	// lower
		if(i >= T.pos[j][0]) {
		    w[i] += v[i - T.pos[j][0]] * T.odiag[j][i]; 
		}
	    }
	}
    }
} 


void mult( const PoissonMaskMatrix &T, const double *v, double *w ) {
    for(int i = 0; i < T.nreal; i++) {
	if(T.diag[i][0] == -1) {	// not interior
	    w[i] = v[i];
	} else {
	    w[i] = v[i] * 4.f - (v[T.diag[i][3]] + v[T.diag[i][4]] + v[T.diag[i][5]] + v[T.diag[i][6]]);
	}
    }
}
/*
int linearsolve(int width, int height, double* b, double* x, double EPS) {
    // set A
    PoissonMatrix A(width, height);

    for(int i = 0; i < width * height; i++) {
	x[i] = 0.f;
    }


    // solve Ax = b
    if(bicgstab(A.n,A,b,x,EPS) < 0)
	return -1;

#ifdef DEBUG_POISSON
    for(int i = 0; i < width * height; i++) {
	printf("%lf\n", x[i]);
    }
#endif

    // return normally
    return 0;
}

int linearsolve(int width, int height, double* b, double* x, const ByteImage1& mask, double EPS) {
    // set A
    PoissonMaskMatrix A(width, height, mask);

    for(int i = 0; i < A.n; i++) {
	x[i] = 0;
    }

    // for real x
    double* xreal = new double[A.nreal];
    double* breal = new double[A.nreal];
    for(int i = 0; i < A.nreal; i++) {
	int nowidx = A.diag[i][1] + width * A.diag[i][2];
	breal[i] = b[nowidx];
    }

    if(bicgstab(A.nreal, A, breal, xreal, EPS) < 0) {
	return -1;
    }

    for(int i = 0; i < A.nreal; i++) {
	int nowidx = A.diag[i][1] + width * A.diag[i][2];
	// printf("%d\n", nowidx);
	x[nowidx] = xreal[i];
    }
    delete[] breal;
    delete[] xreal;

    return 0;
}

FloatImage3& Poisson::cloning(FloatImage3& F, FloatImage3& G, int x, int y) {
    // check index
    int w = G.width();
    int h = G.height();
    tarimg = F;
    if(x < 0 || x + w > (int)F.width() || y < 0 || y + h > (int)F.height()) {
	cout << "error: the index is out of bound" << endl;
	return tarimg;
    }

    // resize tarimg
    FloatImage3 srcarea(w, h);
    FloatImage3 tararea(w, h);

    // for each channel
    for(int c = 0; c < 3; c++) {
	for(int j = 0; j < h; j++) {
	    for(int i = 0; i < w; i++) {
		srcarea(i, j)[c] = F(x + i, y + j)[c];
		tararea(i, j)[c] = G(i, j)[c];
	    }
	}
    }

    // linear solve b, x
    double** solb = new double*[3];
    double** solx = new double*[3];
    for(int i = 0; i < 3; i++) {
	solb[i] = new double[w * h];
	solx[i] = new double[w * h];
    }

    // assign b, x
    int dx[] = {-1, 0, 0, 1};
    int dy[] = {0, -1, 1, 0};
    for(int j = 0, k = 0; j < h; j++) {
	for(int i = 0; i < w; i++, k++) {
	    // bound
	    bool bound_flag = (j == 0) || (j == h - 1) || (i == 0) || (i == w - 1);
	    // set x
	    for(int c = 0; c < 3; c++) solx[c][k] = 0.f;
	    // set b on bound
	    if(bound_flag) {
		for(int c = 0; c < 3; c++) solb[c][k] = (double)srcarea(i, j)[c];
		continue;
	    }
	    // set b on interior area
	    switch(gradient_flag) {
		// normal flag: apply G gradient
		case 0:
DEFAULT_FLAG:	//------default flag------
		    for(int c = 0; c < 3; c++) solb[c][k] = (double)tararea(i, j)[c] * 4.f;
		    for(int _d = 0; _d < 4; _d++) {
			for(int c = 0; c < 3; c++) solb[c][k] -= (double)tararea(i + dx[_d], j + dy[_d])[c];
		    }
		    break;
		case 1:
		    double srcval[3], tarval[3], srctotal, tartotal;
		    srctotal = 0.f;
		    tartotal = 0.f;
		    bool srcflag, tarflag;
		    srcflag = tarflag = true;
		    for(int c = 0; c < 3; c++) {
			srcval[c] = (double)srcarea(i, j)[c] * 4.f;
			tarval[c] = (double)tararea(i, j)[c] * 4.f;
			for(int _d = 0; _d < 4; _d++) {
			    srcval[c] -= (double)srcarea(i + dx[_d], j + dy[_d])[c];
			    tarval[c] -= (double)tararea(i + dx[_d], j + dy[_d])[c];
			}

			srcflag &= fabs(srcval[c] / 4.f) > gradient_thres;
			tarflag &= fabs(tarval[c] / 4.f) > gradient_thres;

			tartotal += tarval[c] * tarval[c];
			srctotal += srcval[c] * srcval[c];
		    }

		    if(srctotal > tartotal) for(int c = 0; c < 3; c++) solb[c][k] = (srcflag) ? srcval[c] : 0;
		    else for(int c = 0; c < 3; c++) solb[c][k] = (tarflag) ? tarval[c] : 0;
		    break;
		case 2:
		    double srcval2[3];
		    bool srcflag2;
		    srcflag2 = true;
		    for(int c = 0; c < 3; c++) {
			srcval2[c] = (double)srcarea(i, j)[c] * 4.f;
			for(int _d = 0; _d < 4; _d++) {
			    srcval2[c] -= (double)srcarea(i + dx[_d], j + dy[_d])[c];
			}

			srcflag2 &= fabs(srcval2[c] / 4.f) > gradient_thres;
		    }
		    for(int c = 0; c < 3; c++) solb[c][k] = (srcflag2) ? srcval2[c] : 0;
		    break;
		default:
		    goto DEFAULT_FLAG;
	    }
	}
    }

    for(int c = 0; c < 3; c++) {
	// solve Ax = b for x
	if(linearsolve(w, h, solb[c], solx[c], lsolve_eps) < 0) {
#ifdef DEBUG_LSOLVE
	    cout << "error: cant solve the system by bicgstab" << endl;
#endif
	}
	    

    	int k = 0;
	// clip and fill the value
	for(int j = 0; j < h; j++) {
	    for(int i = 0; i < w; i++) {
                double nowval =  solx[c][k++];
		Float1& nowpix = tarimg(x + i, y + j)[c];
		if(nowval < 0.f)
		    nowpix = 0.f;
		else if(nowval > 1.f)
		    nowpix = 1.f;
		else
		    nowpix = (Float1)nowval; 
	    }
	}
    }

    for(int c = 0; c < 3; c++) {
	delete[] solx[c];
	delete[] solb[c];
    }
    delete[] solx;
    delete[] solb;

    return tarimg;
}

FloatImage3& Poisson::cloning(FloatImage3& F, FloatImage3& G, const ByteImage1& mask, int x, int y) {
    // check index
    int w = G.width();
    int h = G.height();
    tarimg = F;
    if(x < 0 || x + w > (int)F.width() || y < 0 || y + h > (int)F.height()) {
	cout << "error: the index is out of bound" << endl;
	return tarimg;
    }

    // resize tarimg
    FloatImage3 srcarea(w, h);
    FloatImage3 tararea(w, h);

    // for each channel
    for(int c = 0; c < 3; c++) {
	for(int j = 0; j < h; j++) {
	    for(int i = 0; i < w; i++) {
		srcarea(i, j)[c] = F(x + i, y + j)[c];
		tararea(i, j)[c] = G(i, j)[c];
	    }
	}
    }

    // linear solve b, x
    double** solb = new double*[3];
    double** solx = new double*[3];
    for(int i = 0; i < 3; i++) {
	solb[i] = new double[w * h];
	solx[i] = new double[w * h];
    }

    // assign b, x
    int dx[] = {-1, 0, 0, 1};
    int dy[] = {0, -1, 1, 0};
    for(int j = 0, k = 0; j < h; j++) {
	for(int i = 0; i < w; i++, k++) {
	    // bound
	    bool bound_flag = (j == 0) || (j == h - 1) || (i == 0) || (i == w - 1) 
					|| (int)mask(i - 1, j)*(int)mask(i - 1, j)*(int)mask(i - 1, j)*(int)mask(i - 1, j) == 0;
	    // set x
	    for(int c = 0; c < 3; c++) solx[c][k] = 0.f;
	    // set b on bound
	    if(bound_flag) {
		for(int c = 0; c < 3; c++) solb[c][k] = (double)srcarea(i, j)[c];

		continue;
	    }

	    for(int c = 0; c < 3; c++) solb[c][k] = (double)tararea(i, j)[c] * 4.f;
	    for(int _d = 0; _d < 4; _d++) {
		for(int c = 0; c < 3; c++) solb[c][k] -= (double)tararea(i + dx[_d], j + dy[_d])[c];
	    }
	}
    }

    for(int c = 0; c < 3; c++) {
	// solve Ax = b for x
	if(linearsolve(w, h, solb[c], solx[c], mask, lsolve_eps) < 0) {
#ifdef DEBUG_LSOLVE
	    cout << "error: cant solve the system by bicgstab" << endl;
#endif
	}
	    

    	int k = 0;
	// clip and fill the value
	for(int j = 0; j < h; j++) {
	    for(int i = 0; i < w; i++) {
		double nowval =  solx[c][k++];
		Float1& nowpix = tarimg(x + i, y + j)[c];
		if(mask(i, j) == 0) {
		    nowpix = F(x + i, y + j)[c];
		}  else {
		    if(nowval < 0.f)
			nowpix = 0.f;
		    else if(nowval > 1.f)
			nowpix = 1.f;
		    else
			nowpix = (Float1)nowval; 
		}
	    }
	}
    }

    for(int c = 0; c < 3; c++) {
	delete[] solx[c];
	delete[] solb[c];
    }
    delete[] solx;
    delete[] solb;

    return tarimg;
}
*/

