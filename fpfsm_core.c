/****************************************************************************
 * FileName:            FPFSM_CORE.h
 * Dependencies:        None
 * Processor:           
 * Compiler:            
 * Company:             
 * Date:                2016.12.03
 * Version:             1.01
 * Writer:              
 * Overview:            fpfsm_core模块实现文件
 * Note:                None
******************************************************************************/
#include    "app_cfg.h"
#include    "fpfsm_core.h"

/****************************定义内部类型区***************************/
#define CORE_LIST_ITEM_SIZE    (sizeof(core_task_t))
#define this (*ptThis)

/****************************定义内部变量区***************************/
//task list
static  core_task_t  *s_ptFreeList     = NULL;

//task FIFO
static  core_task_t  *s_pTaskFIFOHead = NULL;
static  core_task_t  **s_ppTaskFIFOTail= NULL;


/******************************************************************************
 * Function:        void    init_fpfsm_core_t(void)
 *
 * PreCondition:    None
 *
 * Input:           fpfsm_core_t *ptItem
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:            None
 *****************************************************************************/
void    init_fpfsm_core_t(void)
{
    s_ptFreeList        =   NULL;
    s_pTaskFIFOHeard    =   NULL;
    s_ppTaskFIFOTail    =   NULL;
}




/******************************************************************************
 * Function:        static void free_fpfsm_core_t(core_task_t *ptItem)
 *
 * PreCondition:    None
 *
 * Input:           core_task_t *ptThis
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:            None
 *****************************************************************************/
static void free_fpfsm_core_t(core_task_t *ptThis)
{
    if(NULL ==  ptThis){
        return;
    }
    this.ptNext  =   s_ptFreeList;
    s_ptFreeList =   &this;
}


/******************************************************************************
 * Function:        static core_task_t *malloc_fpfsm_core_t(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          core_task_t*
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:            None
 *****************************************************************************/
static core_task_t *malloc_fpfsm_core_t(void)
{
    core_task_t  *ptThis;
    if(NULL == s_ptFreeList){
        return  NULL;
    }
    
    ptThis = s_ptFreeList;

    s_ptFreeList  =   this.ptNext;
    this.ptNext   =   NULL;
    return  &this;
}


/******************************************************************************
 * Function:        bool add_memory_block_to_fpfsm_core_t_heap(void *pBlock, uint32_t wBlockSize)
 *
 * PreCondition:    None
 *
 * Input:           void *pBlock, uint32_t wBlockSize
 *
 * Output:          bool
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:            None
 *****************************************************************************/
bool add_memory_block_to_fpfsm_core_t_heap(void *pBlock, uint32_t wBlockSize)
{
    core_task_t   *ptNew     =   NULL;
    uint8_t       *pchTemp   =   pBlock;
    uint32_t      wi         =   0;
    if(NULL == pBlock){
        return  false;
    }
    if(CORE_LIST_ITEM_SIZE > wBlockSize){
        return false;
    }
    //添加新的节点
    for(wi=0;wi<wBlockSize;wi+=CORE_LIST_ITEM_SIZE){
        if((wi+CORE_LIST_ITEM_SIZE) > wBlockSize){
            break;
        }    
        ptNew = (core_task_t *)(pchTemp+wi);
        free_fpfsm_core_t(ptNew);
    }
    return true;
}



/******************************************************************************
 * Function:        static void delete_task_fpfsm_core_t(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:            None
 *****************************************************************************/
static void delete_task_fpfsm_core_t(void)
{
    core_task_t *ptThis;
    if(NULL == s_pTaskFIFOHead){
        return;
    }
    ptThis = s_pTaskFIFOHead;
    
    s_pTaskFIFOHead = this.ptNext;
    if(NULL == s_pTaskFIFOHead){
        s_ppTaskFIFOTail = NULL;
    }    
    free_fpfsm_core_t(ptThis);
}




/******************************************************************************
 * Function:        bool add_task_fpfsm_core_t(ptTask *pUserTask,void *pRam,is_jump_fifo_t tHeadOrTail)
 *
 * PreCondition:    None
 *
 * Input:           ptTask *pUserTask,void *pRam,is_jump_fifo_t tHeadOrTail
 *
 * Output:          bool
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:            None
 *****************************************************************************/
bool add_task_fpfsm_core_t(ptTask *pUserTask,void *pRam,is_jump_fifo_t tHeadOrTail)
{
    core_task_t     *ptThis     = NULL;
    core_task_t     **pptTemp   = NULL;
    SYS_ENTER_CRITICAL();
    ptThis      =   malloc_fpfsm_core_t();
    SYS_EXIT_CRITICAL();
    if(NULL == ptThis){
        return false;
    }
    this.pCallTask  =   pUserTask;
    this.pRam       =   pRam;
    
    SYS_ENTER_CRITICAL();
    if(tHeardOrTail){
        this.ptNext         =   s_pTaskFIFOHeard;
        s_pTaskFIFOHeard    =   &this;
        if(NULL == s_ppTaskFIFOTail){
            s_ppTaskFIFOTail = &(this.ptNext);
        }    
    }else{
        if(NULL == s_ppTaskFIFOTail){
            s_pTaskFIFOHeard    = &this;
        }else{
            *s_ppTaskFIFOTail   = &this;
        }    
        s_ppTaskFIFOTail = &(this.ptNext);
        this.ptNext      = NULL;
    }
    SYS_EXIT_CRITICAL();
    
    return  true;
}




/******************************************************************************
 * Function:        bool scheduler_fpfsm_core_t(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          bool
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:            None
 *****************************************************************************/
bool scheduler_fpfsm_core_t(void)
{
    core_task_t ptTask;
    void        *pRam   = NULL;
    
    SYS_ENTER_CRITICAL();
    if(NULL == s_pTaskFIFOHead){
         SYS_EXIT_CRITICAL();
         return false;
    }
    ptTask = *s_pTaskFIFOHead;
    delete_task_fpfsm_core_t();
    SYS_EXIT_CRITICAL();
    
    if(NULL != ptTask.pCallTask){
        if(ptTask.pCallTask(ptTask.pRam)){
            add_task_fpfsm_core_t(ptTask.pCallTask,ptTask.pRam,ADD_TASK_FIFO_TAIL);
        }    
    }    
    return true;
}    


