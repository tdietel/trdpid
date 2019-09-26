
TRD Simulation with AliRoot
===========================

This directory contains a simple simulation of the TRD with AliRoot, using
`dpgsim.sh`. The purpose of this simulation is to produces simulated TRD digits
(and possibly also online tracklets and GTU tracks) for comparison with data
and the new O2 simulation.

Overview
--------

The simulation consists of the following steps:

- creation of OCDB snapshots for simulation and reconstruction
- simulation
- reconstruction
- QA

Usage
-----

### Preparation of AliRoot and AliDPG

Apart from the usual dependencies, also AliDPG will be needed to run this
simulation. `aliBuild` can be used to build everything:
```
aliBuild build AliDPG --defaults next-root6
aliBuild build AliPhysics --defaults next-root6
```

Then one needs to set up both packages:
```
alienv enter AliPhysics/latest AliDPG/latest
```

A token is needed for some things:
```
alien-token-init
```

### Running the code

Make sure you are in the correct directory:
```
cd PATH/TO/trdpid/sim/dpgsim
```

A `Makefile` should control the running of all simulation steps as necessary.
To get started, a simple invocation of make with the default targets should
work:
```
make
```

Please take a look at the Makefile to see what commands are actually run.

Bugs
----

The `Makefile` should take care of running everything in the correct order,
and only if relevant files were updated. However there seems to be a bug, such
that the full chain is rerun, even if only one of the
