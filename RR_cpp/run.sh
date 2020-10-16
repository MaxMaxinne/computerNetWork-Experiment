n=$1
make clean&&make
if [ ${n} -eq 1 ] 
then
    ./sim ${n} 95 100
elif [ ${n} -eq 3 ]
then
    ./sim ${n} 30 60 90 50 50 50
else
    echo "Unexpected params"
fi