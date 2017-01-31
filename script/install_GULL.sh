#!/usr/bin/env bash


rm -fr GULL
git clone https://github.com/pratas/GULL.git
cd ${FLD_GULL}/src
cmake .
make
cd ../../
