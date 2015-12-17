function [ tb, eb ] = getbitmap( rgb )
%GETBITMAP Get the threshold bitmap and exclusion bitmap of image
%   INPUT:  RGB color image
%   OUTPUT: [threshold bitmap, exclusion bitmap]

gray = rgb2gray( rgb );
m = median( gray );
mm = median( m );

tb = gray > mm;
eb = ( (gray < (mm - 4)) | ((gray) > (mm + 4)) );
