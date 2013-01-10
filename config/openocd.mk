OPENOCD      = openocd


# debug level (d0..d3)
oocd_params  = -d1

# interface and board/target settings (using the OOCD target-library here)
# oocd_params        += -c "fast enable"
# oocd_params        += -f interface/arm-usb-ocd.cfg -f board/stm32f10x_128k_eval.cfg
oocd_params         += -f interface/olimex-arm-usb-ocd.cfg -f target/stm32f1x.cfg
oocd_params         += -c init -c targets

oocd_params_program  = $(oocd_params)
oocd_params_program += -c "reset halt"
oocd_params_program += -c "flash write_image erase $(ELF)" -c "verify_image $(ELF)"
oocd_params_program += -c "reset run"
oocd_params_program += -c "shutdown"   # terminate OOCD after programming

oocd_params_reset    = $(oocd_params)
oocd_params_reset   += -c "reset run"
oocd_params_reset   += -c "shutdown"

oocd_params_debug    = $(oocd_params)
oocd_gdb_port        = 3333
oocd_params_debug   += -c "gdb_port $(oocd_gdb_port)"
oocd_params_debug   += -c "reset halt"


.PHONY: program flash reset debug

program: $(ELF)
	@echo "--- Programming with OPENOCD"
	$(OPENOCD) $(oocd_params_program)

flash: program

reset:
	@echo "--- Resetting device"
	$(OPENOCD) $(oocd_params_reset)

debug:
# Note on gdb-target.xml:
# OpenOCD does not provide an XML target description
# (qXfer:features:read) to GDB, resulting in an error: "Remote 'g'
# packet reply is too long". As a workaround, we provide a target
# description XML file directly to GDB using the following GDB
# command: set tdesc filename <file name>
	@echo Starting OpenOCD debugger
	@echo "*"
	@echo "* $(CROSS)gdb -ex \"set tdesc filename gdb-target.xml\" -ex \"target remote localhost:$(oocd_gdb_port)\" $(ELF)"
	@echo "*"
	$(OPENOCD) $(oocd_params_debug)
