/*
 * emQueuePort.h
 *
 *  Created on: 23 giu 2023
 *      Author: vitomannone
 */

#ifndef EMQUEUEPORT_H_
#define EMQUEUEPORT_H_

/*
 * Enabling this the user can allow the code to manage the copy of the data
 * structure using ElemType.h, otherwise the pointer to the data structure will be
 * returned and the user must handle it inside a critical section
 */
#define QUEUE_USE_LOCK_MECHANISM 1
#define QUEUE_USE_DATATYPE_HANDLER 0
#define QUEUE_USE_EMALLOC 1

/*
 * Necessary definition for lock/unlock functions
 */
#if QUEUE_USE_LOCK_MECHANISM
#include "cmsis_os2.h"
#define emQueuePort_EnterCritical(ptrSem)			( osSemaphoreAcquire(ptrSem, osWaitForever) )
#define emQueuePort_ExitCritical(ptrSem)			( osSemaphoreRelease(ptrSem) )
#define emQueuePort_InitBynSem(strName)				( osSemaphoreNew(1, 1, NULL) )
#define emQueuePort_BynSemDelete(ptrSem)			( osSemaphoreDelete(ptrSem) )
typedef osSemaphoreId_t* Queue_sem_t;
#else
#define emQueuePort_EnterCritical(ptrSem)			( 0 )
#define emQueuePort_ExitCritical(ptrSem)			(void)ptrSem
#define emQueuePort_InitBynSem(strName)				( NULL )
#define emQueuePort_BynSemDelete(ptrSem)			(void)ptrSem
typedef void * Queue_sem_t;
#endif /* USE_LOCK_MECHANISM */
/*
 * Necessary definition for data structure handling
 */
#include "emCircularBuffer.h"
#define emQueuePort_InitStruct(nbStructures, structSize, strName)		emCircularInit(nbStructures, structSize, strName)
#define emQueueport_DeleteStruct(ptrStruct)								emCircularDelete(ptrStruct)
#define emQueuePort_StructIsFull(ptrStruct)								( emCircularIsFull(ptrStruct) == CB_true )
#define emQueuePort_StructIsEmpty(ptrStruct)							( emCircularIsEmpty(ptrStruct) == CB_true )
#define emQueuePort_StructGetHead(ptrStruct)							emCircularGetHead(ptrStruct)
#define emQueuePort_StructGetTail(ptrStruct)							emCircularGetTail(ptrStruct)

/*
 * Necessary definition to handle the specific data type of the structure
 */
#if QUEUE_USE_DATATYPE_HANDLER
#include "ElemType.h"
#define emQueuePort_ElemCpy(ptrSrc, ptrDest, size) 				elemCopy(ptrSrc, ptrDest)
#else
#include <string.h> //for memcpy
#define emQueuePort_ElemCpy(ptrSrc, ptrDest, size) 				memcpy(ptrDest, ptrSrc, size)
#endif /* USE_DATATYPE_HANDLER */
/*
 * Necessary definitions for memory allocation
 */
#if QUEUE_USE_EMALLOC
#include "emAlloc.h"
#define emQueuePort_Malloc(nbBytes)								emMalloc(nbBytes)
#define emQueuePort_Free(ptr)									emFree(ptr)
#else
#define emQueuePort_Malloc(nbBytes)								malloc(nbBytes)
#define emQueuePort_Free(ptr)									free(ptr)
#endif /* EMALLOC_H_ */

#endif /* EMQUEUEPORT_H_ */
