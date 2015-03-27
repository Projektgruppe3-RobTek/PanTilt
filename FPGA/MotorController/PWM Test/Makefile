#compiles vhdl code.
#May not work for all types of code, but provided example should work
#You must have our top module named the same as specified in the
#program variable, in this example main.


PROGRAM=main
PART=xc3s500e-fg320-4
BUILDDIR=build
LOGDIR=log
INTSTYLE = silent
TOOL_PATH_= $(shell locate Xilinx/14.7/ISE_DS/ISE/bin/lin64/xst)
TOOL_PATH=$(TOOL_PATH_:%xst=%)

XST = ${TOOL_PATH}xst
NGDBUILD = ${TOOL_PATH}ngdbuild
MAP = ${TOOL_PATH}map
PAR = ${TOOL_PATH}par
BITGEN = ${TOOL_PATH}bitgen

define XST_FILE_OPTIONS
set -tmpdir "${BUILDDIR}/xst/projnav.tmp"
set -xsthdpdir "${BUILDDIR}/xst"
run
-ifn ${PROGRAM}.prj
-ifmt mixed
-ofn ${BUILDDIR}/${PROGRAM}
-ofmt NGC
-p ${PART}
-top ${PROGRAM}
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


all: ${PROGRAM}_PROGRAM.bit

${BUILDDIR}/${PROGRAM}.xst:
	@mkdir -p ${BUILDDIR}
	@mkdir -p ${LOGDIR}
	echo "$$XST_FILE_OPTIONS" > ${BUILDDIR}/${PROGRAM}.xst

 ${BUILDDIR}/${PROGRAM}.ngc:  ${BUILDDIR}/${PROGRAM}.xst
	@mkdir -p ${BUILDDIR}/xst
	@mkdir -p ${BUILDDIR}/xst/projnav.tmp
	${XST} -intstyle ${INTSTYLE} -ifn  ${BUILDDIR}/${PROGRAM}.xst -ofn  ${BUILDDIR}/${PROGRAM}.syr
	@mv -f ${PROGRAM}.lso ${LOGDIR}/${PROGRAM}.lso 2>/dev/null; true

 ${BUILDDIR}/${PROGRAM}.ngd:  ${BUILDDIR}/${PROGRAM}.ngc
	${NGDBUILD} -p ${PART} -uc ${PROGRAM}.ucf  ${BUILDDIR}/${PROGRAM}.ngc ${BUILDDIR}/${PROGRAM}.ngd  -dd ${BUILDDIR}/ -intstyle ${INTSTYLE}
	@rm ${BUILDDIR}/xlnx_auto_0_xdb/ -rf 2>/dev/null; true
	@mv -f xlnx_auto_0_xdb/ ${BUILDDIR}/xlnx_auto_0_xdb/ 2>/dev/null; true


 ${BUILDDIR}/${PROGRAM}.ncd:  ${BUILDDIR}/${PROGRAM}.ngd
	${MAP} -pr b  ${BUILDDIR}/${PROGRAM}.ngd  -intstyle ${INTSTYLE}
	@mv -f xilinx_device_details.xml ${BUILDDIR}/xilinx_device_details.xml 2>/dev/null; true
	@mv -f main_map.xrpt ${BUILDDIR}/main_map.xrpt 2>/dev/null; true

${BUILDDIR}/parout.ncd:  ${BUILDDIR}/${PROGRAM}.ncd
	${PAR} -w  ${BUILDDIR}/${PROGRAM}.ncd ${BUILDDIR}/parout.ncd ${BUILDDIR}/${PROGRAM}.pcf -intstyle ${INTSTYLE}
	@mv -f main_par.xrpt ${BUILDDIR}/main_par.xrpt 2>/dev/null; true

${PROGRAM}_FLASH.bit: ${BUILDDIR}/parout.ncd
	${BITGEN} -w -g StartUpClk:CCLK -g CRC:Enable ${BUILDDIR}/parout.ncd ${PROGRAM}_FLASH.bit ${BUILDDIR}/${PROGRAM}.pcf -intstyle ${INTSTYLE}
	@mv -f ${PROGRAM}_FLASH.bgn ${BUILDDIR}/${PROGRAM}_FLASH.bgn 2>/dev/null; true
	@mv -f ${PROGRAM}_FLASH.drc ${BUILDDIR}/${PROGRAM}_FLASH.drc 2>/dev/null; true
	@mv -f ${PROGRAM}_FLASH_bitgen.xwbt  ${BUILDDIR}/${PROGRAM}_PROGRAM_FLASH.xwbt 2>/dev/null; true
	@mv -f xilinx_device_details.xml ${LOGDIR}/xilinx_device_details.xml 2>/dev/null; true
	@mv -f webtalk.log ${LOGDIR}/webtalk.log 2>/dev/null; true
	@mv -f usage_statistics_webtalk.html ${LOGDIR}/usage_statistics_webtalk.html 2>/dev/null; true


${PROGRAM}_PROGRAM.bit: ${BUILDDIR}/parout.ncd
	${BITGEN} -w -g StartUpClk:JTAGCLK -g CRC:Enable ${BUILDDIR}/parout.ncd ${PROGRAM}_PROGRAM.bit ${BUILDDIR}/${PROGRAM}.pcf -intstyle ${INTSTYLE}
	@mv -f ${PROGRAM}_PROGRAM.bgn ${BUILDDIR}/${PROGRAM}_PROGRAM.bgn 2>/dev/null; true
	@mv -f ${PROGRAM}_PROGRAM.drc ${BUILDDIR}/${PROGRAM}_PROGRAM.drc 2>/dev/null; true
	@mv -f ${PROGRAM}_PROGRAM_bitgen.xwbt  ${BUILDDIR}/${PROGRAM}_PROGRAM_bitgen.xwbt 2>/dev/null; true
	@mv -f xilinx_device_details.xml ${LOGDIR}/xilinx_device_details.xml 2>/dev/null; true
	@mv -f webtalk.log ${LOGDIR}/webtalk.log 2>/dev/null; true
	@mv -f usage_statistics_webtalk.html ${LOGDIR}/usage_statistics_webtalk.html 2>/dev/null; true


program: ${PROGRAM}_PROGRAM.bit
	sudo djtgcfg -d DOnbUsb prog -i 0 -f ${PROGRAM}_PROGRAM.bit


flash: ${PROGRAM}_FLASH.bit
	sudo djtgcfg -d DOnbUsb prog -i 1 -f ${PROGRAM}_FLASH.bit

clean:
	rm -rf ${BUILDDIR}
	rm -rf ${LOGDIR}
	rm -rf _xmsgs
	rm -rf xlnx_auto_0_xdb
	rm -rf ${PROGRAM}.lso
	rm -rf xilinx_device_details.xml
	rm -rf main_map.xrpt
	rm -rf main_par.xrpt
	rm -rf *.bgn
	rm -rf *.drc
	rm -rf *.xwbt
	rm -rf *.bit
	rm -rf usage_statistics_webtalk.html
	rm -rf *.log
