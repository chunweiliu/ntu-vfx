function ldr = bimap( hdr )
%BIMAPPING Contrast reduction use bilateral mapping
%Input:
%   -   'hdr'   n by m  single  : A HDR image
%Output:
%   -   'ldr'   n by m  unit8   : A LDR image for display

hdr             = double( hdr );                    % convert to double
r               = hdr(:,:,1);
g               = hdr(:,:,2);
b               = hdr(:,:,3);
intensity       = 0.2999*r + 0.587*g + 0.114*b;     % get the intensitive
baseIntensity   = bilateralFilter( intensity );     % get the base image
detailIntensity = intensity ./ baseIntensity;       % get the detail

logbase         = log( baseIntensity );             % trans. to logspace
colormax        = max( max( max( logbase ) ) );     % translate to [0, 1]
colormin        = min( min( min( logbase ) ) );
baseContrast    = ( logbase - colormin ) ./ (colormax - colormin);

ldrIntensity    = baseContrast .* detailIntensity;
oldcolor_max    = max( max(r,g), b );               % get the max value
t               = r ./ oldcolor_max;
u               = g ./ oldcolor_max;
v               = b ./ oldcolor_max;
frac            = 0.2999*t + 0.587*u + 0.114*v;
newcolor_max    = ldrIntensity ./ frac;

ldr(:,:,1)      = t .* newcolor_max;                % ldr's r chanel
ldr(:,:,2)      = u .* newcolor_max;                % get the color back
ldr(:,:,3)      = v .* newcolor_max;                % get the color back
