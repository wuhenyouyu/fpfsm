/****************************************************************************
 * FileName:            FPFSM_CORE.h
 * Dependencies:        None
 * Processor:           
 * Compiler:            
 * Company:             
 * Date:                2016.12.03
 * Version:             1.00
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


typedef enum{
    ADD_TASK_FIFO_TAIL  =   0,
    ADD_TASK_FIFO_HEARD
}is_jump_fifo_t;


typedef bool ptTask(void *pRam);

typedef struct  core_task_t core_task_t;
struct  core_task_t{
    bool        bBlock;
    ptTask      *pCallTask;
    void*       pRam;
    core_task_t *ptNext;
};

#define FSFSM_TASK_TCB_SIZE     (sizeof(struct  core_task_t))

bool add_memory_block_to_fpfsm_core_t_heap(void *pBlock, uint32_t wBlockSize);


bool add_task_fpfsm_core_t(ptTask *pUserTask,void *pRam,is_jump_fifo_t tHeardOrTail);

bool scheduler_fpfsm_core_t(void);


#ifdef __cplusplus
	}
#endif

#endif