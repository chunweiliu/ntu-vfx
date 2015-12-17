function demo

img1 = imread('ntulib/0.jpg');
img2 = imread('ntulib/1.jpg');
[fp1, des1] = mops( img1 );
[fp2, des2] = mops( img2 );
cmatch = mops_match( des1, des2 );
match = zeros(size(cmatch,1), 4);
count = 0;
for n = 1:length(cmatch)
    if(cmatch(n) ~= 0)
        count = count + 1;
        match(count,:) = [fp1(n,1), fp1(n,2), fp2(cmatch(n),1), fp2(cmatch(n),2)];        
    end
end
match = match(1:count-1, :);
% Feature Points
% for n = 1:length(fp1)
%     x = fp1(n,1);
%     y = fp1(n,2);    
%     img1(x-1:x+1, y-1:y+1, 1) = 255;
%     img1(x-1:x+1, y-1:y+1, 2) = 0;
%     img1(x-1:x+1, y-1:y+1, 3) = 0;
% end
% 
% for n = 1:length(fp2)
%     x = fp2(n,1);
%     y = fp2(n,2);    
%     img2(x-1:x+1, y-1:y+1, 1) = 0;
%     img2(x-1:x+1, y-1:y+1, 2) = 0;
%     img2(x-1:x+1, y-1:y+1, 3) = 255;
% end
% 
% row = max( size(img1, 1), size(img2, 1) );  % row is y cord. !!!!!!!!
% col = size(img1, 2)+size(img2, 2);          % col is x cord. !!!!!!!!
% canvas = uint8(zeros( row, col, 3 ));
% canvas(1:size(img1, 1), 1:size(img1,2), :) = img1;
%  
% canvas(1:size(img2,1), size(img1, 2)+1:col, :) = img2;
%  
% imshow(canvas);

for n = 1:length(match)
    x = match(n,1);
    y = match(n,2);    
    img1(x-1:x+1, y-1:y+1, 1) = 255;
    img1(x-1:x+1, y-1:y+1, 2) = 0;
    img1(x-1:x+1, y-1:y+1, 3) = 0;
    
    x = match(n,3);
    y = match(n,4);    
    img2(x-1:x+1, y-1:y+1, 1) = 0;
    img2(x-1:x+1, y-1:y+1, 2) = 0;
    img2(x-1:x+1, y-1:y+1, 3) = 255;
end

row = max( size(img1, 1), size(img2, 1) );  % row is y cord. !!!!!!!!
col = size(img1, 2)+size(img2, 2);          % col is x cord. !!!!!!!!
canvas = uint8(zeros( row, col, 3 ));
canvas(1:size(img1, 1), 1:size(img1,2), :) = img1;

canvas(1:size(img2,1), size(img1, 2)+1:col, :) = img2;

imshow(canvas);
hold on;

match1 = [match(:, 1), match(:, 2)];
match2 = [match(:, 3), match(:, 4)+ size(img1,2)];      
plot([match2(1,2), match1(1,2)], [match2(1,1), match1(1,1)], 'g');

for n = 2:size(match, 1)
    xbegin  = match1(n, 2);
    xend    = match2(n, 2);
    ybegin  = match1(n, 1);
    yend    = match2(n, 1);
    plot([xbegin, xend], [ybegin, yend], 'g');
end

frame = getframe( gcf );
imwrite( frame.cdata, 'matches.png' );
hold off;

frame = getframe(gcf);
imwrite(frame.cdata, 'output.png', 'png');

