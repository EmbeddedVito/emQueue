/*
 * @file emQueue.c
 * @author Mannone Vito
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "emQueue.h"

emQueueHandle_t emQueue_New(const size_t queueSize, const size_t elemSize, const char *name) {
	if( (queueSize < 2) || (elemSize < 1) ) return NULL;
	emQueueHandle_t retval = emQueuePort_Malloc(sizeof(Handler_t));
	if(retval != NULL) {
		retval->dataStruct = emQueuePort_InitStruct(queueSize, elemSize, name);
		retval->semHandle = emQueuePort_InitBynSem(name);
		retval->elemSize = elemSize;
	}
	return retval;
}

emQueueReturn_t emQueue_IsFull(emQueueHandle_t queue) {
	if(queue == NULL) {
		return emError;
	}
	emQueueReturn_t retval = emQueuePort_EnterCritical(queue->semHandle);
	if(retval != 0) {
		emQueuePort_ExitCritical(queue->semHandle);
		return em_SemError;
	} else {
		int temp = emQueuePort_StructIsFull(queue->dataStruct);
		
		retval = temp ? em_True : em_False;
	}
	emQueuePort_ExitCritical(queue->semHandle);
	return retval;
}

emQueueReturn_t emQueue_IsEmpty(emQueueHandle_t queue) {
	if(queue == NULL) {
		return emError;
	}
	emQueueReturn_t retval = emQueuePort_EnterCritical(queue->semHandle);
	if(retval != 0) {
		emQueuePort_ExitCritical(queue->semHandle);
		return em_SemError;
	} else {
		int temp = emQueuePort_StructIsEmpty(queue->dataStruct);
		
		retval = temp ? em_True : em_False;
	}
	emQueuePort_ExitCritical(queue->semHandle);
	return retval;
}

emQueueReturn_t emQueue_Put(emQueueHandle_t queue, const void *ptrElem) {
	if(queue == NULL) return emError;
	emQueueReturn_t retVal = emQueuePort_EnterCritical(queue->semHandle);
	if(retVal == 0) {
		if(emQueuePort_StructIsFull(queue->dataStruct)) {
			retVal = em_QueueFull;
		} else {
			void * dest = emQueuePort_StructGetHead(queue->dataStruct);
			emQueuePort_ElemCpy(ptrElem, dest, queue->elemSize);
			retVal = em_True;
		}
	} else {
		emQueuePort_ExitCritical(queue->semHandle);
		return em_SemError;
	}
	emQueuePort_ExitCritical(queue->semHandle);
	return retVal;
}

emQueueReturn_t emQueue_Get(emQueueHandle_t queue, void *ptrDest) {
	if(queue == NULL) return emError;
	emQueueReturn_t retVal = emQueuePort_EnterCritical(queue->semHandle);
	if(retVal == 0) {
		if(emQueuePort_StructIsEmpty(queue->dataStruct)) {
			retVal = em_QueueEmpty;
		} else {
			void * src = emQueuePort_StructGetTail(queue->dataStruct);
			emQueuePort_ElemCpy(src, ptrDest, queue->elemSize);
			retVal = em_True;
		}
	} else {
		emQueuePort_ExitCritical(queue->semHandle);
		return em_SemError;
	}
	emQueuePort_ExitCritical(queue->semHandle);
	return retVal;
}

emQueueReturn_t emQueue_Delete(emQueueHandle_t queue) {
	if(queue == NULL) return em_True;
	/* Save the reference of the semaphore */
	void *sem = queue->semHandle;
	emQueuePort_EnterCritical(sem);

	emQueueport_DeleteStruct(queue->dataStruct);

	emQueuePort_Free(queue);

	emQueuePort_ExitCritical(sem);
	emQueuePort_BynSemDelete(sem);
	return em_True;
}
