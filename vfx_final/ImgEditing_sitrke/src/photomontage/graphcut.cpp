

#include "graphcut.h"

// maximum likelihood
FloatImage3& GraphCut::maxlike(FloatImage3& srcimg1, FloatImage3& srcimg2) {
    // heigth and width of image
    int height = srcimg1.height();
    int width = srcimg1.width();
    tarimg.resize(width, height);

    // check if same size, error if not
    if((int)srcimg2.width() != width || (int)srcimg2.height() != height) {
	cout << "error in  GraphCut::maxlike: size of input are not match" << endl;
	return tarimg;
    }

    // allocate graph for graphcut
    GraphType *g = new GraphType(width*height, 8*width*height);
    for(int h = 0; h < height; h++) {
	for(int w = 0; w < width; w++) {
	    g->add_node();
	}
    }

    // Histogram
    int hist[3][20];
    for(int c = 0; c < 3; c++) {
	for(int b = 0; b < 19; b++) {
	    hist[c][b] = 0;
	}

	for(int h = 0; h < height; h++) {
	    for(int w = 0; w < width; w++) {
		float color;
		int bin;
		color = srcimg1(w, h)[c];
                bin = (int)round(color * 20);
		if(bin >= 20) bin = 19;
		else if(bin < 0) bin = 0;
		hist[c][bin]++;

		color = srcimg2(w, h)[c];
                bin = (int)round(color * 20);
		if(bin >= 20) bin = 19;
		else if(bin < 0) bin = 0;
		hist[c][bin]++;
	    }
	}
    }

    // assign data term and smooth term
    for(int h = 0; h < height; h++) {
	for(int w = 0; w < width; w++) {
	    int index = h * width + w;
            int binv1 = 0, binv2 = 0;
	    for(int c = 0; c < 3; c++) {
                int bin1 = (int)round(srcimg1(w, h)[c] * 20);
		if(bin1 >= 20) bin1 = 19;
		else if(bin1 < 0) bin1 = 0;
		binv1 += hist[c][bin1];

                int bin2 = (int)round(srcimg2(w, h)[c] * 20);
		if(bin2 >= 20) bin2 = 19;
		else if(bin2 < 0) bin2 = 0;
		binv2 += hist[c][bin2];
	    }

	    g->add_tweights(index, (float)binv1 / (width * height * 2.f) , (float)binv2 / (width * height * 2.f));

	    for(int dh = -neirange; dh <= neirange; dh++) {
		int nowh = h + dh;
		if(nowh < 0 || nowh >= height) continue;
		for(int dw = -neirange; dw <= neirange; dw++) {
		    int noww = w + dw;
		    if(noww < 0 || noww >= width) continue;

		    int neighbor_index = nowh * width + noww;
		    if(neighbor_index <= index) continue;

		    float value1 = 0.f, value2 = 0.f;
		    for(int c = 0; c < 3; c++) {
			value1 += pow(srcimg1(w, h)[c] - srcimg2(w, h)[c], 2);
			value2 += pow(srcimg1(noww, nowh)[c] - srcimg2(noww, nowh)[c], 2);
		    }
		    float value = smdegree * (value1 + value2);
		    g->add_edge(index, neighbor_index, value, value);	    
		}
	    }
	}
    }

    g->maxflow();
    for(int h = 0; h < height; h++) {
	for(int w = 0; w < width; w++) {
	    int index = h * width + w;
	    tarimg(w, h) = (g->what_segment(index) == GraphType::SOURCE) ? srcimg1(w, h) : srcimg2(w, h);
	}
    }

    delete g;
    return tarimg;
}

