
function [HDRimg, G, opt_iters, final_err] = Robertson(images, shspeed, max_iter, weight, G_init)
%
% function [HDRimg, G, opt_iters, final_err] = Robertson(images, shspeed, max_iter, weight, G_init)
% Robertson:
%	Robertson's method for HDR
% input:
%	images		1 * N cells	i-th image in each cell
%	shspeed		1 * N		log shutter speed of j-th image
%   max_iter	1 * 1		max number of iteration
%	weight		1 * 256		weight function of pixel value 0 ~ 255
%	G_init		3 * 256		optional, initial guess of recover curve of 3 channels
% output:
%	HDRimg 		W * H * C	radiance HDR image from robertson's method
% 	G   		3 * 256	 	log exposure of pixel value 0 ~ 255 for 3 channels
% 	opt_iters	1 * 3		number of optimize iterations for each channel
% 	final_err	1 * 3		error of optimization for each channel
%

if nargin < 5
	% initial guess of G
	G_init(1, 1:256) = [1:256] / 128;
	G_init(2, 1:256) = [1:256] / 128;
	G_init(3, 1:256) = [1:256] / 128;
end
if nargin < 4
  % set weight function
  % weight = ones(1, 256) * 128 - abs([0 : 255] - ones(1, 256) * 128);
  weight = ones(1, 256) * 129 - abs([0:255] - ones(1, 256) * 128);
end
if nargin < 3
  max_iter = 10;
end

% normalize G_init
for i = 1 : 3
  if G_init(i, 128) ~= 1
	G_init(i, :) = G_init(i, :) / G_init(i, 128);
  end
end

% set G_init
% temp_mat = [256.^2 256 1; 128.^2 128 1; 1 1 1];
% temp_vec = [3 1 0.01];
% temp_mat = (inv(temp_mat) * temp_vec');
temp_mat = [0.99 / (128.^3) 1]';
% hdl = @(X) ([X'.^2 X' ones(length(X), 1)] * temp_mat)';
hdl = @(X) ([(X-128)'.^3 ones(length(X), 1)] * temp_mat)';
G_init(1, :) = hdl(1:256);
G_init(2, :) = hdl(1:256);
G_init(3, :) = hdl(1:256);

% set weight
% hdl = @(X) ([X'.^4 X'.^3 X'.^2 ones(length(X), 1)] * [0.25 -128 16384 0.001]')';
% weight = hdl([1:256]);

% start optimization
[height width channel] 	= size(images{1,1});
if channel ~= 3
  fprintf('error: image are not rgb format\n');
end

HDRimg					= zeros(height, width, channel);
n_images 				= size(images, 2);
threshold 				= 1e-5;
total_pixel 			= height * width;
shspeed_rep				= repmat(shspeed', 1, total_pixel);
shspeed_square_rep		= shspeed_rep .^ 2;
% max_iter				= 100;

for channel = 1 : 3
  % reshape cell images
  slice_image	= [];
  for i = 1 : n_images
    slice_image(i, :) = reshape(double(images{1, i}(:, :, channel)), 1, total_pixel) + 1;
  end
  
  % pixel weight
  total_weight = weight(slice_image);

  opterr 		= 1e3;
  pre_opterr	= 1e10;
  counter 		= 0;
  nowG			= G_init(channel, :);
  while 1
	counter = counter + 1;
	% optimize E
	bottom 	= sum(total_weight .* shspeed_square_rep);
	up		= sum(total_weight .* nowG(slice_image) .* shspeed_rep);
	nowE	= up ./ bottom;

	% compute optimize error
	opterr	= sum(sum(total_weight .* ((nowG(slice_image) - (shspeed' * nowE)) .^ 2)));

	% check threshold met
	% opterr
	if opterr < threshold || counter == max_iter || (counter ~= 1 && pre_opterr / opterr < 1.05) %|| counter == 1
	  % store iter count, G, E
	  opt_iters(channel) 	= counter;
	  G(channel, :) 		= nowG;
	  HDRimg(:, :, channel) = reshape(nowE, height, width);
	  final_err(channel)	= opterr;
	  break;
	end
	pre_opterr = opterr;

	% optimize G
	for intensity = 1 : 256
		[nowT, nowR] = find(slice_image == intensity);
		if length(nowT) == 0
		  nowG(intensity) = 0;
		  fprintf('warning: no intensity %d occur', intensity);
		else
		  nowG(intensity) = sum(shspeed(nowT) .* nowE(nowR)) / length(nowT);
		end
	end
	% interpolation of non-occurance value
	% temp_idx = find(nowG < 0);
	% nowG(temp_idx) = (nowG(temp_idx - 1) + nowG(temp_idx + 1)) / 2;

	% normalize to nowG(128) = 1
	nowG = nowG / nowG(128);
  end
end

