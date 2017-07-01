
# PlanAhead Launch Script for Pre-Synthesis Floorplanning, created by Project Navigator

create_project -name N64_wired -dir "C:/Users/Robert/Dropbox/Porn/Summer 2012/N64/wired_remote/N64_wired/planAhead_run_2" -part xc7a100tcsg324-3
set_param project.pinAheadLayout yes
set srcset [get_property srcset [current_run -impl]]
set_property target_constrs_file "Remote.ucf" [current_fileset -constrset]
set hdlfile [add_files [list {clock_divider.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set hdlfile [add_files [list {Remote.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set_property top Remote $srcset
add_files [list {Remote.ucf}] -fileset [get_property constrset [current_run]]
open_rtl_design -part xc7a100tcsg324-3
