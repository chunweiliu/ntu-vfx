
function [toneimg] = tonemap_G(HDRimg, alpha_in, beta_in, s_in, showflag)
%
% function [toneimg] = tonemap_G(HDRimg, alpha_in, beta_in, s_in, showflag)
% tonemap_G:
%	tone mapping on gradient domain 
% input:
% 	HDRimg		H*W*3	image with exposure after HDR
%	alpha_in	1 * 1	average weight, default 0.1
%	beta_in		1 * 1	attenuation level, default 0.8
%	s_in		1 * 1	color information level, default 0.5
%	showflag	1 * 1	flag of show or not, default not to show
% output:
%	toneimg		H*W*3	image after tone mapping
%


[height, width, channel] = size(HDRimg);

if nargin < 5
	showflag = 0;
end
if nargin < 4
	s_in = 0.5;
end
if nargin < 3
	beta_in = 0.8;
end
if nargin < 2
	alpha_in = 0.1;
end

% intensity and color
intensity						= sum(HDRimg, 3) / 3;
% intensity						= HDRimg(:,:,1) * 0.3 + HDRimg(:,:,2) * 0.6 + HDRimg(:,:,3) * 0.1;
intensity(find(intensity == 0)) = 1e-8;

colorR		= HDRimg(:, :, 1) ./ intensity;
colorG		= HDRimg(:, :, 2) ./ intensity;
colorB		= HDRimg(:, :, 3) ./ intensity;

% log
logimg  = log(intensity);

% attenuaion
% decides number of levels
min_scale 	= 32;
scale_ratio = 0.5;
n_level 	= floor(log(min(width, height) / min_scale) / log(1 / scale_ratio));
phi_imgs	= cell(1, n_level + 1);

alpha		= alpha_in;
beta		= beta_in;

% compute phi image by guassian pyramid
lastimg = [];
AttMap = [];
for i = 1 : n_level + 1
  if i ~= 1
  	tempimg			= imresize(lastimg / 2, scale_ratio, 'bilinear');
  else
	tempimg			= logimg;
  end

  Gx 				= filter2([-0.5 0 0.5], tempimg);
  Gy 				= filter2([-0.5 0 0.5]', tempimg);
  phi_imgs{1, i} 	= sqrt((Gx .^ 2) + (Gy .^ 2));
  now_avg		 	= mean2(phi_imgs{1, i});

  t1 					= find(phi_imgs{1, i} ~= 0);
  t2 					= find(phi_imgs{1, i} == 0);
  phi_imgs{1, i}(t1) 	= min(1, (phi_imgs{1, i}(t1) / (alpha * now_avg)) .^ (beta - 1));
  phi_imgs{1, i}(t2) 	= 1;

  if max(max(phi_imgs{1,i})) > 1 || min(min(phi_imgs{1,i})) < 0
	fprintf('phi warning\n');
  end

  lastimg = tempimg;
end
clear tempimg lastimg Gx Gy t1 t2;

showimg = [];

% figure(1);
% imshow(phi_imgs{1,1});
showimg = phi_imgs{1,1};
% pause

% start attenuation
now_phi_img = phi_imgs{1, n_level + 1};
% imwrite(now_phi_img, ['debug/nowimg-', num2str(n_level + 1), '.jpg']);
for i = n_level : -1 : 1
  	% linear interpolation
	L_img = imresize(now_phi_img, [size(phi_imgs{1,i}, 1) size(phi_imgs{1,i}, 2)], 'bilinear');
	% L_img = imgexpand2(now_phi_img);

	now_phi_img = L_img .* phi_imgs{1, i};
	% imwrite(L_img, ['debug/Limg-', num2str(i), '.jpg']);
	% imwrite(phi_imgs{1, i}, ['debug/phiimg-', num2str(i), '.jpg']);
	% imwrite(now_phi_img, ['debug/nowimg-', num2str(i), '.jpg']);
end

AttMap = now_phi_img;
% sum(sum(isnan(AttMap)))
% pause
% figure(2);
% imshow(AttMap);
showimg = [showimg AttMap];
% pause
% return

clear now_phi_img L_img phi_imgs

% integral
% find gradient
Gx				= filter2([-1 0 1], logimg) .* AttMap;
Gy				= filter2([-1 0 1]', logimg) .* AttMap;
b               = filter2([-1 1 0], Gx) + filter2([-1 1 0]', Gy);
% figure(3);
% imshow(b);
showimg = [showimg b];
% return
% pause
b               = reshape(b, height * width, 1);
clear Gx Gy;

temp1					= zeros(height + 2, width + 2);
temp1(2:end-1, 2:end-1)	= reshape([1:height*width], height, width);
A						= -delsq(temp1);
clear temp1;

if showflag == 1
	imshow(showimg);
	pause(3);
end


% solve Ax = b by conjugate method
fprintf('start solve....\n');
x				= bicg(A, b, 1e-6, 1e4);
% x				= PoissonSolver2D(b);
fprintf('finished\n');
II              = reshape(x, height, width);
% II				= (II - min(min(II))) / (max(max(II)) - min(min(II))) - 1;
II				= II - max(max(II));

% exp
tone_intensity = exp(II);
% figure(4);
% imshow(tone_intensity);

color_exponent = s_in;
toneimg(:, :, 1) = (colorR .^ color_exponent) .* tone_intensity;
toneimg(:, :, 2) = (colorG .^ color_exponent) .* tone_intensity;
toneimg(:, :, 3) = (colorB .^ color_exponent) .* tone_intensity;

if showflag == 1
	figure(2);
	imshow(toneimg);
end

% pause

