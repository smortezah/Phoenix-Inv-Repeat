#!/bin/bash


rm -fr $FLD_GULL
git clone https://github.com/pratas/GULL.git
cd $FLD_GULL/src
cmake .
make
cd ../../
