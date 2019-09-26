RUN     = 277312
#RUN     = 117222
MODE    = "sim,rec"
NEVENTS = 1
EXTRA   = "--bmin 14.5 --bmax 15.5 --pthardbin 1 --pttrigmin 3.5 --pttrigmax 5.5"
REC     = "Custom"

OBJECTS = Pythia6_Perugia2011 \
	  Pythia6Jets_Perugia2011 \
	  Pythia6GammaJet_Perugia2011 \
	  Pythia8_Monash2013 \
	  Pythia8Jets_Monash2013 \
	  Phojet \
	  EPOSLHC \
	  Hijing

SOURCEDIR   = $(ALIDPG_ROOT)/MC/CustomGenerators/
SOURCES    := $(shell find $(SOURCEDIR) -name '*.C')
OBJECTS    += $(SOURCES:$(SOURCEDIR)%.C=%)
OBJECTS    += PWGGA/SingleParticleGun001


#all: $(OBJECTS)

.PHONY: all
all: Pythia6_Perugia2011/digitsqa.root

DPGSIM  = $(ALIDPG_ROOT)/bin/aliroot_dpgsim.sh
DPGSIM += --run ${RUN} --reconstruction ${REC}
DPGSIM +=--nevents ${NEVENTS} ${EXTRA}

#%: %/AliESDs.root %/galice.root %/digitsqa.root

.PRECIOUS: %/galice.root
%/galice.root: %/OCDBsim.root %/OCDBrec.root
	cd $(dir $@) &&  $(DPGSIM) --generator $* --mode sim 2>&1 | tee dpgsim.log

.PRECIOUS: %/AliESDs.root %/TRD.Digits.root
%/AliESDs.root %/TRD.Digits.root: %/galice.root
	cd $(dir $@) &&  $(DPGSIM) --generator $* --mode rec 2>&1 | tee dpgrec.log

.PRECIOUS: %/digitsqa.root
%/digitsqa.root: %/TRD.Digits.root digitsqa.C
	cd $(dir $@) &&  aliroot -b -q -l $(PWD)/digitsqa.C

.PRECIOUS: %/OCDBsim.root %/OCDBrec.root
#%/OCDBsim.root %/OCDBrec.root: OCDBsim.root OCDBrec.root
%/OCDBsim.root %/OCDBrec.root:
	mkdir -p $(dir $@)
	ln -sf $(PWD)/OCDBsim.root $(dir $@)/.
	ln -sf $(PWD)/OCDBrec.root $(dir $@)/.






help:
	@echo Targets:
	@for i in $(OBJECTS); do echo "    $$i"; done

# $(OBJECTS): OCDBsim.root OCDBrec.root
# 	$(eval GEN := $(subst /,:,$@))
# 	@echo "=== Running test simulation run ${RUN} with generator ${GEN} ==="
# 	@mkdir -p $@
# 	@ln -s $(PWD)/OCDBsim.root $@/.
# 	@ln -s $(PWD)/OCDBrec.root $@/.
# #	@cd $@ && $(ALIDPG_ROOT)/bin/aliroot_dpgsim.sh --run ${RUN} --generator ${GEN} --mode ${MODE} --nevents ${NEVENTS} ${EXTRA} &> dpgsim.log && echo " [SUCCESS] Running test simulation run ${RUN} with generator ${GEN}" || echo " [FAILURE] Running test simulation run ${RUN} with generator ${GEN}"
# 	@cd $@ && $(ALIDPG_ROOT)/bin/aliroot_dpgsim.sh --run ${RUN} --reconstruction ${REC} --generator ${GEN} --mode ${MODE} --nevents ${NEVENTS} ${EXTRA} &> dpgsim.log && echo " [SUCCESS] Running test simulation run ${RUN} with generator ${GEN}" || echo " [FAILURE] Running test simulation run ${RUN} with generator ${GEN}"

.PRECIOUS: OCDBsim.root OCDBrec.root
OCDBsim.root:
	@echo "=== Running OCDB snapshot creation for run ${RUN} ==="
	@$(ALIDPG_ROOT)/bin/aliroot_dpgsim.sh --run ${RUN} --mode ocdb &> snapshot.log && echo " [SUCCESS] Running OCDB snapshot creation for run ${RUN}" || echo " [FAILURE] Running OCDB snapshot creation for run ${RUN}"

OCDBrec.root:
	@echo "=== Running OCDB snapshot creation for run ${RUN} ==="
	@$(ALIDPG_ROOT)/bin/aliroot_dpgsim.sh --run ${RUN} --mode ocdb &> snapshot.log && echo " [SUCCESS] Running OCDB snapshot creation for run ${RUN}" || echo " [FAILURE] Running OCDB snapshot creation for run ${RUN}"

clean:
	rm -rf *~ *.log validation_error.message *.xml $(OBJECTS)
