
imgdir=$1
n_img=$2

for(( i=0 ; i < $2 ; i++ )); do
    mv $imgdir/$i.JPG $imgdir/$i.jpg
done
