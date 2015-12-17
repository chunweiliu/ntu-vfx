function [match, n_match] = mops_match( des1, des2, MATCH_THRESHOLD)
% Multi-Scale Oriented Patches (MOPS) matching
% Inputs:
%       fp1     n x 2       uint8   :n's feature points coordinate
%       des1    n x 64      double  :n's feature descriptor with 64-dim
%       fp2     m x 2       uint8   :m's feature points coordinate
%       des2    m x 64      double  :m's feature descriptor with 64-dim
% Outputs:
%       match   k x 4       double  :k's good feature matching pairs (x1, x2)
%                                    x1 is from fp1, x2 is from fp2
% Parameters:
%       MATCH_T 1 x 1       double  :a threshold of matching, lower is
%                                    more stricter
%Brute Force Search but accuracy
%MATCH_T     = 0.65;
match       = zeros(size(des1, 1), 1);
for n = 1:size(des1, 1)
    
    des         = ones(size(des2,1), 1) * des1(n,:);
    diffc       = des2 - des;
    dis         = ( sum( (diffc).^2, 2) ); % simply 2 norm
    [sim, idx]  = sort( dis );
    e_1nn       = sim(1);
    e_2nn       = sim(2);

    if e_2nn ~= 0 && ...
       (e_1nn / e_2nn) < MATCH_THRESHOLD
        match(n) = idx(1);
    end
end
n_match = sum(match ~= 0);
    