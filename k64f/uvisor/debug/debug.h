/***************************************************************
 * This confidential and  proprietary  software may be used only
 * as authorised  by  a licensing  agreement  from  ARM  Limited
 *
 *             (C) COPYRIGHT 2013-2014 ARM Limited
 *                      ALL RIGHTS RESERVED
 *
 *  The entire notice above must be reproduced on all authorised
 *  copies and copies  may only be made to the  extent permitted
 *  by a licensing agreement from ARM Limited.
 *
 ***************************************************************/
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <uvisor.h>

void debug_fault_bus(uint32_t lr);
void debug_init(void);

#ifdef  NDEBUG
#define DEBUG_FAULT_BUS(...) {}
#define DEBUG_INIT(...)      {}
#else /*NDEBUG*/
#define DEBUG_FAULT_BUS()    {\
    /************************************************************************/\
    /* lr is used to check execution mode before exception                  */\
    /* NOTE: this only works if the function is executed before any branch  */\
    /*       instruction right after the exception                          */\
    register uint32_t lr asm("lr");\
    /************************************************************************/\
    debug_fault_bus(lr);                                                      \
}

#define DEBUG_INIT debug_init
#endif/*NDEBUG*/

#endif/*__DEBUG_H__*/