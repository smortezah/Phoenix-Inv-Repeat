#!/bin/bash


rm -fr $FLD_GOOSE
git clone https://github.com/pratas/goose.git
cd $FLD_GOOSE/src
make
cd ../../
