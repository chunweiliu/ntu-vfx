
function [img_cell] = loadimg(dirstr, n_img)
%
% function [img_cell] = loadimg(dirstr, n_img)
% loadimg: 
%	load image with filename 'imgXX.jpg' from specified directory
% input:
%	dirstr:		string		directory path
%	n_img		1 * 1		number of images
% output:
%	img_cell	1 * ncells	cell of images
%

img_strs = [];
img_cell = [];
if nargin < 2
  fprintf('not support yet\n');
  return
else
  for i = 1 : n_img
	d2 = floor(i / 10);
	d1 = rem(i, 10);
    temp_str = [dirstr, '/', 'img', num2str(d2), num2str(d1), '.', 'j', 'p', 'g'];
	img_cell{1, i} = imread(temp_str);
  end
end

