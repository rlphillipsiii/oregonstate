
# PlanAhead Launch Script for Post-Synthesis floorplanning, created by Project Navigator

create_project -name N64_wired -dir "C:/Users/Robert/Dropbox/Porn/Summer 2012/N64/wired_remote/N64_wired/planAhead_run_4" -part xc7a100tcsg324-3
set_property design_mode GateLvl [get_property srcset [current_run -impl]]
set_property edif_top_file "C:/Users/Robert/Dropbox/Porn/Summer 2012/N64/wired_remote/N64_wired/Remote.ngc" [ get_property srcset [ current_run ] ]
add_files -norecurse { {C:/Users/Robert/Dropbox/Porn/Summer 2012/N64/wired_remote/N64_wired} }
set_property target_constrs_file "Remote.ucf" [current_fileset -constrset]
add_files [list {Remote.ucf}] -fileset [get_property constrset [current_run]]
link_design
