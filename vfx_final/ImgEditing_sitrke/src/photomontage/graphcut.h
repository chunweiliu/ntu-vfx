
#ifndef _GRAPHCUT_
#define _GRAPHCUT_

#include "allinc.h"

#include <cmlib/utility/graphcut.h>
using namespace cmlib::utility::graphcut;
typedef Graph<double, double, double> GraphType;


// Graph Cut Class
class GraphCut {
    public:
	// constructor
        GraphCut(int range, float smoothdegree = 1.f) {
	    neirange = range;
	    smdegree = smoothdegree;
	}

	// method of maximum likelihood for two images
	FloatImage3& maxlike(FloatImage3& srcimg1, FloatImage3& srcimg2);

    private:
	FloatImage3 tarimg;
	int neirange;
	float smdegree;
};

#endif
