
srcdir=$1
tardir=$2
n_img=$3
focus=$4
k1=$5
k2=$6

test -d $tardir || mkdir $tardir
for(( i = 0 ; i < $n_img ; i++ )); do
    ./project2 rdWarp $srcdir/$i.jpg $tardir/$i.jpg $focus $k1 $k2
done
