

all: pythia

pythia: pythia.log
pythia.log: ocdb/277312/OCDBsim.root ocdb/277312/OCDBrec.root

%.log: %.mk
	mkdir -p $*
	ln -sf $(PWD)/$< $*/Makefile
	make -C $* | tee $@


.INTERMEDIATE: snapshot.log
.PRECIOUS: OCDBsim.root OCDBrec.root
OCDBsim.root OCDBrec.root:
	@echo "=== Running OCDB snapshot creation for run ${RUN} ==="
	@$(ALIDPG_ROOT)/bin/aliroot_dpgsim.sh --run ${RUN} --mode ocdb \
	2>&1 | tee snapshot.log

clean:
	rm -f *.log
