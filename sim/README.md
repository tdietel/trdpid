
TRD Simulation with AliRoot
===========================

This directory contains a simple simulation of the TRD with AliRoot, using
`dpgsim.sh`. The purpose of this simulation is to produces simulated TRD digits
(and possibly also online tracklets and GTU tracks) for comparison with data
and the new O2 simulation.

The old simulation code found here, which was based on a 2016 honours project,
and of which some fragments still linger around, is now obsolete and will be
removed soon.

Overview
--------

The simulation consists of the following steps:

- creation of OCDB snapshots for simulation and reconstruction
- simulation
- reconstruction
- QA

The first step, creation of OCDB snapshots, can be done once for all
simulations. The following three steps are specific to each simulation setup.
They will be executed in a directory specific for each setup, and controlled by
a specific makefile.

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

At the time of writing of this README, there were two simulations set up: *pythia* and *particlegun*, which can be run, respectively, with
```
make pythia
make particlegun
```

The actual running of these simulations is controlled by `pythia.mk` and
`particlegun.mk`, respectively. These two makefiles will be symlinked into the
directory where the code will be run, and make will be called in these
directories to produce the actual results. The makefiles pull in other ROOT
macros and configuration files as needed.
