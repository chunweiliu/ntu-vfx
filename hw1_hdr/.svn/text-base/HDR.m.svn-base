
function [HDRimage] = HDR(images, shspeed, smoothness, npixel)
% 
% function [HDRimage] = HDR(images)
% HDR:
% 	Use HDR method to make a HDR image 
% input:
%	images		1 * N cell	images with different shutter speed
%	shspeed		1 * N		shutter speed of i-th image
% 	smoothness  1 * 1		smooth term in Debevec's method (opt.)
% 	npixel		1 * P		number of pixel to HDR (opt.)
% output:
%	HDRimage	H * W * 3	A HDR image
%

% number of images
n_images = size(images, 2);
% number of pixels
if nargin < 4
  npixel = ceil(600 / n_images); 
end
if nargin < 3
  smoothness = 2;
end

[height, width, channel] = size(images{1, 1});

% choose pixel from the small gradient area, now random
flag_randpos = 1;
pixels_pos = zeros(npixel, 2);
if flag_randpos == 0
  for i = 1 : npixel
	pixels_pos(i, 1) = ceil(rand() * height); 
	pixels_pos(i, 2) = ceil(rand() * width); 
  end
else
  % find graident of mid-shspeed image
  mid_image 	= images{1, ceil(n_images / 2)};
  mid_grayimg 	= double(rgb2gray(mid_image));
  
  grad_x(:, 2 : width - 1) 	= (mid_grayimg(:, 1 : width - 2) - mid_grayimg(:, 3 : width)) .^ 2;
  grad_x(:, [1, width]) 	= (256) .^ 2;
  grad_y(2 : height - 1, :) = (mid_grayimg(1 : height - 2, :) - mid_grayimg(3 : height, :)) .^ 2;
  grad_y([1, height], :)	= (256) .^ 2;
  
  grad_xy 					= reshape(sqrt(grad_x + grad_y), 1, height * width);
  
  % reduce noise
  % grad_t					= 40;
  % grad_xy(find(grad_xy) < grad_t) = 0;
  % ------
  % for debug use
  % img_gradxy = sqrt(grad_x + grad_y);
  % img_gradxy(find(img_gradxy < 40)) = 0;
  % max_gradxy = max(max(img_gradxy));
  % img_gradxy = img_gradxy / max_gradxy;
  % imshow(img_gradxy);
  % return

  % assign pixel position with the npixel-least gradients
  [sort_grad IX]= sort(grad_xy);
  grad_thres 	= sort_grad(npixel);
  sort_grad  	= sort_grad(find(sort_grad < grad_thres + 10));
  rand_idx 		= randperm(length(sort_grad));

  for i = 1 : npixel
	  now_IX = IX(rand_idx(i)) - 1;
	  pixels_pos(i, 1) = rem(now_IX, height) + 1;
	  pixels_pos(i, 2) = floor(now_IX / height) + 1;
  end
end

% get pixels
Zr = zeros(npixel, n_images);
Zg = zeros(npixel, n_images);
Zb = zeros(npixel, n_images);
for i = 1:n_images
  	for j = 1:npixel
	  Zr(j, i) = images{1, i}(pixels_pos(j, 1), pixels_pos(j, 2), 1);
	  Zg(j, i) = images{1, i}(pixels_pos(j, 1), pixels_pos(j, 2), 2);
	  Zb(j, i) = images{1, i}(pixels_pos(j, 1), pixels_pos(j, 2), 3);
	end
end

% Do HDR
smooth_l = smoothness;
weight = ones(1, 256) * 128 - abs([0 : 255] - ones(1, 256) * 128);
B = log(shspeed);
[Gr, lEr] = Debevec(Zr, B, smooth_l, weight);
[Gg, lEg] = Debevec(Zg, B, smooth_l, weight);
[Gb, lEb] = Debevec(Zb, B, smooth_l, weight);

HDRimage = zeros(height, width, channel);
for i = 1 : height
  for j = 1 : width
	now_pixel_r = 0;
	now_sum_r = 0;
	now_pixel_g = 0;
	now_sum_g = 0;
	now_pixel_b = 0;
	now_sum_b = 0;
	for k = 1 : n_images
	  tr = images{1, k}(i, j, 1) + 1; 
	  tg = images{1, k}(i, j, 2) + 1; 
	  tb = images{1, k}(i, j, 3) + 1; 

	  now_pixel_r = now_pixel_r + (Gr(tr) - B(k)) * weight(tr);
	  now_sum_r = now_sum_r + weight(tr);
	  now_pixel_g = now_pixel_g + (Gg(tg) - B(k)) * weight(tg);
	  now_sum_g = now_sum_g + weight(tg);
	  now_pixel_b = now_pixel_b + (Gb(tb) - B(k)) * weight(tb);
	  now_sum_b = now_sum_b + weight(tb);
	end
	now_pixel = [now_pixel_r now_pixel_g now_pixel_b] ./ [now_sum_r now_sum_g now_sum_b];
	if now_sum_r == 0
	  now_pixel(1) = 0;
	end
	if now_sum_g == 0
	  now_pixel(2) = 0;
	end
	if now_sum_b == 0
	  now_pixel(3) = 0;
	end
	HDRimage(i, j, :) = now_pixel;
  end
end

HDRimage = exp(HDRimage);

