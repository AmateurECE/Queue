/******************************************************************************
 * NAME:	    qarray.c
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    Contains the implementation of a Queue, using Arrays, as
 *		    the name suggests. Also contains test code. Run
 *		    'make debug' to compile with the test code.
 *
 * CREATED:	    04/14/2017
 *
 * LAST EDITED:	    03/08/2018
 ***/

/* SAMPLE OUTPUT:
 * ===== Inserting ======
 * int 8 @ 0x7fbe71d00070
 * int 4 @ 0x7fbe71d00080
 * int 4 @ 0x7fbe71d00090
 * int 9 @ 0x7fbe71d000a0
 * int 8 @ 0x7fbe71d000b0
 * int 0 @ 0x7fbe71d000c0
 * int 5 @ 0x7fbe71d000d0
 * int 1 @ 0x7fbe71d000e0
 * int 0 @ 0x7fbe71d000f0
 * int 6 @ 0x7fbe71d00100
 * ====== Removing ======
 * int 8 @ 0x7fbe71d00070
 * int 4 @ 0x7fbe71d00080
 * int 4 @ 0x7fbe71d00090
 * int 9 @ 0x7fbe71d000a0
 * int 8 @ 0x7fbe71d000b0
 * int 0 @ 0x7fbe71d000c0
 * int 5 @ 0x7fbe71d000d0
 * int 1 @ 0x7fbe71d000e0
 * int 0 @ 0x7fbe71d000f0
 * int 6 @ 0x7fbe71d00100
 */

/******************************************************************************
 * INCLUDES
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef CONFIG_DEBUG_QARRAY
#include <assert.h>
#include <time.h>
#endif /* CONFIG_DEBUG_QARRAY */

#include "qarray.h"

/******************************************************************************
 * API FUNCTIONS
 ***/

/******************************************************************************
 * FUNCTION:	    qarray_create
 *
 * DESCRIPTION:	    Creates a new qarray and returns a pointer to it.
 *
 * ARGUMENTS:	    destroy: (void (*)(void *)) -- pointer to a user-defined
 *			     function meant to free data held in the array.
 *		    size: int -- the size of the queue.
 *
 * RETURN:	    (qarray *) -- pointer to the new queue, or NULL.
 *
 * NOTES:	    O(1)
 ***/
qarray * qarray_create(void (*destroy)(void *), int size)
{
  if (size <= 0)
    return NULL;

  qarray * q = NULL;
  if ((q = malloc(sizeof(qarray))) == NULL)
    return NULL;

  q->queue = NULL;
  if ((q->queue = calloc(size, sizeof(void *))) == NULL)
    return NULL;
  
  q->destroy = destroy;
  q->front = -1;
  q->back = 0;
  q->capacity = size;
  q->size = 0;

  return q;
}

/******************************************************************************
 * FUNCTION:	    qarray_enqueue
 *
 * DESCRIPTION:	    Adds a new element to the stern of the queue.
 *
 * ARGUMENTS:	    queue: (qarray *) -- the queue to be operated on.
 *		    data: (void *) -- pointer to the data to be enqueued.
 *
 * RETURN:	    int -- 0 on success, -1 otherwise.
 *
 * NOTES:	    O(1)
 ***/
int qarray_enqueue(qarray * queue, void * data)
{
  if (queue == NULL || qarray_isfull(queue))
    return -1;
  
  queue->queue[queue->back] = data;
  queue->back = (queue->back + 1) % queue->capacity;

  if (queue->front == -1)
    queue->front = queue->back - 1;

  queue->size++;
  return 0;
}    

/******************************************************************************
 * FUNCTION:	    qarray_dequeue
 *
 * DESCRIPTION:	    Removes the next element from the queue.
 *
 * ARGUMENTS:	    queue: (qarray *) -- the queue to be operated on.
 *		    data: (void **) -- data is placed here after dequeueing.
 *
 * RETURN:	    int -- 0 on success, -1 otherwise.
 *
 * NOTES:	    O(1)
 ***/
int qarray_dequeue(qarray * queue, void ** data)
{
  if (queue == NULL || qarray_isempty(queue))
    return -1;

  *data = queue->queue[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;

  queue->size--;

  if (qarray_isempty(queue))
    queue->front = -1;

  return 0;
}

/******************************************************************************
 * FUNCTION:	    qarray_peek
 *
 * DESCRIPTION:	    Returns the next element in the queue without removing it.
 *
 * ARGUMENTS:	    queue: (qarray *) -- the queue to be operated on.
 *
 * RETURN:	    (void *) -- pointer to the next element in the queue, or
 *		    NULL if the queue is empty.
 *
 * NOTES:	    O(1)
 ***/
void * qarray_peek(qarray * queue)
{
  if (qarray_isempty(queue))
    return NULL;

  return queue->queue[queue->front];
}

/******************************************************************************
 * FUNCTION:	    qarray_destroy
 *
 * DESCRIPTION:	    Removes all elements from the queue, and sets all bytes to
 *		    0. If destroy is set to NULL, does not free the memory held
 *		    in the queue.
 *
 * ARGUMENTS:	    queue: (qarray **) -- the queue to be operated on.
 *
 * RETURN:	    void.
 *
 * NOTES:	    O(n)
 ***/
void qarray_destroy(qarray ** queue)
{
  if (queue == NULL || *queue == NULL)
    return;

  if ((*queue)->destroy == NULL)
    goto free_struc;

  int i = (*queue)->front;
  while (i != (*queue)->back) {
    if (i < (*queue)->capacity && (*queue)->queue[i] != NULL)
      (*queue)->destroy((*queue)->queue[i]);
    i = (i + 1) % (*queue)->capacity;
  }

 free_struc:
  free((*queue)->queue);
  free(*queue);
  *queue = NULL;
  return;
}

/******************************************************************************
 * MAIN
 ***/

#ifdef CONFIG_DEBUG_QARRAY
int main(int argc, char * argv[])
{
  int * p = NULL;
  qarray * q = NULL; /* Pointer to queue */
 
  q = qarray_create(NULL, 10); /* initialize */
  assert(q != NULL);

  srand((unsigned)time(NULL));

  printf("===== Inserting ======\n");
  for (int i = 0; i < 10; i++) {
    p = malloc(sizeof(int));
    *p = rand() % 10;
    printf("int %d @ %p\n", *p, p);
    qarray_enqueue(q, (void *)p); /* queue up the pointer. */
  }
  
  /* Test to ensure that enqueueing on a full queue returns -1. */
  p = malloc(sizeof(int));
  *p = 0;
  assert(qarray_enqueue(q, (void *)p) == -1);
  free(p);

  printf("====== Removing ======\n");
  while (!qarray_isempty(q)) {
    qarray_dequeue(q, (void **)&p);
    printf("int %d @ %p\n", *p, p);
    free(p);
  }

  /* Test to ensure that dequeueing from an empty queue returns -1. */
  assert(qarray_dequeue(q, (void **)&p) == -1);

  /* Must call qarray_create() in order to be able to use the queue again. */
  qarray_destroy(&q);
  assert(q == NULL);

  return 0;
}
#endif /* CONFIG_DEBUG_ARRAY */

/*****************************************************************************/
