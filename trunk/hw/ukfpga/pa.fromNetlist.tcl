
# PlanAhead Launch Script for Post-Synthesis floorplanning, created by Project Navigator

hdi::project new -name ukfpga -dir "C:/Projects/HW/ukfpga/patmp" -netlist "top.ngc" -search_path { {C:/Projects/HW/ukfpga} }
hdi::project setArch -name ukfpga -arch spartan3e
hdi::param set -name project.paUcfFile -svalue "Nexys2_500General.ucf"
hdi::floorplan new -name floorplan_1 -part xc3s500efg320-5 -project ukfpga
hdi::pconst import -project ukfpga -floorplan floorplan_1 -file "Nexys2_500General.ucf"
