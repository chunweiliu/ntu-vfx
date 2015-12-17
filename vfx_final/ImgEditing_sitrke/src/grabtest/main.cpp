
#include "grabcut.h"

#include <cmlib/image.h>
#include <cmlib/imageio/all.h>
#include <cmlib/imageio.h>

#include <iostream>

int main(int argc, char** argv)
{
    cmlib::image::FloatImage3 haha;
    bool flag = cmlib::image::read(haha, argv[1]);
    std::cout << flag << std::endl;

    GmmImage<GmmColor> image(haha.width(), haha.height());
    for(int j = 0; j < (int)haha.height(); j++) {
	for(int i = 0; i < (int)haha.width(); i++) {
	    image(i, j) = GmmColor(haha(i, j)[0], haha(i, j)[1], haha(i, j)[2]);
	}
    }

    GrabCut grabcut(&image, 3, 3, 50.f);
    grabcut.initialize(220, 20, 403, 580);
    grabcut.fitGMMs();
    grabcut.refine();
    GmmImage<SegmentationValue>* segimg = grabcut.getsegimg();

    cmlib::image::ByteImage1 maskimg(haha.width(), haha.height());
    for(int j = 0; j < (int)haha.height(); j++) {
	for(int i = 0; i < (int)haha.width(); i++) {
	    maskimg(i, j) = ((*segimg)(i, j) == SegmentationForeground) ? 255 : 0;
	}
    }

    cmlib::image::write(maskimg, "resultmask.jpg");
}
