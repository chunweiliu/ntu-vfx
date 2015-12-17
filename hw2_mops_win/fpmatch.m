function fpmatch( folder )
% Auto Compute Matches of A Set of Images from A folder
% Inputs:
%       folder      1 x n       char    : a folder path
% Outputs:
%       file format
tic;

% Read Parameters File
%filename        = strcat(folder, '\', folder, '.txt');
%fid             = fopen( filename, 'r' );
%parameters      = fscanf( fid, '%f', [2 inf] );
%fclose( fid );
%F_THRESHOLD     = parameters(1);
%MATCH_THRESHOLD = parameters(2);
F_THRESHOLD     = 10;
MATCH_THRESHOLD = 0.65;


% Read JPEG FILE
FORMAT          = '\*.jpg';
files           = dir( strcat(folder, FORMAT) );
fp_cell         = cell(1, length( files ) );
des_cell        = cell(1, length( files ) );
im_cell         = cell(1, length( files ) );

for n = 1:length( files )
    filename = strcat(folder, '\', files(n).name);
    img = imread( filename );    
    
    [fp, des] = mops( img, F_THRESHOLD );
    
    filename = files(n).name;
    filename = filename(1:length(filename)-4);
    filenum  = str2double( filename ) + 1;
    fp_cell{filenum} = fp;                  % the file idx number
    des_cell{filenum} = des;  
    im_cell{filenum} = img;
    
    filename = strcat(folder, '\', 'feat', filename, '.txt');
    fid = fopen( filename, 'w+' );
    fprintf( fid, '%d\n\n', size(fp, 1) );
    for m = 1:size( fp, 1 )
        fprintf( fid, '1\n');
        fprintf( fid, '%d\n', m-1 );
        fprintf( fid, '%d %d\n', fp(m, 2), fp(m, 1) ); % (x, y) cord
        fprintf( fid, '%d\n', size(des, 2) );
        for d = 1:size( des, 2 )
            fprintf( fid, '%f ', des(m, d));
        end
        fprintf( fid, '\n\n' );
    end
    fclose( fid );    
end
fprintf(1, 'feat*.txt in "%s" are constructed : ', folder);
toc;

tic;
for n = 1:length(files)
    filename1 = files(n).name;
    filename1 = filename1(1:length(filename1) - 4);
    filenum1  = str2double(filename1) + 1;
    for m = 1:length(files)        
        if m == n
            continue;
        end
        
        
        filename2 = files(m).name;
        filename2 = filename2(1:length(filename2) - 4);
        filenum2 = str2double(filename2) + 1;
        
        [match, n_match] = mops_match( des_cell{filenum1}, des_cell{filenum2},  MATCH_THRESHOLD);
        
        filename = strcat(folder, '\match_', filename1, '_', filename2);
        
        %draw_match( im_cell{filenum1}, im_cell{filenum2}, ...
        %            fp_cell{filenum1}, fp_cell{filenum2}, match, filename);
        
        filename = strcat(filename, '.txt');
        fid = fopen( filename, 'w+');
        fprintf( fid, '%d %d\n', size(match, 1), n_match );
        for k = 1:length(match)
            fprintf( fid, '%d %d\n', k-1, match(k)-1 );
        end
        fclose( fid );
    end
end
fprintf(1, 'match*_*.txt in "%s" are constructed : ', folder);
toc;