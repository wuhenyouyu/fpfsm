/****************************************************************************
 * FileName:            app_cfg.h
 * Dependencies:        None
 * Processor:           
 * Compiler:            
 * Company:             
 * Date:                2016.12.03
 * Version:             1.00
 * Writer:              
 * Overview:            fpfsm_core ‰»Î≈‰÷√Œƒº˛
 * Note:                None
******************************************************************************/
#ifndef __FPFSM_CORE_APP_CFG_H__
#define __FPFSM_CORE_APP_CFG_H__

#ifdef __cplusplus
extern "C"
	{
#endif




#include    "../heard/heard.h"
#include    "../heard/stdint.h"



#define SYS_ENTER_CRITICAL()    EnINT()
#define SYS_EXIT_CRITICAL()     DisINT()






#ifdef __cplusplus
	}
#endif

#endif