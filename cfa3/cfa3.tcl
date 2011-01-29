
########## Tcl recorder starts at 01/28/11 12:26:35 ##########

set version "1.4"
set proj_dir "C:/hw/cfa3"
cd $proj_dir

# Get directory paths
set pver $version
regsub -all {\.} $pver {_} pver
set lscfile "lsc_"
append lscfile $pver ".ini"
set lsvini_dir [lindex [array get env LSC_INI_PATH] 1]
set lsvini_path [file join $lsvini_dir $lscfile]
if {[catch {set fid [open $lsvini_path]} msg]} {
	 puts "File Open Error: $lsvini_path"
	 return false
} else {set data [read $fid]; close $fid }
foreach line [split $data '\n'] { 
	set lline [string tolower $line]
	set lline [string trim $lline]
	if {[string compare $lline "\[paths\]"] == 0} { set path 1; continue}
	if {$path && [regexp {^\[} $lline]} {set path 0; break}
	if {$path && [regexp {^bin} $lline]} {set cpld_bin $line; continue}
	if {$path && [regexp {^fpgapath} $lline]} {set fpga_dir $line; continue}
	if {$path && [regexp {^fpgabinpath} $lline]} {set fpga_bin $line}}

set cpld_bin [string range $cpld_bin [expr [string first "=" $cpld_bin]+1] end]
regsub -all "\"" $cpld_bin "" cpld_bin
set cpld_bin [file join $cpld_bin]
set install_dir [string range $cpld_bin 0 [expr [string first "ispcpld" $cpld_bin]-2]]
regsub -all "\"" $install_dir "" install_dir
set install_dir [file join $install_dir]
set fpga_dir [string range $fpga_dir [expr [string first "=" $fpga_dir]+1] end]
regsub -all "\"" $fpga_dir "" fpga_dir
set fpga_dir [file join $fpga_dir]
set fpga_bin [string range $fpga_bin [expr [string first "=" $fpga_bin]+1] end]
regsub -all "\"" $fpga_bin "" fpga_bin
set fpga_bin [file join $fpga_bin]

if {[string match "*$fpga_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$fpga_bin;$env(PATH)" }

if {[string match "*$cpld_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$cpld_bin;$env(PATH)" }

lappend auto_path [file join $install_dir "ispcpld" "tcltk" "lib" "ispwidget" "runproc"]
package require runcmd

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" top.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 12:26:35 ###########


########## Tcl recorder starts at 01/28/11 19:23:46 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" top.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 19:23:46 ###########


########## Tcl recorder starts at 01/28/11 19:44:29 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" top.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 19:44:29 ###########


########## Tcl recorder starts at 01/28/11 19:45:34 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" top.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 19:45:34 ###########


########## Tcl recorder starts at 01/28/11 19:56:08 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" top.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 19:56:08 ###########


########## Tcl recorder starts at 01/28/11 19:56:19 ##########

# Commands to make the Process: 
# JEDEC File
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup top.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"top.bls\" -o \"top.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i top.bl0 -o top.bl1 -collapse none -reduce none  -err automake.err -keepwires -family"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"top.bl1\" -o \"cfa3.bl2\" -omod \"cfa3\"  -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/impsrc\"  -prj cfa3 -lci cfa3.lct -log cfa3.imp -err automake.err -tti cfa3.bl2 -dir $proj_dir"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -blifopt cfa3.b2_"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" cfa3.bl2 -sweep -mergefb -err automake.err -o cfa3.bl3 @cfa3.b2_ "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -dev lc4k -diofft cfa3.d0"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" cfa3.bl3 -family AMDMACH -idev van -o cfa3.bl4 -oxrf cfa3.xrf -err automake.err @cfa3.d0 "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -dev lc4k -prefit cfa3.l0"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/prefit\" -blif -inp cfa3.bl4 -out cfa3.bl5 -err automake.err -log cfa3.log -mod top @cfa3.l0  -sc"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [catch {open cfa3.rs1 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs1: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -nojed -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [catch {open cfa3.rs2 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs2: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/lpf4k\" \"@cfa3.rs2\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete cfa3.rs1
file delete cfa3.rs2
if [runCmd "\"$cpld_bin/tda\" -i cfa3.bl5 -o cfa3.tda -lci cfa3.lct -dev m4s_64_64 -family lc4k -mod top -ovec NoInput.tmv -err tda.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj cfa3 -if cfa3.jed -j2s -log cfa3.svl "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 19:56:19 ###########


########## Tcl recorder starts at 01/28/11 19:57:05 ##########

# Commands to make the Process: 
# Constraint Editor
if [runCmd "\"$cpld_bin/blifstat\" -i cfa3.bl5 -o cfa3.sif"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Constraint Editor
if [catch {open lattice_cmd.rs2 w} rspFile] {
	puts stderr "Cannot create response file lattice_cmd.rs2: $rspFile"
} else {
	puts $rspFile "-nodal -src cfa3.bl5 -type BLIF -presrc cfa3.bl3 -crf cfa3.crf -sif cfa3.sif -devfile \"$install_dir/ispcpld/dat/lc4k/m4s_64_64.dev\" -lci cfa3.lct
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/lciedit\" @lattice_cmd.rs2"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 19:57:05 ###########


########## Tcl recorder starts at 01/28/11 20:04:19 ##########

# Commands to make the Process: 
# ABEL Test Vector Template
if [runCmd "\"$cpld_bin/blif2eqn\" top.bl0 -o top.abt -testfix -template \"$install_dir/ispcpld/plsi/abel/plsiabt.tft\" -prj cfa3 -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:04:19 ###########


########## Tcl recorder starts at 01/28/11 20:04:47 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open cfa3.rs1 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs1: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -nojed -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [catch {open cfa3.rs2 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs2: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/lpf4k\" \"@cfa3.rs2\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete cfa3.rs1
file delete cfa3.rs2
if [runCmd "\"$cpld_bin/tda\" -i cfa3.bl5 -o cfa3.tda -lci cfa3.lct -dev m4s_64_64 -family lc4k -mod top -ovec NoInput.tmv -err tda.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj cfa3 -if cfa3.jed -j2s -log cfa3.svl "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:04:47 ###########


########## Tcl recorder starts at 01/28/11 20:05:10 ##########

# Commands to make the Process: 
# ISC-1532 File
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj cfa3 -if cfa3.jed -j2i "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:05:10 ###########


########## Tcl recorder starts at 01/28/11 20:05:22 ##########

# Commands to make the Process: 
# Post-Fit Re-Compile
# - none -
# Application to view the Process: 
# Post-Fit Re-Compile
if [catch {open lattice_cmd.rs2 w} rspFile] {
	puts stderr "Cannot create response file lattice_cmd.rs2: $rspFile"
} else {
	puts $rspFile "-src cfa3.bl5 -type BLIF -devfile \"$install_dir/ispcpld/dat/lc4k/m4s_64_64.dev\" -lci cfa3.lct -prc cfa3.lco -log cfa3.log -touch cfa3.fti
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/lciedit\" @lattice_cmd.rs2"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:05:22 ###########


########## Tcl recorder starts at 01/28/11 20:06:35 ##########

# Commands to make the Process: 
# JEDEC File
if [runCmd "\"$cpld_bin/tda\" -i cfa3.bl5 -o cfa3.tda -lci cfa3.lct -dev m4s_64_64 -family lc4k -mod top -ovec NoInput.tmv -err tda.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj cfa3 -if cfa3.jed -j2s -log cfa3.svl "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:06:35 ###########


########## Tcl recorder starts at 01/28/11 20:13:00 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i top.bl0 -o top.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi top.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:13:00 ###########


########## Tcl recorder starts at 01/28/11 20:17:46 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:17:47 ###########


########## Tcl recorder starts at 01/28/11 20:19:48 ##########

# Commands to make the Process: 
# Constraint Editor
if [runCmd "\"$cpld_bin/blifstat\" -i cfa3.bl5 -o cfa3.sif"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Constraint Editor
if [catch {open lattice_cmd.rs2 w} rspFile] {
	puts stderr "Cannot create response file lattice_cmd.rs2: $rspFile"
} else {
	puts $rspFile "-nodal -src cfa3.bl5 -type BLIF -presrc cfa3.bl3 -crf cfa3.crf -sif cfa3.sif -devfile \"$install_dir/ispcpld/dat/lc4k/m4s_64_64.dev\" -lci cfa3.lct
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/lciedit\" @lattice_cmd.rs2"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:19:48 ###########


########## Tcl recorder starts at 01/28/11 20:20:04 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open cfa3.rs1 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs1: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -nojed -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [catch {open cfa3.rs2 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs2: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/lpf4k\" \"@cfa3.rs2\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete cfa3.rs1
file delete cfa3.rs2
if [runCmd "\"$cpld_bin/tda\" -i cfa3.bl5 -o cfa3.tda -lci cfa3.lct -dev m4s_64_64 -family lc4k -mod top -ovec NoInput.tmv -err tda.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj cfa3 -if cfa3.jed -j2s -log cfa3.svl "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:20:04 ###########


########## Tcl recorder starts at 01/28/11 20:20:10 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:20:10 ###########


########## Tcl recorder starts at 01/28/11 20:25:10 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:25:10 ###########


########## Tcl recorder starts at 01/28/11 20:27:32 ##########

# Commands to make the Process: 
# JEDEC File
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup top.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"top.bls\" -o \"top.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i top.bl0 -o top.bl1 -collapse none -reduce none  -err automake.err -keepwires -family"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"top.bl1\" -o \"cfa3.bl2\" -omod \"cfa3\"  -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/impsrc\"  -prj cfa3 -lci cfa3.lct -log cfa3.imp -err automake.err -tti cfa3.bl2 -dir $proj_dir"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -blifopt cfa3.b2_"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" cfa3.bl2 -sweep -mergefb -err automake.err -o cfa3.bl3 @cfa3.b2_ "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -dev lc4k -diofft cfa3.d0"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" cfa3.bl3 -family AMDMACH -idev van -o cfa3.bl4 -oxrf cfa3.xrf -err automake.err @cfa3.d0 "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -dev lc4k -prefit cfa3.l0"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/prefit\" -blif -inp cfa3.bl4 -out cfa3.bl5 -err automake.err -log cfa3.log -mod top @cfa3.l0  -sc"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [catch {open cfa3.rs1 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs1: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -nojed -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [catch {open cfa3.rs2 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs2: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/lpf4k\" \"@cfa3.rs2\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete cfa3.rs1
file delete cfa3.rs2
if [runCmd "\"$cpld_bin/tda\" -i cfa3.bl5 -o cfa3.tda -lci cfa3.lct -dev m4s_64_64 -family lc4k -mod top -ovec NoInput.tmv -err tda.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj cfa3 -if cfa3.jed -j2s -log cfa3.svl "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:27:32 ###########


########## Tcl recorder starts at 01/28/11 20:28:56 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:28:56 ###########


########## Tcl recorder starts at 01/28/11 20:31:47 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:31:47 ###########


########## Tcl recorder starts at 01/28/11 20:32:04 ##########

# Commands to make the Process: 
# Bus Signal Cross Reference
if [runCmd "\"$cpld_bin/vhdl\" -s -lib=lat_vhd rplytest.sch"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/exfgen\" -vhd rplytest.vhd -out rplytest.exf"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:32:04 ###########


########## Tcl recorder starts at 01/28/11 20:32:16 ##########

# Commands to make the Process: 
# Generate Schematic Symbol
if [runCmd "\"$cpld_bin/naf2sym\" rplytest"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:32:16 ###########


########## Tcl recorder starts at 01/28/11 20:32:21 ##########

# Commands to make the Process: 
# Compiled Equations
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/blif2eqn\" rplytest.bl0 -o rplytest.eq0  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:32:21 ###########


########## Tcl recorder starts at 01/28/11 20:32:44 ##########

# Commands to make the Process: 
# Compiled Equations
if [runCmd "\"$cpld_bin/blif2eqn\" top.bl0 -o top.eq0  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:32:44 ###########


########## Tcl recorder starts at 01/28/11 20:36:43 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt test1.wdl -network=rplytest -module=rplytest"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:36:43 ###########


########## Tcl recorder starts at 01/28/11 20:37:48 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:37:48 ###########


########## Tcl recorder starts at 01/28/11 20:37:55 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:37:55 ###########


########## Tcl recorder starts at 01/28/11 20:39:30 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:39:30 ###########


########## Tcl recorder starts at 01/28/11 20:39:32 ##########

# Commands to make the Process: 
# Compile Schematic
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:39:33 ###########


########## Tcl recorder starts at 01/28/11 20:39:36 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:39:36 ###########


########## Tcl recorder starts at 01/28/11 20:40:01 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt test1.wdl -network=rplytest -module=rplytest"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:40:01 ###########


########## Tcl recorder starts at 01/28/11 20:40:12 ##########

# Commands to make the Process: 
# Compiled Equations
if [runCmd "\"$cpld_bin/blif2eqn\" rplytest.bl0 -o rplytest.eq0  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:40:12 ###########


########## Tcl recorder starts at 01/28/11 20:41:35 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:41:36 ###########


########## Tcl recorder starts at 01/28/11 20:41:38 ##########

# Commands to make the Process: 
# Generate Schematic Symbol
if [runCmd "\"$cpld_bin/naf2sym\" rplytest"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:41:38 ###########


########## Tcl recorder starts at 01/28/11 20:41:43 ##########

# Commands to make the Process: 
# Compile Schematic
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:41:43 ###########


########## Tcl recorder starts at 01/28/11 20:41:48 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:41:48 ###########


########## Tcl recorder starts at 01/28/11 20:43:05 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt test1.wdl -network=rplytest -module=rplytest"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:43:05 ###########


########## Tcl recorder starts at 01/28/11 20:49:40 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:49:40 ###########


########## Tcl recorder starts at 01/28/11 20:49:47 ##########

# Commands to make the Process: 
# Compile Schematic
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:49:47 ###########


########## Tcl recorder starts at 01/28/11 20:49:51 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:49:51 ###########


########## Tcl recorder starts at 01/28/11 20:50:57 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:50:57 ###########


########## Tcl recorder starts at 01/28/11 20:51:07 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:51:07 ###########


########## Tcl recorder starts at 01/28/11 20:53:14 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt test1.wdl -network=rplytest -module=rplytest"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:53:14 ###########


########## Tcl recorder starts at 01/28/11 20:54:46 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt test1.wdl -network=rplytest -module=rplytest"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:54:46 ###########


########## Tcl recorder starts at 01/28/11 20:55:35 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:55:35 ###########


########## Tcl recorder starts at 01/28/11 20:56:03 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt test1.wdl -network=rplytest -module=rplytest"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:56:03 ###########


########## Tcl recorder starts at 01/28/11 20:56:56 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:56:56 ###########


########## Tcl recorder starts at 01/28/11 20:57:08 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:57:08 ###########


########## Tcl recorder starts at 01/28/11 20:58:46 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:58:46 ###########


########## Tcl recorder starts at 01/28/11 20:58:54 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 20:58:54 ###########


########## Tcl recorder starts at 01/28/11 21:01:28 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:01:28 ###########


########## Tcl recorder starts at 01/28/11 21:01:41 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:01:41 ###########


########## Tcl recorder starts at 01/28/11 21:03:05 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt test1.wdl -network=rplytest -module=rplytest"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:03:05 ###########


########## Tcl recorder starts at 01/28/11 21:04:25 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt test1.wdl -network=rplytest -module=rplytest"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:04:25 ###########


########## Tcl recorder starts at 01/28/11 21:07:25 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:07:25 ###########


########## Tcl recorder starts at 01/28/11 21:08:30 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:08:30 ###########


########## Tcl recorder starts at 01/28/11 21:10:38 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:10:38 ###########


########## Tcl recorder starts at 01/28/11 21:10:44 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:10:44 ###########


########## Tcl recorder starts at 01/28/11 21:11:18 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt test1.wdl -network=rplytest -module=rplytest"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:11:18 ###########


########## Tcl recorder starts at 01/28/11 21:12:46 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:12:46 ###########


########## Tcl recorder starts at 01/28/11 21:12:55 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:12:55 ###########


########## Tcl recorder starts at 01/28/11 21:14:52 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:14:52 ###########


########## Tcl recorder starts at 01/28/11 21:14:56 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:14:56 ###########


########## Tcl recorder starts at 01/28/11 21:18:05 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:18:05 ###########


########## Tcl recorder starts at 01/28/11 21:18:11 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup rplytest.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"rplytest.bls\" -o \"rplytest.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i rplytest.bl0 -o rplytest.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"rplytest.blo\" -o \"rplytest.blh\" -omod \"rplytest\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" rplytest.blh -o rplytest.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i rplytest.bli -o rplytest.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi rplytest.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk test1.ltv -map rplytest.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:18:11 ###########


########## Tcl recorder starts at 01/28/11 21:28:55 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" top.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:28:55 ###########


########## Tcl recorder starts at 01/28/11 21:29:02 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup top.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"top.bls\" -o \"top.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i top.bl0 -o top.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi top.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:29:02 ###########


########## Tcl recorder starts at 01/28/11 21:29:42 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/sch2jhd\" top.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:29:42 ###########


########## Tcl recorder starts at 01/28/11 21:29:42 ##########

# Commands to make the Process: 
# JEDEC File
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup top.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"top.bls\" -o \"top.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i top.bl0 -o top.bl1 -collapse none -reduce none  -err automake.err -keepwires -family"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"top.bl1\" -o \"cfa3.bl2\" -omod \"cfa3\"  -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/impsrc\"  -prj cfa3 -lci cfa3.lct -log cfa3.imp -err automake.err -tti cfa3.bl2 -dir $proj_dir"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -blifopt cfa3.b2_"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" cfa3.bl2 -sweep -mergefb -err automake.err -o cfa3.bl3 @cfa3.b2_ "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -dev lc4k -diofft cfa3.d0"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" cfa3.bl3 -family AMDMACH -idev van -o cfa3.bl4 -oxrf cfa3.xrf -err automake.err @cfa3.d0 "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -dev lc4k -prefit cfa3.l0"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/prefit\" -blif -inp cfa3.bl4 -out cfa3.bl5 -err automake.err -log cfa3.log -mod top @cfa3.l0  -sc"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [catch {open cfa3.rs1 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs1: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -nojed -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [catch {open cfa3.rs2 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs2: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/lpf4k\" \"@cfa3.rs2\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete cfa3.rs1
file delete cfa3.rs2
if [runCmd "\"$cpld_bin/tda\" -i cfa3.bl5 -o cfa3.tda -lci cfa3.lct -dev m4s_64_64 -family lc4k -mod top -ovec NoInput.tmv -err tda.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj cfa3 -if cfa3.jed -j2s -log cfa3.svl "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:29:42 ###########


########## Tcl recorder starts at 01/28/11 21:29:54 ##########

# Commands to make the Process: 
# Constraint Editor
if [runCmd "\"$cpld_bin/blifstat\" -i cfa3.bl5 -o cfa3.sif"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Constraint Editor
if [catch {open lattice_cmd.rs2 w} rspFile] {
	puts stderr "Cannot create response file lattice_cmd.rs2: $rspFile"
} else {
	puts $rspFile "-nodal -src cfa3.bl5 -type BLIF -presrc cfa3.bl3 -crf cfa3.crf -sif cfa3.sif -devfile \"$install_dir/ispcpld/dat/lc4k/m4s_64_64.dev\" -lci cfa3.lct
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/lciedit\" @lattice_cmd.rs2"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:29:54 ###########


########## Tcl recorder starts at 01/28/11 21:30:19 ##########

# Commands to make the Process: 
# JEDEC File
if [catch {open cfa3.rs1 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs1: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -nojed -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [catch {open cfa3.rs2 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs2: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/lpf4k\" \"@cfa3.rs2\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete cfa3.rs1
file delete cfa3.rs2
if [runCmd "\"$cpld_bin/tda\" -i cfa3.bl5 -o cfa3.tda -lci cfa3.lct -dev m4s_64_64 -family lc4k -mod top -ovec NoInput.tmv -err tda.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj cfa3 -if cfa3.jed -j2s -log cfa3.svl "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 21:30:19 ###########


########## Tcl recorder starts at 01/28/11 23:19:09 ##########

# Commands to make the Process: 
# JEDEC File
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup top.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"top.bls\" -o \"top.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i top.bl0 -o top.bl1 -collapse none -reduce none  -err automake.err -keepwires -family"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"top.bl1\" -o \"cfa3.bl2\" -omod \"cfa3\"  -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/impsrc\"  -prj cfa3 -lci cfa3.lct -log cfa3.imp -err automake.err -tti cfa3.bl2 -dir $proj_dir"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -blifopt cfa3.b2_"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" cfa3.bl2 -sweep -mergefb -err automake.err -o cfa3.bl3 @cfa3.b2_ "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -dev lc4k -diofft cfa3.d0"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" cfa3.bl3 -family AMDMACH -idev van -o cfa3.bl4 -oxrf cfa3.xrf -err automake.err @cfa3.d0 "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -dev lc4k -prefit cfa3.l0"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/prefit\" -blif -inp cfa3.bl4 -out cfa3.bl5 -err automake.err -log cfa3.log -mod top @cfa3.l0  -sc"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [catch {open cfa3.rs1 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs1: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -nojed -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [catch {open cfa3.rs2 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs2: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/lpf4k\" \"@cfa3.rs2\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete cfa3.rs1
file delete cfa3.rs2
if [runCmd "\"$cpld_bin/tda\" -i cfa3.bl5 -o cfa3.tda -lci cfa3.lct -dev m4s_64_64 -family lc4k -mod top -ovec NoInput.tmv -err tda.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj cfa3 -if cfa3.jed -j2s -log cfa3.svl "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 23:19:09 ###########


########## Tcl recorder starts at 01/28/11 23:25:14 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i top.bl0 -o top.blo  -keepwires -family -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/sch2tf\" -template \"$install_dir/ispcpld/plsi/latsim/plsi.tft\" -prj cfa3 -ext .lsi top.sch "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 23:25:14 ###########


########## Tcl recorder starts at 01/28/11 23:31:10 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 23:31:10 ###########


########## Tcl recorder starts at 01/28/11 23:34:33 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 23:34:33 ###########


########## Tcl recorder starts at 01/28/11 23:37:35 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 23:37:35 ###########


########## Tcl recorder starts at 01/28/11 23:40:27 ##########

# Commands to make the Process: 
# ABEL Test Vector Template
if [runCmd "\"$cpld_bin/blif2eqn\" top.bl0 -o top.abt -testfix -template \"$install_dir/ispcpld/plsi/abel/plsiabt.tft\" -prj cfa3 -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 23:40:27 ###########


########## Tcl recorder starts at 01/28/11 23:44:44 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 23:44:44 ###########


########## Tcl recorder starts at 01/28/11 23:50:02 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 23:50:02 ###########


########## Tcl recorder starts at 01/28/11 23:51:30 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 23:51:30 ###########


########## Tcl recorder starts at 01/28/11 23:54:30 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 23:54:30 ###########


########## Tcl recorder starts at 01/28/11 23:57:21 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 23:57:21 ###########


########## Tcl recorder starts at 01/28/11 23:59:55 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/28/11 23:59:55 ###########


########## Tcl recorder starts at 01/29/11 00:08:46 ##########

# Commands to make the Process: 
# Functional Simulation
if [runCmd "\"$cpld_bin/exp\" -lhdl -ext=.ltv -testpt testform.wdl -network=top -module=top"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"top.blo\" -o \"top.blh\" -omod \"top\" -err \"automake.err\" "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" top.blh -o top.bli -sweep -mergefb -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" -i top.bli -o top.blj -family AMDMACH -idev van -err automake.err -dev lc4k "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
# Application to view the Process: 
# Functional Simulation
if [catch {open simcp._sp w} rspFile] {
	puts stderr "Cannot create response file simcp._sp: $rspFile"
} else {
	puts $rspFile "simcp.pre1 -ini simcpls.ini -unit simcp.pre1
-cfg machgen.fdk testform.ltv -map top.lsi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/simcp\" @simcp._sp"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/29/11 00:08:46 ###########


########## Tcl recorder starts at 01/29/11 00:18:15 ##########

# Commands to make the Process: 
# JEDEC File
if [runCmd "\"$cpld_bin/sch2blf\" -dev Lattice -sup top.sch  -err automake.err"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"top.bls\" -o \"top.bl0\" -ipo  -family -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" -i top.bl0 -o top.bl1 -collapse none -reduce none  -err automake.err -keepwires -family"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblflink\" \"top.bl1\" -o \"cfa3.bl2\" -omod \"cfa3\"  -err \"automake.err\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/impsrc\"  -prj cfa3 -lci cfa3.lct -log cfa3.imp -err automake.err -tti cfa3.bl2 -dir $proj_dir"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -blifopt cfa3.b2_"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mblifopt\" cfa3.bl2 -sweep -mergefb -err automake.err -o cfa3.bl3 @cfa3.b2_ "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -dev lc4k -diofft cfa3.d0"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/mdiofft\" cfa3.bl3 -family AMDMACH -idev van -o cfa3.bl4 -oxrf cfa3.xrf -err automake.err @cfa3.d0 "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/abelvci\" -vci cfa3.lct -dev lc4k -prefit cfa3.l0"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/prefit\" -blif -inp cfa3.bl4 -out cfa3.bl5 -err automake.err -log cfa3.log -mod top @cfa3.l0  -sc"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [catch {open cfa3.rs1 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs1: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -nojed -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [catch {open cfa3.rs2 w} rspFile] {
	puts stderr "Cannot create response file cfa3.rs2: $rspFile"
} else {
	puts $rspFile "-i cfa3.bl5 -lci cfa3.lct -d m4s_64_64 -lco cfa3.lco -html_rpt -fti cfa3.fti -fmt PLA -tto cfa3.tt4 -eqn cfa3.eq3 -tmv NoInput.tmv
-rpt_num 1
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/lpf4k\" \"@cfa3.rs2\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete cfa3.rs1
file delete cfa3.rs2
if [runCmd "\"$cpld_bin/tda\" -i cfa3.bl5 -o cfa3.tda -lci cfa3.lct -dev m4s_64_64 -family lc4k -mod top -ovec NoInput.tmv -err tda.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj cfa3 -if cfa3.jed -j2s -log cfa3.svl "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 01/29/11 00:18:15 ###########

