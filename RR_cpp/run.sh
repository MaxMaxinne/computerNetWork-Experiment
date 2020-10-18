n=$1
make clean&&make
if [ ${n} -eq 1 ] 
then
    ./sim ${n} 95 100
elif [ ${n} -eq 3 ]
then
    ./sim ${n} 3 4 5 13 11 14
else
    echo "Unexpected params"
fi