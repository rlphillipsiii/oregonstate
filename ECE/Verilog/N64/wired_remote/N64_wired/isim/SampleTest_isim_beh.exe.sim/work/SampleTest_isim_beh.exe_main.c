/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

#include "xsi.h"

struct XSI_INFO xsi_info;



int main(int argc, char **argv)
{
    xsi_init_design(argc, argv);
    xsi_register_info(&xsi_info);

    xsi_register_min_prec_unit(-12);
    work_m_00000000004069939614_2867366326_init();
    work_m_00000000000020406173_2253964355_init();
    work_m_00000000003895497853_1152422479_init();
    work_m_00000000003733479056_3959061474_init();
    work_m_00000000001941191385_2035480523_init();
    work_m_00000000003235052626_3732038678_init();
    work_m_00000000004134447467_2073120511_init();


    xsi_register_tops("work_m_00000000003235052626_3732038678");
    xsi_register_tops("work_m_00000000004134447467_2073120511");


    return xsi_run_simulation(argc, argv);

}
