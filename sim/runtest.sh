#!/bin/sh

# set up the test directoru
mkdir -p test
cd test
ln -sf ../Config.C Config.C
ln -sf ../sim.C sim.C
ln -sf ../rec.C rec.C

ls -la

# clean up
rm -rf *.root *.dat *.log fort* hlt hough raw* recraw/*.root recraw/*.log

# run simulation
aliroot -b -q sim.C      2>&1 | tee sim.log
mv syswatch.log simwatch.log

# run reconstruction
aliroot -b -q rec.C      2>&1 | tee rec.log
mv syswatch.log recwatch.log

aliroot -b -q ${ALICE_ROOT}/STEER/macros/CheckESD.C 2>&1 | tee check.log

# skip AOD generation
#aliroot -b -q aod.C 2>&1 | tee aod.log

# run the raw reconstruction 
mkdir recraw
cd recraw
ln -s ../raw.root
aliroot -b -q ../../rawrec.C 2>&1 | tee rawrec.log
#aliroot -b -q  2>&1 aod.C | tee aod.log

