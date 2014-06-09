// AP Startup Code

.code16
.global ap_startup_routine
ap_startup_routine:
        cli
        hlt
ap_startup_routine_end:

.global ap_startup_routine_size
.set    ap_startup_routine_size, ap_startup_routine_end - ap_startup_routine
