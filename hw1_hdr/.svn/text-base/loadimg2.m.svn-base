
function [img_cell sspeed] = loadimg2(dirstr, name_re)
%
% function [img_cell] = loadimg2(dirstr, n_img)
% loadimg2: 
%	load all image and shutter speed with regular expression in specified directory
% input:
%	dirstr:		string		directory path
%	name_re:	string		image name re
% output:
%	img_cell	1 * ncells	cell of images
%	sspeed		1 * n		corresponding shutter speed
%



imgstr 		= dir([dirstr '/' name_re]);
n_img  		= size(imgstr, 1);
img_cell 	= cell(1, n_img);
sspeed		= zeros(1, n_img);

for i = 1 : n_img
	nowstr 				= [dirstr '/' imgstr(i).name];
	img_cell{1, i} 		= imread(nowstr);
	now_exif 			= exifread(nowstr);
	sspeed(i) 			= now_exif.ExposureTime;
end

