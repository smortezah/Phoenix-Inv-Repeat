#!/bin/bash


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   filter tot ir=0, tot ir=1 and diff by threshold
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
$FLD_GOOSE/src/goose-reducematrixbythreshold 0.5 < diff-$REF-$TAR.dat \
 > diff-filtered-$REF-$TAR.dat
