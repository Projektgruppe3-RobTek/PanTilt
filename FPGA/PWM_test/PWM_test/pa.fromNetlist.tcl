
# PlanAhead Launch Script for Post-Synthesis floorplanning, created by Project Navigator

create_project -name PWM_test -dir "C:/Users/nwl1515/Dropbox/RobTek - F15 Projekt/DIG/Kode/PWM_test/PWM_test/planAhead_run_1" -part xc3s500efg320-4
set_property design_mode GateLvl [get_property srcset [current_run -impl]]
set_property edif_top_file "C:/Users/nwl1515/Dropbox/RobTek - F15 Projekt/DIG/Kode/PWM_test/PWM_test/main.ngc" [ get_property srcset [ current_run ] ]
add_files -norecurse { {C:/Users/nwl1515/Dropbox/RobTek - F15 Projekt/DIG/Kode/PWM_test/PWM_test} }
set_property target_constrs_file "main.ucf" [current_fileset -constrset]
add_files [list {main.ucf}] -fileset [get_property constrset [current_run]]
link_design
