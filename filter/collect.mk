#!/usr/bin/make -f

GRIDUSER = tdietel
ALIENDIR = /alice/cern.ch/user/t/tdietel/DigitsFilter-20190904-020946/output/
FILENAME = DigitsFilter.root

TARGETS = $(shell alien_find $(ALIENDIR) $(FILENAME) | grep $(FILENAME) \
| sed 's/.*\/output\///')

all: $(TARGETS)

DigitsFilter.root: $(TARGETS)
	hadd $@ $^

test:
	echo $(TARGETS)

%/$(FILENAME):
	mkdir -p $(dir $@)
	alien_cp alien:$(ALIENDIR)/$@ $@




#
# #alien-token-init $GRIDUSER
#
# for i in $(alien_find $ALIENDIR $FILENAME | grep $FILENAME)
# do
#   echo "-------------------------"
#   echo $i
#   AAA=$(echo $i | sed 's/.*\/output\///' | sed 's/DigitsFilter.root//')
#   echo $AAA
#   mkdir -p $AAA
#   if [[ ! -f $AAA/$FILENAME ]]; then
#     echo "NEED TO GET"
#     alien_cp alien:$i $AAA/$FILENAME
#   fi
# done
