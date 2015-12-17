function [g, lE] = Debevec(Z, B, l, w)
% 
% function [g, lE] = Debevec(Z, B, l, w)
% Debevec.m:
%	use debevec's method to recover radiance curve
% input:
%	Z 		P * N		i-th pixel in j-th image
%	B		1 * N		log shutter speed of j-th image
%	l		1 * 1		lambda to decide smoothness
%	w		1 * 256		weight function of pixel value 0 ~ 255
% output:
% 	g   	256 * 1	 	log exposure of pixel value 0 ~ 255
% 	lE		P * 1 		log irradiance of i-th pixel
%

n = 256;
A = zeros(size(Z,1)*size(Z,2)+n+1,n+size(Z,1));
b = zeros(size(A,1),1);

k = 1;              %% Include the data-fitting equations
for i=1:size(Z,1)
  for j=1:size(Z,2)
    wij = w(Z(i,j)+1);
    A(k,Z(i,j)+1) = wij; A(k,n+i) = -wij; b(k,1) = wij * B(1,j);
    k=k+1;
  end
end

A(k,129) = 1;       %% Fix the curve by setting its middle value to 0
k=k+1;

for i=1:n-2         %% Include the smoothness equations
  A(k,i)=l*w(i+1); A(k,i+1)=-2*l*w(i+1); A(k,i+2)=l*w(i+1);
  k=k+1;
end

x = A\b;            %% Solve the system using SVD

g = x(1:n);
lE = x(n+1:size(x,1));

