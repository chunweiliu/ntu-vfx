function [ fp, theta ] = mops_detect( gimg, F_THRESHOLD )
% Multi-Scale Oriented Patches (MOPS)
% Inputs:
%       gimg    m x n       uint8   :a gray level image
% Outputs:
%       fp      m x 2       double  :m's feature points and it's (x, y)
%       theta   m x 1       double  :m's feature oriented angle
% Parameters:
%       DETECTED_POINTS 1x1 uint8   :how many feature points we want
MAX_POINTS      = 50000;
DETECTED_POINTS = 500;
SIGMA_D         = 1.0;
SIGMA_I         = 1.5;
SIGMA_O         = 4.5;
P_SIZE          = 1;
P_FAC           = 0.9;
P_RFAC          = 1/P_FAC;


% Harris Matrix constructed 
% Compute the orientation 
% Compute the corner strength function f in multiscale
dimg        = double( gimg );
pimg        = dimg;
[row, col]  = size( pimg );
THETA       = zeros(row, col, P_SIZE);
FHM         = zeros(row, col, P_SIZE);
fp          = zeros(MAX_POINTS, 2);
theta       = zeros(MAX_POINTS, 1);
conner_f    = zeros(MAX_POINTS, 1);
count       = 0;
scale       = 1;
for k=1:P_SIZE
    
    [FX, FY]    = gradient( pimg );
    [row, col]  = size( pimg );

    G_O         = fspecial('gaussian', [12, 12], SIGMA_O);
    UX          = imfilter( FX, G_O );
    UY          = imfilter( FY, G_O );
    LENGTH      = (UX .^2 + UY .^ 2) .^ 0.5;

    THETA(1:row, 1:col, k) = acos(UX ./ LENGTH);    

    G_D         = fspecial('gaussian', [3, 3], SIGMA_D);
    FX_G_D      = imfilter( FX, G_D );
    FY_G_D      = imfilter( FY, G_D );
    FX_2_G_D    = FX_G_D .^ 2;
    FY_2_G_D    = FY_G_D .^ 2;
    FXY_G_D     = FX_G_D .* FY_G_D;

    G_I         = fspecial('gaussian', [3, 3], SIGMA_I);
    FX_G_I      = imfilter( FX_2_G_D, G_I );
    FY_G_I      = imfilter( FY_2_G_D, G_I);
    FXY_G_I     = imfilter( FXY_G_D, G_I );

    FHM(1:row, 1:col, k) = (FX_G_I .* FY_G_I - FXY_G_I .^ 2) ./ (FX_G_I + FY_G_I) ;

    % Compute if FHM(r, c, k) is the local maximum
    % feature block
    %b_row = 30;
    %e_row = 618;
    %b_col = 100;
    %e_col = 548;
    b_row = 1;
    e_row = row;
    b_col = 1;
    e_col = col;
    
    for n = b_row+1:e_row-1
        for m = b_col+1:e_col-1
            f               = FHM(n, m, k);

            is_local_max    =   f > F_THRESHOLD && ...
                                f > FHM(n-1, m-1, k) && ...
                                f > FHM(n-1, m, k) && ...
                                f > FHM(n-1, m+1, k) && ...
                                f > FHM(n, m-1, k) && ...
                                f > FHM(n, m+1, k) && ...
                                f > FHM(n+1, m-1, k) && ...
                                f > FHM(n+1, m, k) && ...
                                f > FHM(n+1, m+1, k);
            if( is_local_max )
                fp_refine = sp_refine( FHM(n-1:n+1, m-1:m+1, k), n, m );
                ridx = round(fp_refine(1)*scale);
                cidx = round(fp_refine(2)*scale);
                %ridx = round(n*scale);
                %cidx = round(m*scale);                
                %fp = [ridx, cidx, THETA(n, m, k), k, f]; % append f into the last
                count = count + 1;
                fp(count, :) = [ridx, cidx];
                theta(count) = THETA(n, m, k);
                conner_f(count) = f;
            end
        end
    end
        
    pimg        = imresize( pimg, P_FAC ); % scale in length
    scale       = scale * P_RFAC;    
end
%fp_list     = fp_list(1:count,:);
fp = fp(1:count,:);
theta = theta(1:count);
conner_f = conner_f(1:count);

%fp = sortrows(fp, -1);      % sort by row in decrease 
%fp = sortrows(fp, -2);
idx = anms( fp, conner_f );
idx = idx( 1:min( DETECTED_POINTS, size( fp(:,1)) ) );

fp = fp(idx, :);
theta = theta(idx);

function idx = anms( fp, fhm )
% Choose the local max feature ( Adaptive Non-maximal Suppression )
% Inputs:
%       fp          n x 2       uint8   : global intesting point
%       fhm         n x 1       double  : intesting point's conner response
% Outputs:
%       fp          m x 2       uint8   : local inesting point
eps     = 1e-10;
part    = 1024;      % grid on y axis ....
finish  = min(size(fp, 1), part);
begin   = 1;
r       = zeros( size(fp, 1), 1);
while( begin < size(fp, 1) )
    fhm_now             = fhm(begin:finish);
    F                   = fhm_now * ones(1, size(fhm_now, 1));
    bitmap              = (F - ( 0.9.*F' )) < 0;

    fp_now              = fp(begin:finish, :); 
    pair_wise_dist_fp   = pdist( fp_now );
    pair_wise_dist_fp   = squareform( pair_wise_dist_fp );
% find nonzero minimum
    candidates          = pair_wise_dist_fp .* bitmap;
    
    val                 = max( 1./ (candidates - eps) , [], 2 );
    r(begin:finish)     = (1 ./ val) + eps;
    
    begin               = finish + 1;
    finish              = min( finish + part, size(fp, 1) );
end
[val, idx] = sort(r, 'descend');

function refinedX = sp_refine_x( f, n, m)
% SP_REFINE
% Inputs:
%       f           3 x 3       double  : the function we want interpret
%       n           1 x 1       double  : row index of the pixel
%       m           1 x 1       double  : col index of the pixel
% Outputs:
%       refinedX    2 x 1       uint8   : pixel value of the pixel in 2-dim
%                                         plane [y, x]'
%       fefinedF    1 x 1       double  : real function values of FHM
fx  = ( f(2, 3) - f(2, 1) ) / 2;
fy  = ( f(3, 2) - f(1, 2) ) / 2;
fxx = f(2, 3) - 2*f(2,2) + f(2, 1);
fyy = f(3, 2) - 2*f(2,2) + f(1, 2);
fxy = (f(1, 1) - f(1, 3) - f(3, 1) + f(3, 3)) * 0.25;

%X   = [2, 2]';
FX  = [fx, fy]';
FXX = [fxx, fxy;...
       fxy, fyy];

refinedX = -inv( FXX ) * FX;
refinedX = refinedX + [n; m];
%refinedF = f(2,2) + FX' * X + 0.5.* (X') * FXX * X;

function refinedX = sp_refine( f, n, m)
% SP_REFINE
% Inputs:
%       f           3 x 3       double  : the function we want interpret
%       n           1 x 1       double  : row index of the pixel
%       m           1 x 1       double  : col index of the pixel
% Outputs:
%       refinedX    2 x 1       uint8   : pixel value of the pixel in 2-dim
%                                         plane [y, x]'
%       fefinedF    1 x 1       double  : real function values of FHM
fx  = ( f(3, 2) - f(1, 2) ) / 2;
fy  = ( f(2, 3) - f(2, 1) ) / 2;
fxx = f(3, 2) - 2*f(2,2) + f(1, 2);
fyy = f(2, 3) - 2*f(2,2) + f(2, 1);
fxy = (f(1, 1) - f(1, 3) - f(3, 1) + f(3, 3)) * 0.25;

%X   = [2, 2]';
FX  = [fx, fy]';
FXX = [fxx, fxy;...
       fxy, fyy];

refinedX = -inv( FXX ) * FX;
refinedX = refinedX + [n; m];
%refinedF = f(2,2) + FX' * X + 0.5.* (X') * FXX * X;
