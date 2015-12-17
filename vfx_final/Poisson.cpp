#include <iomanip>
#include "Poisson.h"
#include "geometry.h"
#include <numeric>

using std::vector;

bool operator==(const CvPoint &p1, const CvPoint &p2) 
{ 
    return (p1.x == p2.x && p1.y == p2.y); 
}

void PoissonImg
(
    const IplImage* src, 
    const IplImage* dstMask, 
    const CvPoint& offset, 
    IplImage* dst
) 
{
    using namespace boost::numeric::ublas;

    // dest image
    int width = dstMask->width;
    int height = dstMask->height;

    // store the points to be processed
    std::vector<CvPoint> vInterior;
    std::vector<CvPoint> vBoundary;

    // classify pixels to _vInterior_ or _vBoundary_ based on mask ROI
    { /*{{{*/
	char *imgData = dstMask->imageData;

	for(int y = 0; y < height; y++) {
	    for(int x = 0; x < width; x++) {
		if(imgData[x] == 0) 
		    continue;

		// check whether it is on the boundary by searching its 8 neighbors
		bool boundary = false;

		for(int dy = -1; (dy <= 1 && !boundary); dy++) {
		    if(y+dy < 0 || y+dy >= height)
			continue;

		    for(int dx = -1; dx <= 1; dx++) {
			if(x+dx < 0 || x+dx >= width)
			    continue;

			char *tmp = imgData + dstMask->widthStep * dy;

			if(tmp[x + dx] == 0) {
			    boundary = true;
			    break;
			}
		    }
		}

		// store the point coordinate
		if(boundary)
		    vBoundary.push_back(cvPoint(x, y));
		else
		    vInterior.push_back(cvPoint(x, y));
	    } //for x

	    imgData += dstMask->widthStep;
	} //for y
    } /*}}}*/
    

    // fill the elements in the sparse matrix
    size_t nInterior = vInterior.size();
    SMMatrix A(nInterior, nInterior);
    DVector b[3];
    DVector solution[3];

    for(int i = 0; i < dst->nChannels; i++) {
	b[i] = DVector(nInterior);
	b[i].clear();
	solution[i] = DVector(nInterior);
	solution[i].clear();
    }
    
    // fill the elements in _A_ & _b_
    { /*{{{*/
	for(size_t row = 0; row < nInterior; row++) {
	    int Np = 0;
	    CvPoint pos = vInterior[row];

	    for(int i = 0; i < dst->nChannels; i++)
		b[i][row] = 0.;

	    // check the 4 neighbors
	    // note that in row-major sequence
	    int dx[4] = {0, -1, 1, 0};
	    int dy[4] = {-1, 0, 0, 1};

	    for(int d = 0; d < 4; d++) {
		int nx = pos.x + dx[d], ny = pos.y + dy[d];

		// pixel on the image border has truncated neighborhood
		const CvScalar color1 = cvGet2D(src, pos.y + offset.y, pos.x + offset.x);
		if(nx >= 0 && nx < width && ny >= 0 && ny < height) {
		    Np++;

		    // div(v)
		    CvScalar color2 = cvGet2D(src, ny + offset.y, nx + offset.x);
		    for(int c = 0; c < dst->nChannels; c++) {
			b[c][row] += color1.val[c] - color2.val[c];
		    }

		    std::vector<CvPoint>::iterator findResult;
		    findResult = std::find(vBoundary.begin(), vBoundary.end(), cvPoint(nx, ny));
		    if(findResult != vBoundary.end()) {
			CvScalar color = cvGet2D(dst, ny, nx);
			for(int i = 0; i < dst->nChannels; i++)
			    b[i][row] += color.val[i];
		    }
		}
		else {
		    continue;
		}

		std::vector<CvPoint>::iterator findResult;
		findResult = std::find(vInterior.begin(), vInterior.end(), cvPoint(nx, ny));
		if(findResult != vInterior.end())
		    A(row, findResult - vInterior.begin()) = -1.0;
	    } //for d

	    A(row, row) = (float)Np;

	    // initial guess
	    CvScalar color = cvGet2D(src, pos.y + offset.y, pos.x + offset.x);
	    for(int c = 0; c < dst->nChannels; c++)
		solution[c][row] = color.val[c];

	} //for row
    } /*}}}*/

    // solve the linear equation
    { /*{{{*/
	//clock_t t1 = clock();
	SCMatrix SA = A;
	for(int i = 0; i < dst->nChannels; i++)
	    SparseConjGrad(SA, b[i], solution[i], nInterior * 1.1);
	//clock_t t2 = clock();
	//cerr << "solve sparse conjugate-gradient takes " 
	//    << double(t2 - t1) / CLOCKS_PER_SEC << " seconds" << endl;
    } /*}}}*/

    // write to the result
    for(size_t i = 0; i < nInterior; i++) {
	CvScalar color;
	for(int j = 0; j < dst->nChannels; j++)
	    color.val[j] = solution[j][i];
	cvSet2D(dst, vInterior[i].y, vInterior[i].x, color);
    }
}

void deFace(IplImage* dst, const IplImage* dstMask) 
{
    using namespace boost::numeric::ublas;

    // dest image
    int width = dstMask->width;
    int height = dstMask->height;

    // store the points to be processed
    std::vector<CvPoint> vInterior;
    std::vector<CvPoint> vBoundary;

    // classify pixels to _vInterior_ or _vBoundary_ based on mask ROI
    { /*{{{*/
	//int minY = height, maxY = -1;
	//int minX = width, maxX = -1;
	char *imgData = dstMask->imageData;
	
	for(int y = 0; y < height; y++) {
	    for(int x = 0; x < width; x++) {
		if(imgData[x] == 0) 
		    continue;
#if 0
		if(x < minX)
		    minX = x;
		else if(x > maxX)
		    maxX = x;
		if(y < minY)
		    minY = y;
		else if(y > maxY)
		    maxY = y;
#endif
		// check if it is on the boundary by searching its 8 neighbors
		bool boundary = false;
		for(int dy = -1; (dy <= 1 && !boundary); dy++) {
		    if(y+dy < 0 || y+dy >= height)  
			continue;

		    for(int dx = -1; dx <= 1; dx++) {
			if(x+dx < 0 || x+dx >= width)
			    continue;

			char *tmp = imgData + dstMask->widthStep * dy;
			
			if(tmp[x + dx] == 0) {
			    boundary = true;
			    break;
			}
		    }
		}

		// store the point coordinate
		if(boundary)
		    vBoundary.push_back(cvPoint(x, y));
		else
		    vInterior.push_back(cvPoint(x, y));
	    } //for x

	    imgData += dstMask->widthStep;
	} //for y
    } /*}}}*/
    
    // fill the elements in the sparse matrix
    size_t nInterior = vInterior.size();
    size_t nBoundary = vBoundary.size();
    SMMatrix A(nInterior, nInterior);
    DVector b[3];
    DVector solution[3];
    for(int i = 0; i < dst->nChannels; i++) {
	b[i] = DVector(nInterior);
	b[i].clear();
	solution[i] = DVector(nInterior);
	solution[i].clear();
    }
    
    // fill the elements in _A_ & _b_
    { /*{{{*/
	for(size_t row = 0; row < nInterior; row++) {
	    int Np = 0;
	    CvPoint pos = vInterior[row];

	    for(int i = 0; i < dst->nChannels; i++)
		b[i][row] = 0.;

	    // check the 4 neighbors
	    // note that in row-major sequence
	    int dx[4] = {0, -1, 1, 0};
	    int dy[4] = {-1, 0, 0, 1};

	    for(int d = 0; d < 4; d++) {
		int nx = pos.x + dx[d], ny = pos.y + dy[d];

		// pixel on the image border has truncated neighborhood
		if(nx >= 0 && nx < width && ny >= 0 && ny < height) {
		    Np++;

		    // div(v)
		    std::vector<CvPoint>::iterator findResult;
		    findResult = std::find(vBoundary.begin(), vBoundary.end(), cvPoint(nx, ny));
		    if(findResult != vBoundary.end()) {
			CvScalar color = cvGet2D(dst, ny, nx);
			for(int i = 0; i < dst->nChannels; i++)
			    b[i][row] += color.val[i];
		    }
		}
		else {
		    continue;
		}

		std::vector<CvPoint>::iterator findResult;
		findResult = std::find(vInterior.begin(), vInterior.end(), cvPoint(nx, ny));
		if(findResult != vInterior.end())
		    A(row, findResult-vInterior.begin()) = -1.0;
	    } //for d

	    A(row, row) = (float)Np;

	} //for row
    } /*}}}*/

    // initial guess
    { /*{{{*/
	CvScalar skinColor = cvScalarAll(0), sample;
	for(size_t i = 0; i < nBoundary; i++) {
	    CvPoint p = vBoundary[i];
	    
	    sample = cvGet2D(dst, p.y, p.x);
	    for(int j = 0; j < dst->nChannels; j++)
		skinColor.val[j] += sample.val[j];
	}
	for(int j = 0; j < dst->nChannels; j++)
	    skinColor.val[j] /= (double)nBoundary;
	for(size_t i = 0; i < nInterior; i++)
	    for(int j = 0; j < dst->nChannels; j++)
		solution[j][i] = skinColor.val[j];
    } /*}}}*/
    
    // solve the linear system
    { /*{{{*/
	//clock_t t1 = clock();
	SCMatrix SA = A;
	
	for(int i = 0; i < dst->nChannels; i++)
	    SparseConjGrad(SA, b[i], solution[i], 150);
	//clock_t t2 = clock();
	//cerr << "solve sparse conjugate-gradient takes " 
	//    << std::setprecision(2) << double(t2 - t1) / CLOCKS_PER_SEC 
	//    << " seconds" << endl;
    } /*}}}*/

    // write to the result
    for(size_t i = 0; i < nInterior; i++) {
	CvScalar color;
	for(int j = 0; j < dst->nChannels; j++)
	    color.val[j] = solution[j][i];
	cvSet2D(dst, vInterior[i].y, vInterior[i].x, color);
    }
}

void mixPoissonImg
(
    const IplImage* src, 
    const IplImage* dstMask, 
    const CvPoint& offset, 
    IplImage* dst
) 
{
    using namespace boost::numeric::ublas;

    // dest image
    int width = dstMask->width;
    int height = dstMask->height;

    // store the points to be processed
    std::vector<CvPoint> vInterior;
    std::vector<CvPoint> vBoundary;

    // classify pixels to _vInterior_ or _vBoundary_ based on mask ROI
    { /*{{{*/
	char *imgData = dstMask->imageData;

	for(int y = 0; y < height; y++) {
	    for(int x = 0; x < width; x++) {
		if(imgData[x] == 0) 
		    continue;

		// check whether it is on the boundary by searching its 8 neighbors
		bool boundary = false;

		for(int dy = -1; (dy <= 1 && !boundary); dy++) {
		    if(y+dy < 0 || y+dy >= height)
			continue;

		    for(int dx = -1; dx <= 1; dx++) {
			if(x+dx < 0 || x+dx >= width)
			    continue;

			char *tmp = imgData + dstMask->widthStep * dy;

			if(tmp[x + dx] == 0) {
			    boundary = true;
			    break;
			}
		    }
		}

		// store the point coordinate
		if(boundary)
		    vBoundary.push_back(cvPoint(x, y));
		else
		    vInterior.push_back(cvPoint(x, y));
	    } //for x

	    imgData += dstMask->widthStep;
	} //for y
    } /*}}}*/
    

    // fill the elements in the sparse matrix
    size_t nInterior = vInterior.size();
    SMMatrix A(nInterior, nInterior);
    DVector b[3];
    DVector solution[3];

    for(int i = 0; i < dst->nChannels; i++) {
	b[i] = DVector(nInterior);
	b[i].clear();
	solution[i] = DVector(nInterior);
	solution[i].clear();
    }
    
    // fill the elements in _A_ & _b_
    { /*{{{*/
	for(size_t row = 0; row < nInterior; row++) {
	    int Np = 0;
	    CvPoint pos = vInterior[row];

	    for(int i = 0; i < dst->nChannels; i++)
		b[i][row] = 0.;

	    // check the 4 neighbors
	    // note that in row-major sequence
	    int dx[4] = {0, -1, 1, 0};
	    int dy[4] = {-1, 0, 0, 1};

	    for(int d = 0; d < 4; d++) {
		int nx = pos.x + dx[d], ny = pos.y + dy[d];

		// pixel on the image border has truncated neighborhood
		const CvScalar color01 = cvGet2D(src, pos.y + offset.y, pos.x + offset.x);
		const CvScalar color11 = cvGet2D(dst, pos.y, pos.x);
		if(nx >= 0 && nx < width && ny >= 0 && ny < height) {
		    Np++;

		    // div(v)
		    CvScalar color02 = cvGet2D(src, ny + offset.y, nx + offset.x);
		    CvScalar color12 = cvGet2D(dst, ny, nx);
		    for(int c = 0; c < dst->nChannels; c++) {
			float f = color01.val[c] - color02.val[c];
			float g = color11.val[c] - color12.val[c];
			
			b[c][row] += (fabs(f) > fabs(g)) ? f : g;
		    }

		    std::vector<CvPoint>::iterator findResult;
		    findResult = std::find(vBoundary.begin(), vBoundary.end(), cvPoint(nx, ny));
		    if(findResult != vBoundary.end()) {
			CvScalar color = cvGet2D(dst, ny, nx);
			for(int c = 0; c < dst->nChannels; c++)
			    b[c][row] += color.val[c];
		    }
		}
		else {
		    continue;
		}

		std::vector<CvPoint>::iterator findResult;
		findResult = std::find(vInterior.begin(), vInterior.end(), cvPoint(nx, ny));
		if(findResult != vInterior.end())
		    A(row, findResult - vInterior.begin()) = -1.0;
	    } //for d

	    A(row, row) = (float)Np;

	    // initial guess
	    CvScalar color = cvGet2D(src, pos.y + offset.y, pos.x + offset.x);
	    for(int c = 0; c < dst->nChannels; c++)
		solution[c][row] = color.val[c];

	} //for row
    } /*}}}*/

    // solve the linear equation
    { /*{{{*/
	//clock_t t1 = clock();
	SCMatrix SA = A;
	for(int i = 0; i < dst->nChannels; i++)
	    SparseConjGrad(SA, b[i], solution[i], 0.5 * nInterior);
	//clock_t t2 = clock();
	//cerr << "solve sparse conjugate-gradient takes " 
	//    << double(t2 - t1) / CLOCKS_PER_SEC << " seconds" << endl;
    } /*}}}*/

    // write to the result
    for(size_t i = 0; i < nInterior; i++) {
	CvScalar color;
	for(int j = 0; j < dst->nChannels; j++)
	    color.val[j] = std::min(std::max(solution[j][i], 0.f), 255.f);
	cvSet2D(dst, vInterior[i].y, vInterior[i].x, color);
    }
}

#if 0
/*{{{*/
struct Point3 {
    Point3(float _x, float _y, float _t) 
	: x(_x), y(_y), t(_t) 
    { 
	// empty
    }

    bool operator==(const Point3& p)
    {
	return ((p.x == x) && (p.y == y) && (p.t == t));
    }
    float x, y, t; 
};

void deFaceVideo(Video& dst, const Video& dstMask)
{
    using namespace boost::numeric::ublas;
    
    // deFace of the first & last frame
    deFace(dst[0], dstMask[0]);
    deFace(dst[dst.num_frame() - 1], dstMask[dstMask.num_frame() - 1]);

    // dest image
    const int width = dstMask.num_width();
    const int height = dstMask.num_height();
    const int channel = dstMask.num_channel();
    const int frame = dstMask.num_frame();

    // store the points to be processed
    std::vector<Point3> vInterior;
    std::vector<Point3> vBoundary;
    std::vector<size_t> vAccumBoundary(frame, 0);
    std::vector<size_t> vAccumInterior(frame, 0);

    // classify pixels to _vInterior_ or _vBoundary_ based on mask ROI
    { /*{{{*/
	// the first frame: all pixels that its neighbor in second frame 
	// is labeled nonzero are classified as boundary
	const char *imgData = dstMask[1]->imageData;
	for(int y = 0; y < height; y++, imgData += dstMask[1]->widthStep) {
	    for(int x = 0; x < width; x++) {
		if(imgData[x] == 0)
		    continue;
		else
		    vBoundary.push_back(Point3(x, y, 0));
	    }
	}
	vAccumBoundary[0] = vBoundary.size();
	//cerr << "vAccumBoundary[0] = " << vAccumBoundary[0] << endl;

	for(size_t t = 1; t < dstMask.num_frame() - 1; t++) {
	    const char* currImgData = dstMask[t]->imageData;

	    for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
		    // early reject if current pixel is not labeled to be solved
		    if(currImgData[x] == 0) 
			continue;
		    
		    // check if it is on the boundary by searching its 26 neighbors
		    bool boundary = false;
		    for(int dt = -1; dt <= 1; dt++) {
			imgData = dstMask[t + dt]->imageData + y * dstMask[t + dt]->widthStep;

			for(int dy = -1; dy <= 1; dy++) {
			    if(y + dy < 0 || y + dy >= height)  
				continue;

			    for(int dx = -1; dx <= 1; dx++) {
				if(x + dx < 0 || x + dx >= width)
				    continue;

				const char* tmp = imgData + dstMask[t + dt]->widthStep * dy;

				if(tmp[x + dx] == 0) {
				    boundary = true;
				    goto classify;
				}
			    }
			}
		    }
		    classify:

		    // store the point coordinate
		    if(boundary)
			vBoundary.push_back(Point3(x, y, t));
		    else
			vInterior.push_back(Point3(x, y, t));
		} //for x

		currImgData += dstMask[t]->widthStep;
	    } //for y

	    vAccumBoundary[t] = vBoundary.size();
	    //cerr << "vAccumBoundary[" << t << "] = " << vAccumBoundary[t] << endl;
	    vAccumInterior[t] = vInterior.size();
	} //for t
	
	// the first frame: all pixels that its neighbor in second frame 
	// is labeled nonzero are classified as boundary
	imgData = dstMask[frame - 2]->imageData;
	for(int y = 0; y < height; y++, imgData += dstMask[frame - 2]->widthStep) {
	    for(int x = 0; x < width; x++) {
		if(imgData[x] == 0)
		    continue;
		else
		    vBoundary.push_back(Point3(x, y, frame - 1));
	    }
	}
	vAccumBoundary[frame - 1] = vBoundary.size();
	//cerr << "vAccumBoundary[" << frame - 1 << "] = " << vAccumBoundary[frame - 1] << endl;
	vAccumInterior[frame - 1] = vInterior.size();
    } /*}}}*/
#if 0
for(size_t i = 0; i < vBoundary.size(); i++) {
    Point3& pos = vBoundary[i];
    const CvScalar BLACK = cvScalarAll(0);
    cvSet2D(dst[pos.t], pos.y, pos.x, BLACK);
}
for(size_t i = 0; i < vInterior.size(); i++) {
    Point3& pos = vInterior[i];
    const CvScalar WHITE = cvScalarAll(255);
    cvSet2D(dst[pos.t], pos.y, pos.x, WHITE);
}
return;
#endif
    // fill the elements in the sparse matrix
    size_t nInterior = vInterior.size();
    size_t nBoundary = vBoundary.size();
    SMMatrix A(nInterior, nInterior);
    DVector b[3];
    DVector solution[3];
    for(int i = 0; i < channel; i++) {
	b[i] = DVector(nInterior);
	b[i].clear();
	solution[i] = DVector(nInterior);
	solution[i].clear();
    }
    
    // fill the elements in _A_ & _b_
    { /*{{{*/
	for(size_t row = 0; row < nInterior; row++) {
	    int Np = 2; // previous & next frame must be adjacent in certain
	    Point3 pos = vInterior[row];

	    for(int i = 0; i < channel; i++)
		b[i][row] = 0.;

	    // check the 6 neighbors
	    // note that in row-major sequence
	    const int dx[6] = {0, 0, -1, 1, 0, 0};
	    const int dy[6] = {0, -1, 0, 0, 1, 0};
	    const int dt[6] = {-1, 0, 0, 0, 0, 1};

	    for(int d = 0; d < 6; d++) {
		int nx = pos.x + dx[d], ny = pos.y + dy[d], nt = pos.t + dt[d];

		// pixel on the image border has truncated neighborhood
		if(nx >= 0 && nx < width && ny >= 0 && ny < height) {
		    Np++;

		    // div(v)
		    std::vector<Point3>::iterator findResult;
		    std::vector<Point3>::iterator BEGIN = (nt == 0) 
			? vBoundary.begin() 
			: vBoundary.begin() + vAccumBoundary[nt - 1];
		    std::vector<Point3>::iterator END = vBoundary.begin() + vAccumBoundary[nt];
		    findResult = std::find(BEGIN, END, Point3(nx, ny, nt));

		    if(findResult != END) {
			CvScalar color = cvGet2D(dst[nt], ny, nx);
			for(int i = 0; i < channel; i++)
			    b[i][row] += color.val[i];
		    }
		}
		else {
		    continue;
		}

		std::vector<Point3>::iterator findResult;
		std::vector<Point3>::iterator BEGIN = (nt == 0) 
		    ? vInterior.begin() 
		    : vInterior.begin() + vAccumInterior[nt - 1];
		std::vector<Point3>::iterator END = vInterior.begin() + vAccumInterior[nt];
		findResult = std::find(BEGIN, END, Point3(nx, ny, nt));
		if(findResult != END)
		    A(row, findResult - vInterior.begin()) = -1.0;
	    } //for d

	    A(row, row) = float(Np);

	} //for row
    } /*}}}*/

    // initial guess
    { /*{{{*/
	CvScalar skinColor = cvScalarAll(0), sample;
	for(size_t i = 0; i < nBoundary; i++) {
	    Point3 p = vBoundary[i];
	    
	    sample = cvGet2D(dst[p.t], p.y, p.x);
	    for(int j = 0; j < channel; j++)
		skinColor.val[j] += sample.val[j];
	}
	for(int j = 0; j < channel; j++)
	    skinColor.val[j] /= double(nBoundary);
	for(size_t i = 0; i < nInterior; i++)
	    for(int j = 0; j < channel; j++)
		solution[j][i] = skinColor.val[j];
    } /*}}}*/
#if 0 
    // solve the linear system
    { /*{{{*/
	SCMatrix SA = A;
	
	for(int i = 0; i < channel; i++)
	    SparseConjGrad(SA, b[i], solution[i], 150);
    } /*}}}*/
#endif
    // write to the result
    for(size_t i = 0; i < nInterior; i++) {
	CvScalar color;
	for(int j = 0; j < channel; j++)
	    color.val[j] = solution[j][i];
	cvSet2D(dst[vInterior[i].t], vInterior[i].y, vInterior[i].x, color);
    }
}

void mixPoissonVideo
(
    const Video& src, 
    const Video& dstMask,
    const vector<CvPoint>& offset,
    Video& dst
)
{
    using namespace boost::numeric::ublas;
    
    // dest image
    const int width = dstMask.num_width();
    const int height = dstMask.num_height();
    const int channel = dstMask.num_channel();
    const int frame = dstMask.num_frame();
    
    // deFace of the first & last frame
    mixPoissonImg(src[0], dstMask[0], offset[0], dst[0]);
    mixPoissonImg(src[frame - 1], dstMask[frame - 1], offset[frame - 1], dst[frame - 1]);

    // store the points to be processed
    std::vector<Point3> vInterior;
    std::vector<Point3> vBoundary;
    std::vector<size_t> vAccumBoundary(frame, 0);
    std::vector<size_t> vAccumInterior(frame, 0);

    // classify pixels to _vInterior_ or _vBoundary_ based on mask ROI
    { /*{{{*/
	// the first frame: all pixels that its neighbor in second frame 
	// is labeled nonzero are classified as boundary
	const char *imgData = dstMask[1]->imageData;
	for(int y = 0; y < height; y++, imgData += dstMask[1]->widthStep) {
	    for(int x = 0; x < width; x++) {
		if(imgData[x] == 0)
		    continue;
		else
		    vBoundary.push_back(Point3(x, y, 0));
	    }
	}
	vAccumBoundary[0] = vBoundary.size();

	for(size_t t = 1; t < dstMask.num_frame() - 1; t++) {
	    const char* currImgData = dstMask[t]->imageData;

	    for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
		    // early reject if current pixel is not labeled to be solved
		    if(currImgData[x] == 0) 
			continue;
		    
		    // check if it is on the boundary by searching its 26 neighbors
		    bool boundary = false;
		    for(int dt = -1; dt <= 1; dt++) {
			imgData = dstMask[t + dt]->imageData + y * dstMask[t + dt]->widthStep;

			for(int dy = -1; dy <= 1; dy++) {
			    if(y + dy < 0 || y + dy >= height)  
				continue;

			    for(int dx = -1; dx <= 1; dx++) {
				if(x + dx < 0 || x + dx >= width)
				    continue;

				const char* tmp = imgData + dstMask[t + dt]->widthStep * dy;

				if(tmp[x + dx] == 0) {
				    boundary = true;
				    goto classify;
				}
			    }
			}
		    }
		    classify:

		    // store the point coordinate
		    if(boundary)
			vBoundary.push_back(Point3(x, y, t));
		    else
			vInterior.push_back(Point3(x, y, t));
		} //for x

		currImgData += dstMask[t]->widthStep;
	    } //for y

	    vAccumBoundary[t] = vBoundary.size();
	    vAccumInterior[t] = vInterior.size();
	} //for t
	
	// the first frame: all pixels that its neighbor in second frame 
	// is labeled nonzero are classified as boundary
	imgData = dstMask[dstMask.num_frame() - 1]->imageData;
	for(int y = 0; y < height; y++, imgData += dstMask[dstMask.num_frame() - 1]->widthStep) {
	    for(int x = 0; x < width; x++) {
		if(imgData[x] == 0)
		    continue;
		else
		    vBoundary.push_back(Point3(x, y, frame - 1));
	    }
	}
	vAccumBoundary[frame - 1] = vBoundary.size();
	vAccumInterior[frame - 1] = vInterior.size();
    } /*}}}*/
    
    // fill the elements in the sparse matrix
    size_t nInterior = vInterior.size();
    size_t nBoundary = vBoundary.size();
    SMMatrix A(nInterior, nInterior);
    DVector b[3];
    DVector solution[3];
    for(int i = 0; i < channel; i++) {
	b[i] = DVector(nInterior);
	b[i].clear();
	solution[i] = DVector(nInterior);
	solution[i].clear();
    }
    
    // fill the elements in _A_ & _b_
    { /*{{{*/
	for(size_t row = 0; row < nInterior; row++) {
	    int Np = 2; // previous & next frame must be adjacent in certain
	    Point3 pos = vInterior[row];

	    for(int i = 0; i < channel; i++)
		b[i][row] = 0.;

	    // check the 6 neighbors
	    // note that in row-major sequence
	    const int dx[6] = {0, 0, -1, 1, 0, 0};
	    const int dy[6] = {0, -1, 0, 0, 1, 0};
	    const int dt[6] = {-1, 0, 0, 0, 0, 1};

	    for(int d = 0; d < 6; d++) {
		int nx = pos.x + dx[d], ny = pos.y + dy[d], nt = pos.t + dt[d];

		// pixel on the image border has truncated neighborhood
		if(nx >= 0 && nx < width && ny >= 0 && ny < height) {
		    Np++;

		    // div(v)
		    std::vector<Point3>::iterator findResult;
		    std::vector<Point3>::iterator BEGIN = (nt == 0) 
			? vBoundary.begin() 
			: vBoundary.begin() + vAccumBoundary[nt - 1];
		    std::vector<Point3>::iterator END = vBoundary.begin() + vAccumBoundary[nt];
		    findResult = std::find(BEGIN, END, Point3(nx, ny, nt));

		    if(findResult != END) {
			CvScalar color = cvGet2D(dst[nt], ny, nx);
			for(int i = 0; i < channel; i++)
			    b[i][row] += color.val[i];
		    }
		}
		else {
		    continue;
		}

		std::vector<Point3>::iterator findResult;
		std::vector<Point3>::iterator BEGIN = (nt == 0) 
		    ? vInterior.begin() 
		    : vInterior.begin() + vAccumInterior[nt - 1];
		std::vector<Point3>::iterator END = vInterior.begin() + vAccumInterior[nt];
		findResult = std::find(BEGIN, END, Point3(nx, ny, nt));
		if(findResult != END)
		    A(row, findResult - vInterior.begin()) = -1.0;
	    } //for d

	    A(row, row) = float(Np);

	} //for row
    } /*}}}*/

    // initial guess
    { /*{{{*/
	CvScalar skinColor = cvScalarAll(0), sample;
	for(size_t i = 0; i < nBoundary; i++) {
	    Point3 p = vBoundary[i];
	    
	    sample = cvGet2D(dst[p.t], p.y, p.x);
	    for(int j = 0; j < channel; j++)
		skinColor.val[j] += sample.val[j];
	}
	for(int j = 0; j < channel; j++)
	    skinColor.val[j] /= double(nBoundary);
	for(size_t i = 0; i < nInterior; i++)
	    for(int j = 0; j < channel; j++)
		solution[j][i] = skinColor.val[j];
    } /*}}}*/
    
    // solve the linear system
    { /*{{{*/
	SCMatrix SA = A;
	
	for(int i = 0; i < channel; i++)
	    SparseConjGrad(SA, b[i], solution[i], 150);
    } /*}}}*/

    // write to the result
    for(size_t i = 0; i < nInterior; i++) {
	CvScalar color;
	for(int j = 0; j < channel; j++)
	    color.val[j] = solution[j][i];
	cvSet2D(dst[vInterior[i].t], vInterior[i].y, vInterior[i].x, color);
    }
}
/*}}}*/
#endif

void MVCPoisson
(
    const IplImage* src, 
    const IplImage* dstMask, 
    const CvPoint& offset, 
    IplImage* dst
)
{
    // basic image information
    size_t width = dstMask->width;
    size_t height = dstMask->height;
    size_t channel = dst->nChannels;

    // store the points to be processed
    vector<CvPoint> vInterior;
    vector<CvPoint> vBoundary;

    // find contours
    IplImage* contourMap = cvCloneImage(dstMask);
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contour = NULL;
    cvFindContours(
	contourMap, 
	storage, 
	&contour, 
	sizeof(CvContour), 
	CV_RETR_EXTERNAL, 
	CV_CHAIN_APPROX_SIMPLE
    );
    cvZero(contourMap);
    cvDrawContours(contourMap, contour, cvScalarAll(255), cvScalarAll(255), -1);
   
    // store boundary pixels in sequence
    { /*{{{*/
	vBoundary.push_back(*( (CvPoint *)cvGetSeqElem(contour, 0) ));
	for(int i = 0; contour != NULL && i < contour->total; i++) {
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

		vBoundary.push_back(cvPoint(x, y));
		CV_NEXT_LINE_POINT(iterator);
	    }
	}

	// the first point is already in the vector
	// pop the last element for duplication
	vBoundary.pop_back();
    } /*}}}*/

    // store interior pixels
    IplImage* interiorMap = cvCreateImage(cvGetSize(dstMask), 8, 1);
    cvXor(contourMap, dstMask, interiorMap);
    const char* data = interiorMap->imageData;
    for(size_t y = 0; y < height; y++) {
	for(size_t x = 0; x < width; x++) {
	    if(data[x] != 0)
		vInterior.push_back(cvPoint(x, y));
	}
	data += interiorMap->widthStep;
    }

    std::vector<double> vLambda(vBoundary.size());
    std::vector<CvScalar> vDiff(vBoundary.size());

    // compute the difference along the boundary
    for(size_t i = 0; i < vBoundary.size(); i++) {
	// TODO
	// ensure point(vBoundary[i] + offset) is within the _src_
	CvScalar colorSrc = cvGet2D(src, vBoundary[i].y + offset.y, vBoundary[i].x + offset.x);
	CvScalar colorDst = cvGet2D(dst, vBoundary[i].y, vBoundary[i].x);
	for(size_t c = 0; c < channel; c++) 
	    vDiff[i].val[c] = colorDst.val[c] - colorSrc.val[c];
    }

    // evaluate the mean-value interpolation
    { /*{{{*/
	for(size_t i = 0; i < vInterior.size(); i++) {
	    CvPoint& px = vInterior[i];

	    // compute _vLambda_
	    { /*{{{*/
		for(size_t j = 0; j < vBoundary.size(); j++) {
		    CvPoint& currp = vBoundary[j];
		    CvPoint& nextp = (j == vBoundary.size() - 1) ? vBoundary[0] : vBoundary[j + 1];
		    CvPoint& prevp = (j == 0) ? vBoundary[vBoundary.size() - 1] : vBoundary[j - 1];
		    Vector v1(px.x - currp.x, px.y - currp.y);
		    Vector v2(px.x - nextp.x, px.y - nextp.y); 
		    Vector v3(px.x - prevp.x, px.y - prevp.y);
		    float L1 = v1.Length();

		    float currCos = INNER_PROD(v1, v2) / (L1 * v2.Length());
		    float prevCos = INNER_PROD(v1, v3) / (L1 * v3.Length());
		    // tan(x/2) = \frac{1 - cos(x)}{sin(x)} = \sqrt{\frac{1 - cos(x)}{1 + cos(x)}}
		    float currTan = sqrt((1. - currCos) / (1. + currCos));
		    float prevTan = sqrt((1. - prevCos) / (1. + prevCos));
		    if(isnan(currTan)) currTan = 0.;
		    if(isnan(prevTan)) prevTan = 0.;

		    vLambda[j] = (currTan + prevTan) / L1;

		    // some numerical rounoff error could lead to nan
		    if(isnan(vLambda[j]))
			vLambda[j] = 0.;
		}
	    } /*}}}*/

	    double sum = std::accumulate(vLambda.begin(), vLambda.end(), 0.);
	    double invsum = 1. / sum;

	    // interpolate membrane
	    CvScalar color = cvScalarAll(0.);
	    for(size_t j = 0; j < vBoundary.size(); j++) {
		vLambda[j] *= invsum;

		for(size_t c = 0; c < channel; c++)
		    color.val[c] += vLambda[j] * vDiff[j].val[c];
	    }

	    // add to source patch
	    CvScalar source = cvGet2D(src, vInterior[i].y + offset.y, vInterior[i].x + offset.x);
	    std::transform(color.val, color.val + channel, source.val, color.val, std::plus<double>());
	    cvSet2D(dst, vInterior[i].y, vInterior[i].x, color);
	}
    } /*}}}*/

    // deallocate memory
    cvReleaseImage(&contourMap);
    cvReleaseImage(&interiorMap);
}

