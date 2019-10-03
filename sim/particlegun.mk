RUN     = 277312
NEVENTS = 10

SIMDIR ?= ../

.PHONY: all
all: AliESDs.root digitsqa.root DigitsTask.root


GeneratorCustom.C: $(SIMDIR)/ParticleGun.C ; cp $< $@
ReconstructionCustom.C: $(SIMDIR)/RecTRDdigits.C ; cp $< $@

%.C: $(SIMDIR)/%.C ; cp $< $@

galice.root: OCDBsim.root GeneratorCustom.C
	rm -f *.RecPoints.root *.Hits.root *.Digits.root *.SDigits.root
	$(ALIDPG_ROOT)/bin/aliroot_dpgsim.sh --mode sim \
	--run $(RUN) --generator Custom --nevents $(NEVENTS)
	ls -l > ls-sim.txt

# This is a trick to avoid running reconstruction multiple times
# taken from: https://stackoverflow.com/questions/19822435/multiple-targets-from-one-recipe-and-parallel-execution
.INTERMEDIATE: rec.log
.PRECIOUS: rec.log
AliESDs.root: rec.log
TRD.Digits.root: rec.log

rec.log: galice.root OCDBrec.root ReconstructionCustom.C
	$(ALIDPG_ROOT)/bin/aliroot_dpgsim.sh --nocleanup --mode rec \
	--run $(RUN) --reconstruction Custom --nevents $(NEVENTS)
	ls -l > ls-rec.txt

.PRECIOUS: digitsqa.root
digitsqa.root: $(SIMDIR)/digitsqa.C TRD.Digits.root
	aliroot -b -q -l $(SIMDIR)/digitsqa.C

.PRECIOUS: DigitsTask.root
.INTERMEDIATE: train.log
DigitsTask.root: train.log

train.log: $(SIMDIR)/train.C AddDigitsTask.C AliESDs.root TRD.Digits.root
	aliroot -b -q -l $(SIMDIR)/train.C 2>&1 | tee train.log


.PRECIOUS: OCDBsim.root OCDBrec.root
OCDBsim.root: ;	ln -sf ../ocdb/$(RUN)/$@ $@
OCDBrec.root: ;	ln -sf ../ocdb/$(RUN)/$@ $@

clean:
	rm -f *.root *.log *.dat *.ps *.pcm grpdump.sh validation_error.message
	rm -rf GRP
