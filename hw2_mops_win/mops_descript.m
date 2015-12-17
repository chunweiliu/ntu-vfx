function des = mops_descript( fp, theta, gimg )
% Multi-Scale Oriented Patches (MOPS) detector and descriptor
% Input:
%       gimg    n x m       uint8   :a gray level image
% Output:
%       fp      k x 2       double  :k's feature points coordinate
%       des     k x 64      double  :k's feature descriptor with 64-dim
eps             = 1e-10;
row             = size(gimg, 1);
col             = size(gimg, 2);
des             = zeros( size( fp, 1), 64 );
dp_idx(1,:)     = -20:5:15; % every row delta
dp_idx(2,:)     = -20:5:15; % every col index  delta
for n = 1:length(fp)    
    % rotate
    th      = theta(n);
    th      = -th;
    M       = [cos(th), sin(th);...
              -sin(th), cos(th)];
    p_idx   = M * dp_idx;    
    p_idx   = round(p_idx);
    
    % get the position of index 
    ridx  = p_idx(1,:) + fp(n,1);
    cidx  = p_idx(2,:) + fp(n,2);
    
    % bondary conditions
    ridx( ridx < 1 ) = 1;  
    cidx( cidx < 1 ) = 1;
    ridx( ridx > row ) = row;
    cidx( cidx > col ) = col;    
    patch   = gimg(ridx, cidx);
    
    
    patch   = double( patch );
    p_mean  = mean( mean( patch ) );
    p_std   = std( std ( patch ) );
    patch   = ( patch - p_mean ) ./ (p_std + eps);
        
    des(n,:)= reshape( patch, 1, 8*8 );
end