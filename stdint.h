 /****************************************************************************
 * FileName:        stdint.h
 * Dependencies:    None
 * Company:
 * Date:            2016.04.20
 * Version:         1.00
 * Writer:
 * Overview:        类型定义			
******************************************************************************/
#ifndef __STDINT_H__
#define __STDINT_H__






//类型定义
typedef unsigned            char    uint8_t;
typedef signed              char    int8_t;
typedef unsigned            short   uint16_t;
typedef signed              short   int16_t;
typedef unsigned            long    uint32_t;
typedef signed              long    int32_t;
typedef unsigned    long    long    uint64_t;
typedef signed      long    long    int64_t;


typedef unsigned            char    uint_fast8_t;


//布尔变量定义
typedef enum{
    false   =   0,
    true    =   !false
}bool;



#define NULL    0



//! \name finit state machine state
//! @{
typedef enum {
    fsm_rt_err          = -1,    //!< fsm error, error code can be get from other interface
    fsm_rt_cpl          = 0,     //!< fsm complete
    fsm_rt_on_going     = 1      //!< fsm on-going
} fsm_rt_t;
//! @}






#endif
