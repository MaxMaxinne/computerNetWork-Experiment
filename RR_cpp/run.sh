n=$1
make clean&&make
if [ ${n} -eq 1 ] 
then
    ./sim ${n} 95 100
elif [ ${n} -eq 3 ]
then
    ./sim ${n} 2 3 4 13 13 13
else
    echo "Unexpected params"
fi