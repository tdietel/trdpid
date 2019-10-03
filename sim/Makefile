

all: pythia

pythia: pythia.log
particlegun: particlegun.log

pythia.log: ocdb/277312/OCDBsim.root ocdb/277312/OCDBrec.root
particlegun.log: ocdb/277312/OCDBsim.root ocdb/277312/OCDBrec.root

%.log: %.mk
	mkdir -p $*
	ln -sf $(PWD)/$< $*/Makefile
	make -C $* | tee $@

.INTERMEDIATE: ocdb/%/snapshot.log
.PRECIOUS: ocdb/%/OCDBsim.root ocdb/%/OCDBrec.root
ocdb/%/OCDBsim.root: ocdb/%/snapshot.log
ocdb/%/OCDBrec.root: ocdb/%/snapshot.log

ocdb/277312/OCDBsim.root ocdb/277312/OCDBrec.root: ocdb/277312/snapshot.log

ocdb/%/snapshot.log:
	@echo "=== Running OCDB snapshot creation for run $* ==="
	mkdir -p $(dir $@) 
	cd $(dir $@) ; \
	$(ALIDPG_ROOT)/bin/aliroot_dpgsim.sh --run $* --mode ocdb \
	2>&1 | tee snapshot.log

clean:
	rm -f *.log
