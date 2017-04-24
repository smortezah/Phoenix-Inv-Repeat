#!/usr/bin/env bash


rm -fr $FLD_XS
git clone https://github.com/pratas/XS.git
cd $FLD_XS
make
cd ..
