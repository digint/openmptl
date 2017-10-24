#OUTPUT_DIRECTORY = /tmp/doxygen
OUTPUT_DIRECTORY =

DOCS = openmptl \
       lib \
       drivers \
       arm_cortex_common \
       stm32_common \
       stm32f10x \
       stm32f4

DOXYGEN = doxygen

all: $(DOCS)

$(DOCS):
	@echo "*** Checking $@.conf"
	grep -q -e "^HTML_OUTPUT\s*=\s*$@" $@.conf
	@echo "*** Building $@"
	( cat $@.conf ; echo "OUTPUT_DIRECTORY=$(OUTPUT_DIRECTORY)" ) | $(DOXYGEN) -

clean:
	rm -f *.tag
