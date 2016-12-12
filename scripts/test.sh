#!/bin/bash

cd ..
cmake ./source
make

# ir = 0
ir=0
alphaDen=1
rm -f ir$ir.dat
touch ir$ir.dat

for file in ir$ir
do
echo -e "# ir\talpha\tctx\tbpb" >> $file.dat
done

for((ctx=2;ctx<20;++ctx))
do
./phoenix -m t,$ctx,$alphaDen,$ir -t b.fa >> ir$ir.dat
done

for FILE in ir$ir.dat
do
    gnuplot <<- EOF
        set xlabel "context"
        set ylabel "bpb"
        #set title "Graph title"
        set term png
        set output "${FILE}.png"
        plot "${FILE}" using 3:4
EOF
done