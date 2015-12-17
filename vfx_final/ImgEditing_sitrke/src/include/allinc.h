
#ifndef _ALLINC_
#define _ALLINC_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <vector>
#include <iostream>
#include <string>
using namespace std;

#include <cmlib/image.h>
#include <cmlib/imageio/all.h>
#include <cmlib/imageio.h>
using namespace cmlib::image;

// judging whether the value is in the range [l, u)
template<class T>
int valwhere(T val, T upperbound, T lowerbound = (T)0) {
    if(val >= upperbound)
	return 1;
    else if(val < lowerbound) 
	return -1;
    else
	return 0;
}

#endif
