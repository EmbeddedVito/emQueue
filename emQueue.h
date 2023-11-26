/*
 * @file emQueue.h
 * @author Mannone Vito
 *
 * @brief implementation of a FIFO queue
 *
 * This implementation does not rely on a specific implementation either of
 * 	a data structure, operating system (locking mechanism) or dynamic memory
 * 	allocation algorithm. All these function can and must be defined in the
 * 	emQueuePort.h file.
 *
 * It is not possible to wait for the queue to be not full or not empty in order
 * to wake a task on this specific conditions. It is in fact necessary to poll
 * the queue status before writing and reading from it.
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

#ifndef EMQUEUE_H_
#define EMQUEUE_H_

#include <stddef.h>

/*
 * Necessary to define the porting functions
 */
#include "emQueuePort.h"

/*
 * Module return values
 */
typedef enum {
	emError = -1,
	em_True,
	em_False,
	em_SemError,
	em_QueueFull,
	em_QueueEmpty,
} emQueueReturn_t;

/*
 * Definition of the queue data type
 */
typedef struct {
	void *dataStruct;
	Queue_sem_t semHandle;
	size_t elemSize;
} Handler_t ;

typedef Handler_t * emQueueHandle_t;
/* End of data type definition */

/*
 * API Functions
 */
/*
 * @brief This function initializes the queue allocating the necessary memory for it.
 *
 * @param queueSize, number of elements of the queue
 * @param elemSize, size of every element in terms of bytes
 * @param name, name for the semaphore initialization. Can be NULL if
 * 		no locking mechanism is defined
 * @return emQueueHandle_t, pointer to the queue created. Returns
 * 		NULL if it was not possible to create the queue
 */
emQueueHandle_t emQueue_New(const size_t queueSize, const size_t elemSize, const char *name);

/*
 * @brief This function is used to know if the queue is full.
 *
 * @param queue, queue handler
 * @return emQueueReturn_t, return value
 */
emQueueReturn_t emQueue_IsFull(emQueueHandle_t queue);

/*
 * @brief This function is used to know if the queue is empty.
 *
 * @param queue, queue handler
 * @return emQueueReturn_t, return value
 */
emQueueReturn_t emQueue_IsEmpty(emQueueHandle_t queue);

/*
 * @brief This function is used to write an element in the queue.
 *
 * @param queue, queue handler
 * @param ptrElem, pointer to the element to be written
 * @return emQueueReturn_t, return value
 *
 * Note that this function will copy the element in the buffer. It is
 * assumed that the passed pointer is the address of an element of the
 * correct data type.
 * This function will check again if the queue is not full before writing
 * the element in the queue.
 */
emQueueReturn_t emQueue_Put(emQueueHandle_t queue, const void *ptrElem);

/*
 * @brief This function is used to read an element from the queue.
 *
 * @param queue, queue handler
 * @param ptrDest, pointer to the address of the element destination
 * @return emQueueReturn_t, return value
 *
 * Note that this function will copy the element from the buffer to the
 * ptrDest address.
 * This function will check again if the queue is not empty before reading
 * the element from the queue.
 */
emQueueReturn_t emQueue_Get(emQueueHandle_t queue, void *ptrDest);

/*
 * @brief This function is used to delete the queue and free all the
 * 		allocated memory.
 *
 * @param queue, queue handler
 * @return emQueueReturn_t, return value
 */
emQueueReturn_t emQueue_Delete(emQueueHandle_t queue);

#endif /* EMQUEUE_H_ */
