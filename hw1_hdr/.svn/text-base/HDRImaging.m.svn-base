
function [artifact] = HDRImaging(imgdir, imgname_re, flagalign, flaghdr, flagtone)

% addpath
% addpath('./alignment');
% addpath('./tonemapping');

% load images
[img_series, sspeeds] = loadimg2(imgdir, imgname_re);

% alignment
if flagalign == 1
	[img_series] = get_shifted_imgs(img_series);
end

% HDR
if flaghdr == 1
	smoothness = 2;
	HDRimg = HDR(img_series, sspeeds, smoothness);
else
	artifact = [];
	return;
end

% flagtone
artifact = [];
if flagtone == 1
	alpha_in 	= 0.1;
	beta_in 	= 0.85;
	s_in		= 0.7;
	artifact 	= tonemap_G(HDRimg, alpha_in, beta_in, s_in);
elseif flagtone == 2
	artifact	= bimap(HDRimg);
end
