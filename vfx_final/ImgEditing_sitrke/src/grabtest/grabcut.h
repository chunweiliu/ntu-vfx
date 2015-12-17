/*
 * GrabCut implementation source code Copyright(c) 2005-2006 Justin Talbot
 *
 * All Rights Reserved.
 * For educational use only; commercial use expressly forbidden.
 * NO WARRANTY, express or implied, for this software.
 */

#ifndef GRAB_CUT_H
#define GRAB_CUT_H

// #include "allinc.h"
#include "gmm.h"

#include <cmlib/utility/graphcut.h>
using namespace cmlib::utility::graphcut;
typedef Graph<double,double,double> GraphType;
	    
class GrabCut
{
    public:
	GrabCut( GmmImage<GmmColor> *image, int nc_fore = 5, int nc_back = 5, double smooth = 50.f);

	~GrabCut();

	// Initialize Trimap, inside rectangle is TrimapUnknown, outside is TrimapBackground
	void initialize(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

	// Edit Trimap
	void setTrimap(int x1, int y1, int x2, int y2, const TrimapValue& t);
	
	void fitGMMs();

	// Run Grabcut refinement on the hard segmentation
	void refine();
	int refineOnce();	// returns the number of pixels that have changed from foreground to background or vice versa

	void overlayAlpha();

	// get segmentation images
	GmmImage<SegmentationValue>* getsegimg() {
	    return m_hardSegmentation;
	}
/*
        void convertGmm2Cmlib_tarimg() {
            tarimg.resize(GmmTarimg.height(), GmmTarimg.width());
            for(int h = 0; h < GmmTarimg.height(); h++) {
                for(int w = 0; w < GmmTarimg.width(); w++) {
                    GmmColor& nowpix = GmmTarimg(w, h);
                    tarimg(w, h) = Float3(nowpix.r, nowpix.g, nowpix.b);
                }
            }
        }
        void convertCmlib2Gmm_tarimg() {
            GmmTarimg = GmmImage<GmmColor>(tarimg.width(), tarimg.height());
            for(int h = 0; h < GmmTarimg.height(); h++) {
                for(int w = 0; w < GmmTarimg.width(); w++) {
                    Float3& nowpix = tarimg(w, h);
                    GmmTarimg(x, y) = GmmColor(nowpix[0], nowpix[1], nowpix[2]);
                }
            }
        }
*/
private:

	unsigned int m_w, m_h;				// All the following Image<*> variables will be the same width and height.
										// Store them here so we don't have to keep asking for them.
	GmmImage<GmmColor> *m_image;
	GmmImage<TrimapValue> *m_trimap;
	GmmImage<unsigned int> *m_GMMcomponent;
	GmmImage<SegmentationValue> *m_hardSegmentation;

	GmmImage<Real> *m_softSegmentation;	// Not yet implemented (this would be interpreted as alpha)

	GMM *m_backgroundGMM, *m_foregroundGMM;

	int updateHardSegmentation();		// Update hard segmentation after running GraphCut, 
										// Returns the number of pixels that have changed from foreground to background or vice versa.

	// Variables used in formulas from the paper.
	Real m_lambda;		// lambda = 50. This value was suggested the GrabCut paper.
	Real m_beta;		// beta = 1 / ( 2 * average of the squared color distances between all pairs of neighboring pixels (8-neighborhood) )
	Real m_L;			// L = a large value to force a pixel to be foreground or background

	void computeBeta();
	void computeL();

	// Precomputed N-link weights
	GmmImage<NLinks> *m_NLinks;

	void computeNLinks();
	Real computeNLink(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

	// Graph for Graphcut
	GraphType *m_graph;
	GmmImage<GraphType::node_id> *m_nodes;

	void initGraph();	// builds the graph for GraphCut

	// GmmImages of various variables that can be displayed for debugging.
	GmmImage<Real> *m_NLinksImage;
	GmmImage<GmmColor> *m_TLinksImage;
	GmmImage<GmmColor> *m_GMMImage;
	GmmImage<Real> *m_AlphaImage;

	void buildImages();
};

#endif //GRAB_CUT_H
