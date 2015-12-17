
function [ToneImg] = tonemap_P(HDRimg)
%
% function [ToneImg] = tonemap_P(HDRimg)
% tonemap_P:
%	use Photographic Reproduction to apply tonemapping
% input:
%	HDRimg		H * W * 3	HDR image (0~inf)
% output:
%   ToneImg		H * W * 3	Tonemapping HDR image (0 ~ 1)
%	%RGBimg		H * W * 3	RGB image
%

% some parameters
delta 	= 1e-5;
key		= 0.18;
L_white	= 1e5;

% reshape HDR image to HDRarray
[height, width, channel] 	= size(HDRimg);
HDRarray(1, :)				= reshape(HDRimg(:, :, 1), 1, height * width);
HDRarray(2, :)				= reshape(HDRimg(:, :, 2), 1, height * width);
HDRarray(3, :)				= reshape(HDRimg(:, :, 3), 1, height * width);

% for each channel

% find Lw_bar
Lw_bar 	= exp(sum(sum(log(HDRarray + delta))) / (height * width * 3));
Lm		= HDRarray / Lw_bar * key;
Ld		= (1 + Lm / (L_white ^ 2)) .* Lm ./ (1 + Lm);

ToneImg(:, :, 1) = reshape(Ld(1, :), height, width);
ToneImg(:, :, 2) = reshape(Ld(2, :), height, width);
ToneImg(:, :, 3) = reshape(Ld(3, :), height, width);

