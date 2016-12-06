/****************************************************************************
 * FileName:            FPFSM_CORE.h
 * Dependencies:        None
 * Processor:           
 * Compiler:            
 * Company:             
 * Date:                2016.12.03
 * Version:             1.01
 * Writer:              
 * Overview:            fpfsm_core模块输出接口文件
 * Note:                None
******************************************************************************/
#ifndef __FPFSM_CORE_H__
#define __FPFSM_CORE_H__

#ifdef __cplusplus
extern "C"
	{
#endif

//声明类型区
typedef enum{
    ADD_TASK_FIFO_TAIL  =   0,
    ADD_TASK_FIFO_HEARD
}is_jump_fifo_t;


typedef bool ptTask(void *pRam);

typedef struct  core_task_t core_task_t;
struct  core_task_t{
    ptTask      *pCallTask;
    void        *pRam;
    core_task_t *ptNext;
};

#define FSFSM_TASK_TCB_SIZE     (sizeof(struct  core_task_t))   //只用到SIZE，用不到类型




//用户接口

void    init_fpfsm_core_t(void);

bool add_memory_block_to_fpfsm_core_t_heap(void *pBlock, uint32_t wBlockSize);

bool add_task_fpfsm_core_t(ptTask *pUserTask,void *pRam,is_jump_fifo_t tHeardOrTail);

bool scheduler_fpfsm_core_t(void);


//user/sys interface
#define USER_NEW_FSM(__TASK,__PRAM,__PRIORITY)  add_task_fpfsm_core_t((__TASK),(__PRAM),(__PRIORITY))

#define SYS_CALL_SUB_FSM()                      scheduler_fpfsm_core_t()

#define SYS_INIT_FSM(__MEMORY,__SIZE)           do{\
                                                    init_fpfsm_core_t();\
                                                    add_memory_block_to_fpfsm_core_t_heap((__MEMORY),(__SIZE));\
                                                }while(0)

#ifdef __cplusplus
	}
#endif

#endif