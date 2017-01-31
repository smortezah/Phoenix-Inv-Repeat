#!/usr/bin/env bash


rm -fr GOOSE
git clone https://github.com/pratas/goose.git
cd ${FLD_GOOSE}/src
make
cd ../../
