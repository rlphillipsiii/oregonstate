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

/* This file is designed for use with ISim build 0x2f00eba5 */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "C:/Users/Robert/Documents/ECE272DemoReceiver/Receiver.v";
static unsigned int ng1[] = {128U, 0U};
static unsigned int ng2[] = {5U, 0U};
static unsigned int ng3[] = {64U, 0U};
static unsigned int ng4[] = {0U, 0U};
static unsigned int ng5[] = {2064U, 0U};
static unsigned int ng6[] = {4U, 0U};
static unsigned int ng7[] = {2080U, 0U};
static unsigned int ng8[] = {1U, 0U};
static unsigned int ng9[] = {160U, 0U};
static unsigned int ng10[] = {9U, 0U};
static unsigned int ng11[] = {144U, 0U};
static unsigned int ng12[] = {6U, 0U};
static unsigned int ng13[] = {96U, 0U};
static unsigned int ng14[] = {8U, 0U};
static unsigned int ng15[] = {80U, 0U};
static unsigned int ng16[] = {2U, 0U};
static unsigned int ng17[] = {15U, 0U};



static void Always_200_0(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    int t6;
    char *t7;
    char *t8;

LAB0:    t1 = (t0 + 1352U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(200, ng0);
    t2 = (t0 + 1548);
    *((int *)t2) = 1;
    t3 = (t0 + 1380);
    *((char **)t3) = t2;
    *((char **)t1) = &&LAB4;

LAB1:    return;
LAB4:    xsi_set_current_line(201, ng0);
    t4 = (t0 + 600U);
    t5 = *((char **)t4);

LAB5:    t4 = ((char*)((ng1)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 12, t4, 12);
    if (t6 == 1)
        goto LAB6;

LAB7:    t2 = ((char*)((ng3)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 12, t2, 12);
    if (t6 == 1)
        goto LAB8;

LAB9:    t2 = ((char*)((ng5)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 12, t2, 12);
    if (t6 == 1)
        goto LAB10;

LAB11:    t2 = ((char*)((ng7)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 12, t2, 12);
    if (t6 == 1)
        goto LAB12;

LAB13:    t2 = ((char*)((ng9)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 12, t2, 12);
    if (t6 == 1)
        goto LAB14;

LAB15:    t2 = ((char*)((ng11)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 12, t2, 12);
    if (t6 == 1)
        goto LAB16;

LAB17:    t2 = ((char*)((ng13)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 12, t2, 12);
    if (t6 == 1)
        goto LAB18;

LAB19:    t2 = ((char*)((ng15)));
    t6 = xsi_vlog_unsigned_case_compare(t5, 12, t2, 12);
    if (t6 == 1)
        goto LAB20;

LAB21:
LAB23:
LAB22:    xsi_set_current_line(211, ng0);
    t2 = ((char*)((ng17)));
    t3 = (t0 + 828);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 4);

LAB24:    goto LAB2;

LAB6:    xsi_set_current_line(203, ng0);
    t7 = ((char*)((ng2)));
    t8 = (t0 + 828);
    xsi_vlogvar_assign_value(t8, t7, 0, 0, 4);
    goto LAB24;

LAB8:    xsi_set_current_line(204, ng0);
    t3 = ((char*)((ng4)));
    t4 = (t0 + 828);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 4);
    goto LAB24;

LAB10:    xsi_set_current_line(205, ng0);
    t3 = ((char*)((ng6)));
    t4 = (t0 + 828);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 4);
    goto LAB24;

LAB12:    xsi_set_current_line(206, ng0);
    t3 = ((char*)((ng8)));
    t4 = (t0 + 828);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 4);
    goto LAB24;

LAB14:    xsi_set_current_line(207, ng0);
    t3 = ((char*)((ng10)));
    t4 = (t0 + 828);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 4);
    goto LAB24;

LAB16:    xsi_set_current_line(208, ng0);
    t3 = ((char*)((ng12)));
    t4 = (t0 + 828);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 4);
    goto LAB24;

LAB18:    xsi_set_current_line(209, ng0);
    t3 = ((char*)((ng14)));
    t4 = (t0 + 828);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 4);
    goto LAB24;

LAB20:    xsi_set_current_line(210, ng0);
    t3 = ((char*)((ng16)));
    t4 = (t0 + 828);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 4);
    goto LAB24;

}


extern void work_m_00000000002859429031_2895063221_init()
{
	static char *pe[] = {(void *)Always_200_0};
	xsi_register_didat("work_m_00000000002859429031_2895063221", "isim/RecieverTestWtestpoints_isim_beh.exe.sim/work/m_00000000002859429031_2895063221.didat");
	xsi_register_executes(pe);
}
