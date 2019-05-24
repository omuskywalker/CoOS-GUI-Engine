/**
 *******************************************************************************
 * @file       GuiSystem.h
 * @version    V0.0.1   
 * @date       2019.5.15
 * @brief      This is a file for GUI engine.	
 *******************************************************************************
 * @copy
 *
 * 
 *******************************************************************************
 */ 

/*---------------------------- Include ---------------------------------------*/
#include <coocox.h>
#include <CoOS.h>
#include <OsKernelHeap.h>

/*---------------------------- Error Codes   ---------------------------------*/
#define E_TITLE_NULL          (StatusType)22	

/*---------------------------- structure -------------------------------------*/
typedef struct listNode
{
    struct listNode *prev;
    struct listNode *next;
}CoList,*P_CoList;


/*---------------------------- Function Define -------------------------------*/
void *GuiMalloc(U32 size);
void GuiFree(void* memBuf);
