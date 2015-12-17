function [ fp, des ] = mops( img, F_THRESHOLD )
% Multi-Scale Oriented Patches (MOPS) detector and descriptor
% Input:
%       img     n x m x 3   uint8   :a color image
% Output:
%       fp      k x 2       double  :k's feature points coordinate
%       des     k x 64      double  :k's feature descriptor with 64-dim
gimg            = rgb2gray( img );
[ fp, theta ]   = mops_detect( gimg,  F_THRESHOLD);
des             = mops_descript( fp, theta, gimg );
