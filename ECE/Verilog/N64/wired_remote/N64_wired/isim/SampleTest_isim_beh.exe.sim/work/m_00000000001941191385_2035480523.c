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

/* This file is designed for use with ISim build 0xc3576ebc */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "C:/Users/Robert/Dropbox/Porn/Summer 2012/N64/wired_remote/N64_wired/Remote.v";
static unsigned int ng1[] = {65343U, 0U};
static unsigned int ng2[] = {2048U, 0U};
static unsigned int ng3[] = {5U, 0U};
static unsigned int ng4[] = {1024U, 0U};
static unsigned int ng5[] = {0U, 0U};
static unsigned int ng6[] = {256U, 0U};
static unsigned int ng7[] = {1U, 0U};
static unsigned int ng8[] = {512U, 0U};
static unsigned int ng9[] = {4U, 0U};
static unsigned int ng10[] = {2560U, 0U};
static unsigned int ng11[] = {6U, 0U};
static unsigned int ng12[] = {2304U, 0U};
static unsigned int ng13[] = {9U, 0U};
static unsigned int ng14[] = {1536U, 0U};
static unsigned int ng15[] = {8U, 0U};
static unsigned int ng16[] = {1280U, 0U};
static unsigned int ng17[] = {2U, 0U};
static unsigned int ng18[] = {15U, 0U};



static void Always_158_0(char *t0)
{
    char t6[8];
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    unsigned int t7;
    unsigned int t8;
    unsigned int t9;
    char *t10;
    char *t11;
    char *t12;
    unsigned int t13;
    unsigned int t14;
    unsigned int t15;
    unsigned int t16;
    unsigned int t17;
    unsigned int t18;
    unsigned int t19;
    char *t20;
    char *t21;
    unsigned int t22;
    unsigned int t23;
    unsigned int t24;
    unsigned int t25;
    unsigned int t26;
    unsigned int t27;
    unsigned int t28;
    unsigned int t29;
    int t30;
    int t31;
    unsigned int t32;
    unsigned int t33;
    unsigned int t34;
    unsigned int t35;
    unsigned int t36;
    unsigned int t37;
    char *t38;
    int t39;
    char *t40;
    char *t41;

LAB0:    t1 = (t0 + 2368U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(158, ng0);
    t2 = (t0 + 2688);
    *((int *)t2) = 1;
    t3 = (t0 + 2400);
    *((char **)t3) = t2;
    *((char **)t1) = &&LAB4;

LAB1:    return;
LAB4:    xsi_set_current_line(159, ng0);
    t4 = (t0 + 1048U);
    t5 = *((char **)t4);
    t4 = ((char*)((ng1)));
    t7 = *((unsigned int *)t5);
    t8 = *((unsigned int *)t4);
    t9 = (t7 & t8);
    *((unsigned int *)t6) = t9;
    t10 = (t5 + 4);
    t11 = (t4 + 4);
    t12 = (t6 + 4);
    t13 = *((unsigned int *)t10);
    t14 = *((unsigned int *)t11);
    t15 = (t13 | t14);
    *((unsigned int *)t12) = t15;
    t16 = *((unsigned int *)t12);
    t17 = (t16 != 0);
    if (t17 == 1)
        goto LAB25;

LAB26:
LAB27:
LAB5:    t38 = ((char*)((ng2)));
    t39 = xsi_vlog_unsigned_case_compare(t6, 16, t38, 16);
    if (t39 == 1)
        goto LAB6;

LAB7:    t2 = ((char*)((ng4)));
    t30 = xsi_vlog_unsigned_case_compare(t6, 16, t2, 16);
    if (t30 == 1)
        goto LAB8;

LAB9:    t2 = ((char*)((ng6)));
    t30 = xsi_vlog_unsigned_case_compare(t6, 16, t2, 16);
    if (t30 == 1)
        goto LAB10;

LAB11:    t2 = ((char*)((ng8)));
    t30 = xsi_vlog_unsigned_case_compare(t6, 16, t2, 16);
    if (t30 == 1)
        goto LAB12;

LAB13:    t2 = ((char*)((ng10)));
    t30 = xsi_vlog_unsigned_case_compare(t6, 16, t2, 16);
    if (t30 == 1)
        goto LAB14;

LAB15:    t2 = ((char*)((ng12)));
    t30 = xsi_vlog_unsigned_case_compare(t6, 16, t2, 16);
    if (t30 == 1)
        goto LAB16;

LAB17:    t2 = ((char*)((ng14)));
    t30 = xsi_vlog_unsigned_case_compare(t6, 16, t2, 16);
    if (t30 == 1)
        goto LAB18;

LAB19:    t2 = ((char*)((ng16)));
    t30 = xsi_vlog_unsigned_case_compare(t6, 16, t2, 16);
    if (t30 == 1)
        goto LAB20;

LAB21:
LAB23:
LAB22:    xsi_set_current_line(168, ng0);
    t2 = ((char*)((ng18)));
    t3 = (t0 + 1448);
    xsi_vlogvar_wait_assign_value(t3, t2, 0, 0, 4, 0LL);

LAB24:    goto LAB2;

LAB6:    xsi_set_current_line(160, ng0);
    t40 = ((char*)((ng3)));
    t41 = (t0 + 1448);
    xsi_vlogvar_wait_assign_value(t41, t40, 0, 0, 4, 0LL);
    goto LAB24;

LAB8:    xsi_set_current_line(161, ng0);
    t3 = ((char*)((ng5)));
    t4 = (t0 + 1448);
    xsi_vlogvar_wait_assign_value(t4, t3, 0, 0, 4, 0LL);
    goto LAB24;

LAB10:    xsi_set_current_line(162, ng0);
    t3 = ((char*)((ng7)));
    t4 = (t0 + 1448);
    xsi_vlogvar_wait_assign_value(t4, t3, 0, 0, 4, 0LL);
    goto LAB24;

LAB12:    xsi_set_current_line(163, ng0);
    t3 = ((char*)((ng9)));
    t4 = (t0 + 1448);
    xsi_vlogvar_wait_assign_value(t4, t3, 0, 0, 4, 0LL);
    goto LAB24;

LAB14:    xsi_set_current_line(164, ng0);
    t3 = ((char*)((ng11)));
    t4 = (t0 + 1448);
    xsi_vlogvar_wait_assign_value(t4, t3, 0, 0, 4, 0LL);
    goto LAB24;

LAB16:    xsi_set_current_line(165, ng0);
    t3 = ((char*)((ng13)));
    t4 = (t0 + 1448);
    xsi_vlogvar_wait_assign_value(t4, t3, 0, 0, 4, 0LL);
    goto LAB24;

LAB18:    xsi_set_current_line(166, ng0);
    t3 = ((char*)((ng15)));
    t4 = (t0 + 1448);
    xsi_vlogvar_wait_assign_value(t4, t3, 0, 0, 4, 0LL);
    goto LAB24;

LAB20:    xsi_set_current_line(167, ng0);
    t3 = ((char*)((ng17)));
    t4 = (t0 + 1448);
    xsi_vlogvar_wait_assign_value(t4, t3, 0, 0, 4, 0LL);
    goto LAB24;

LAB25:    t18 = *((unsigned int *)t6);
    t19 = *((unsigned int *)t12);
    *((unsigned int *)t6) = (t18 | t19);
    t20 = (t5 + 4);
    t21 = (t4 + 4);
    t22 = *((unsigned int *)t5);
    t23 = (~(t22));
    t24 = *((unsigned int *)t20);
    t25 = (~(t24));
    t26 = *((unsigned int *)t4);
    t27 = (~(t26));
    t28 = *((unsigned int *)t21);
    t29 = (~(t28));
    t30 = (t23 & t25);
    t31 = (t27 & t29);
    t32 = (~(t30));
    t33 = (~(t31));
    t34 = *((unsigned int *)t12);
    *((unsigned int *)t12) = (t34 & t32);
    t35 = *((unsigned int *)t12);
    *((unsigned int *)t12) = (t35 & t33);
    t36 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t36 & t32);
    t37 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t37 & t33);
    goto LAB27;

}


extern void work_m_00000000001941191385_2035480523_init()
{
	static char *pe[] = {(void *)Always_158_0};
	xsi_register_didat("work_m_00000000001941191385_2035480523", "isim/SampleTest_isim_beh.exe.sim/work/m_00000000001941191385_2035480523.didat");
	xsi_register_executes(pe);
}
