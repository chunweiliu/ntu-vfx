function [ret_shift_x, ret_shift_y] = imgalign( imgsrc, imgtar, maxoffset,...
                                                cur_shift_x, cur_shift_y)
%IMGALIGN Image Alignment
%   INPUT:  (source image, target image)
%   OUTPUT: minumun offset of target image from source image
%   Do image alignment for image translation

if( maxoffset > 0 )
    sml_imgsrc = imresize(imgsrc, 0.5);
    sml_imgtar = imresize(imgtar, 0.5);
    [cur_shift_x, cur_shift_y] = imgalign(sml_imgsrc, sml_imgtar, maxoffset-1,...
                                    cur_shift_x, cur_shift_y);
    cur_shift_x = cur_shift_x * 2;
    cur_shift_y = cur_shift_y * 2;
else
    cur_shift_x = 0;
    cur_shift_y = 0;
end

[tb_src, eb_src] = getbitmap( imgsrc );
[tb_tar, eb_tar] = getbitmap( imgtar );

shift_bit = maxoffset;
min_err = size(imgsrc,1)*size(imgsrc,2);
for n = -1:1
    for m = -1:1
        xs = cur_shift_x + n;
        ys = cur_shift_y + m;
        shifted_tb_tar = bitmapshift(xs, ys, tb_tar, shift_bit);       
        shifted_eb_tar = bitmapshift(xs, ys, eb_tar, shift_bit);
        diff_b = xor( tb_src, shifted_tb_tar );
        diff_b = diff_b & eb_src;
        diff_b = diff_b & shifted_eb_tar;
        err = sum(sum(diff_b));
        if err < min_err
            ret_shift_x = xs;
            ret_shift_y = ys;
            min_err = err;
        end
    end
end

