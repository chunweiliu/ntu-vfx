function shifted_img = bitmapshift( xs, ys, img, shift_bit )
%IMGSHIFT Shift image by (xs, xy)
%   INPUT:  shifted weight x, shifted weight y, bitmap image
%   OUTPUT: shifted bitmap
%MAXSHIFT = 2 ^ shift_bit;
MAXSHIFT = 64;
[row, col] = size(img);
left = 1+MAXSHIFT;
top = 1+MAXSHIFT;
right = left + row - 1;
down = top + col - 1;
tmp = zeros(row+2*MAXSHIFT,col+2*MAXSHIFT);
tmp = uint8(tmp); % for debug
tmp(left+xs:right+xs, top+ys:down+ys) = img;
shifted_img = tmp(left:right, top:down);


