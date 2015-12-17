function imre(folder )

FORMAT          = '\*.jpg';
files           = dir( strcat(folder, FORMAT) );
for n = 1:length( files )
    filename = strcat(folder, '\', files(n).name);
    img = imread( filename );
    img = imresize( img, 0.5 );
    imwrite( img, filename );
end