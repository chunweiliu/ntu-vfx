function shifted_imgs = get_shifted_imgs( imgsrc, imgtars )
%GET_SHIFTED_IMGS Get several images and align them
%   shifted_imgs = get_shifted_imgs( imgsrc, imgtars )
%   INPUTS:     (A source image, Some target images cell)
%   OUTPUTS:    Shifted target images cells

MAXSHIFTBIT = 5;
shifted_imgs = cell(1, length( imgtars ) );
for n = 1:length(imgtars)
    imgtar = imgtars{n};
    [xs, ys] = imgalign( imgsrc, imgtar, MAXSHIFTBIT, 0, 0 );
    for m = 1:3
        shifted_img(:,:,m) = bitmapshift( xs, ys, imgtar(:,:,m), MAXSHIFTBIT );
    end
    shifted_imgs(1, n) = {shifted_img};
end