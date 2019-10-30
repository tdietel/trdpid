#!/bin/sh

RUN=$1
DPGSIM=${ALIDPG_ROOT}/bin/aliroot_dpgsim.sh

if [ "$RUN" == "" ]
then
  echo "usage: $0 <RUN_NUMBER>"
  exit 1
fi


mkdir -p snapshots/$RUN
cd snapshots/$RUN
$DPGSIM --run $RUN --mode ocdb 2>&1 | tee snapshot.log
cd ../..
