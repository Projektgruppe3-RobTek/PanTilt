#compiles vhdl code.
#May not work for all types of code, but provided example should work
#You must have our top module named the same as specified in the
#program variable, in this example main.


PROGRAM=main
PART=xc3s500e-fg320-4


TOOL_PATH_= $(shell locate ISE_DS/ISE/bin/lin64/xst)
TOOL_PATH=$(TOOL_PATH_:%xst=%)

XST = ${TOOL_PATH}xst
NGDBUILD = ${TOOL_PATH}ngdbuild
MAP = ${TOOL_PATH}map
PAR = ${TOOL_PATH}par
BITGEN = ${TOOL_PATH}bitgen

define XST_FILE_OPTIONS
set -tmpdir "xst/projnav.tmp"
set -xsthdpdir "xst"
run
-ifn $(PROGRAM).prj
-ifmt mixed
-ofn $(PROGRAM)
-ofmt NGC
-p $(PART)
-top $(PROGRAM)
-opt_mode Speed
-opt_level 1
-iuc NO
-keep_hierarchy No
-netlist_hierarchy As_Optimized
-rtlview Yes
-glob_opt AllClockNets
-read_cores YES
-write_timing_constraints NO
-cross_clock_analysis NO
-hierarchy_separator /
-bus_delimiter <>
-case Maintain
-slice_utilization_ratio 100
-bram_utilization_ratio 100
-verilog2001 YES
-fsm_extract YES -fsm_encoding Auto
-safe_implementation No
-fsm_style LUT
-ram_extract Yes
-ram_style Auto
-rom_extract Yes
-mux_style Auto
-decoder_extract YES
-priority_extract Yes
-shreg_extract YES
-shift_extract YES
-xor_collapse YES
-rom_style Auto
-auto_bram_packing NO
-mux_extract Yes
-resource_sharing YES
-async_to_sync NO
-mult_style Auto
-iobuf YES
-max_fanout 500
-bufg 24
-register_duplication YES
-register_balancing No
-slice_packing YES
-optimize_primitives NO
-use_clock_enable Yes
-use_sync_set Yes
-use_sync_reset Yes
-iob Auto
-equivalent_register_removal YES
-slice_utilization_ratio_maxmargin 5
endef

export XST_FILE_OPTIONS


all: $(PROGRAM)_PROGRAM.bit


$(PROGRAM).xst:
	echo "$$XST_FILE_OPTIONS" > $(PROGRAM).xst

$(PROGRAM).ngc: $(PROGRAM).xst
	mkdir -p xst
	mkdir -p xst/projnav.tmp
	$(XST) -intstyle ise -ifn $(PROGRAM).xst -ofn $(PROGRAM).syr

$(PROGRAM).ngd: $(PROGRAM).ngc
	$(NGDBUILD) -p $(PART) -uc $(PROGRAM).ucf $(PROGRAM).ngc

$(PROGRAM).ncd: $(PROGRAM).ngd
	$(MAP) -detail -pr b $(PROGRAM).ngd

parout.ncd: $(PROGRAM).ncd
	$(PAR) -w $(PROGRAM).ncd parout.ncd $(PROGRAM).pcf

$(PROGRAM)_FLASH.bit: parout.ncd
	$(BITGEN) -w -g StartUpClk:CCLK -g CRC:Enable parout.ncd $(PROGRAM)_FLASH.bit $(PROGRAM).pcf

$(PROGRAM)_PROGRAM.bit: parout.ncd
	$(BITGEN) -w -g StartUpClk:JTAGCLK -g CRC:Enable parout.ncd $(PROGRAM)_PROGRAM.bit $(PROGRAM).pcf
	
program: $(PROGRAM)_PROGRAM.bit
	sudo djtgcfg -d DOnbUsb prog -i 0 -f $(PROGRAM)_PROGRAM.bit

flash: $(PROGRAM)_FLASH.bit
	sudo djtgcfg -d DOnbUsb prog -i 1 -f $(PROGRAM)_FLASH.bit

clean:
	rm -rf xst
	rm -f *.ngc
	rm -f *.srp
	rm -f *.xrpt
	rm -f *.bgn
	rm -f *.bld
	rm -f *.drc
	rm -f *.par
	rm -f *.pad
	rm -f *.xpi
	rm -f *.map
	rm -f *.mrp
	rm -f *.ncd
	rm -f *.ngd
	rm -f *.ngm
	rm -f *.bit
	rm -f *.pcf
	rm -f *.twr
	rm -f parout*
	rm -f *.lst
	rm -f *.unroutes
	rm -f *.xml
	rm -rf xlnx_auto_0_xdb
	rm -f $(PROGRAM).xst
	rm -f $(PROGRAM).lso
	rm -f *.xwbt
	rm -f *.log
	rm -f *.html
	rm -f *.syr
	rm -f *.ngr
	rm -f _xmsgs -R

