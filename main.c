/*********************************************************
FileName	:	main.c
Date		:	2015.03.06
Author		:	
Version		:	1.0.2
Description	:	主文件
***********************************************************/


#include    "../heard/pic18f4550_config.h"
#include    "../heard/heard.h"
#include    "../fpfsm_core/fpfsm_core.h"
//宏定义
#define	DELAY_1MS	274//310


//变量声明
#define TASK_NUM    10
static  uint8_t s_chTaskMemory[FSFSM_TASK_TCB_SIZE * TASK_NUM];



//函数声明
void	IO_Init(void);


static  bool    task_delay(void*    pTemp);
static  bool    task1_start(void *pTemp);
static  bool    task1_task(void*    pTemp);




void	main(void)
{
    DisINT()
        IO_Init();
        SYS_INIT_FSM();
        USER_NEW_MEMORY_FSM(s_chTaskMemory,(FSFSM_TASK_TCB_SIZE * TASK_NUM));
    EnINT()

    task1_start(NULL);
    while(1)
    {
        SYS_CALL_SUB_FSM();
    }
}



void	IO_Init(void)
{
    UCON = 0;
    UIE = 0;                      // Mask all USB interrupts
    UCONbits.USBEN = 0;           // Disable module & attach to bus
    ADCON1 |= 0x0F;               // Default all pins to digital

    LED_TRIS=ALLOUTPUT_PIN;
    LED		=0xFF;

    BU_TRIS = ALLOUTPUT_PIN;
    BU = 0;
}





/****************** TASK ******************/
enum{
    TASK_DELAY_START    =   0,
    TASK_DELAY_WAIT,
    TASK_DELAY_END
};

typedef struct{
    uint8_t     state;
    uint16_t    hwDelayTimer;
    uint16_t    hwDelayMS;
    ptTask      *PCallBack;
}tLEDTaskStack;



static  bool    task_delay(void*    pTemp)
{
    tLEDTaskStack   *ptTaskStack = (tLEDTaskStack*)pTemp;
    if(NULL == ptTaskStack){
        return false;
    }
    switch(ptTaskStack->state){
    case TASK_DELAY_START:
        ptTaskStack->state        =   TASK_DELAY_WAIT;
        ptTaskStack->hwDelayMS    =   DELAY_1MS;
        //break;
    case TASK_DELAY_WAIT:
        ptTaskStack->hwDelayMS--;
        if(!ptTaskStack->hwDelayMS){
            ptTaskStack->hwDelayMS = DELAY_1MS;
            ptTaskStack->hwDelayTimer--;
            if(!ptTaskStack->hwDelayTimer){
                ptTaskStack->state        =   TASK_DELAY_END;
            }
        }
        break;
    case TASK_DELAY_END:
        if(USER_NEW_FSM(ptTaskStack->PCallBack,ptTaskStack,ADD_TASK_FIFO_TAIL)){
            return false;   
        }
        break;
    }
    return true;
}

/****************** TASK1 ******************/
static  bool    task1_start(void *pTemp)
{
    static  tLEDTaskStack   s_tTask1Pram;
    s_tTask1Pram.state=TASK_DELAY_START;
    s_tTask1Pram.hwDelayTimer = 100;
    s_tTask1Pram.PCallBack    = NULL;
    if(USER_NEW_FSM(task1_task,&s_tTask1Pram,ADD_TASK_FIFO_TAIL)){
        return false;
    }
    return true;
}


static  bool    task1_task(void*    pTemp)
{
    tLEDTaskStack   *ptTask1Pram = (tLEDTaskStack*)pTemp;
    if(NULL == ptTask1Pram){
        return false;
    }
    switch(ptTask1Pram->state){
    case TASK_DELAY_START:
        LED1 = ~LED1;
        ptTask1Pram->state = TASK_DELAY_WAIT;
    case TASK_DELAY_WAIT:
        if(USER_NEW_FSM(task_delay,ptTask1Pram,ADD_TASK_FIFO_TAIL)){
            ptTask1Pram->state=TASK_DELAY_START;
            ptTask1Pram->PCallBack    = task1_start;
            return false;
        }
        break;
    }
    return true;
}