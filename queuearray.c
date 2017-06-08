/*******************************************************************************
 * NAME:	    queuearray.c
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    Contains the implementation of a Queue, using Arrays, as the
 *		    name suggests. Also contains test code. Run 'make debug' to
 *		    compile with the test code.
 *
 * CREATED:	    04/14/17
 *
 * LAST EDITED:	    06/07/17
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

/*******************************************************************************
 * INCLUDES
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef CONFIG_DEBUG_QARRAY
#include <assert.h>
#include <time.h>
#endif /* CONFIG_DEBUG_QARRAY */

#include "queuearray.h"

/*******************************************************************************
 * API FUNCTIONS
 ***/

/*******************************************************************************
 * FUNCTION:	    queue_init
 *
 * DESCRIPTION:	    Initializes a queue pointer to the size specified.
 *
 * ARGUMENTS:	    queue: (Queue *) -- the queue to be initialized.
 *		    destroy: (void (*)(void *)) -- pointer to a user-defined
 *			     function meant to free data held in the array.
 *		    size: int -- the size of the queue.
 *
 * RETURN:	    void.
 *
 * NOTES:	    O(1)
 ***/
void queue_init(Queue * queue, void (*destroy)(void *), int size)
{
  queue->queue = calloc(size, sizeof(void *));
  queue->destroy = destroy;
  queue->front = -1;
  queue->back = 0;
  queue->capacity = size;
  queue->size = 0;
}

/*******************************************************************************
 * FUNCTION:	    queue_enqueue
 *
 * DESCRIPTION:	    Adds a new element to the stern of the queue.
 *
 * ARGUMENTS:	    queue: (Queue *) -- the queue to be operated on.
 *		    data: (void *) -- pointer to the data to be enqueued.
 *
 * RETURN:	    int -- 0 on success, -1 otherwise.
 *
 * NOTES:	    O(1)
 ***/
int queue_enqueue(Queue * queue, void * data)
{

  if (queue_isfull(queue))
    return -1;

  
  queue->queue[queue->back] = data;
  queue->back = (queue->back + 1) % queue->capacity;

  if (queue->front == -1)
    queue->front = queue->back - 1;

  queue->size++;
  return 0;
}    

/*******************************************************************************
 * FUNCTION:	    queue_dequeue
 *
 * DESCRIPTION:	    Removes the next element from the queue.
 *
 * ARGUMENTS:	    queue: (Queue *) -- the queue to be operated on.
 *		    data: (void **) -- the data is placed here after dequeueing.
 *
 * RETURN:	    int -- 0 on success, -1 otherwise.
 *
 * NOTES:	    O(1)
 ***/
int queue_dequeue(Queue * queue, void ** data)
{
  
  if (queue_isempty(queue))
    return -1;

  *data = queue->queue[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;

  queue->size--;

  if (queue_isempty(queue))
    queue->front = -1;

  return 0;
}

/*******************************************************************************
 * FUNCTION:	    queue_peek
 *
 * DESCRIPTION:	    Returns the next element in the queue without removing it.
 *
 * ARGUMENTS:	    queue: (Queue *) -- the queue to be operated on.
 *
 * RETURN:	    (void *) -- pointer to the next element in the queue, or
 *		    NULL if the queue is empty.
 *
 * NOTES:	    O(1)
 ***/
void * queue_peek(Queue * queue)
{
  if (queue_isempty(queue))
    return NULL;

  return queue->queue[queue->front];
}

/*******************************************************************************
 * FUNCTION:	    queue_dest
 *
 * DESCRIPTION:	    Removes all elements from the queue, and sets all bytes to
 *		    0. If destroy is set to NULL, does not free the memory held
 *		    in the queue.
 *
 * ARGUMENTS:	    queue: (Queue *) -- the queue to be operated on.
 *
 * RETURN:	    void.
 *
 * NOTES:	    O(n)
 ***/
void queue_dest(Queue * queue)
{
  int i = queue->front;
  while (i < queue->back) {
    if (queue->queue[i] != NULL && queue->destroy != NULL)
      queue->destroy(queue->queue[i]);
    else if (queue->queue[i] != NULL)
      free(queue->queue[i]);

    i = (i + 1) % queue->capacity;
  }
    
  memset(queue, 0, sizeof(Queue));
  return;
}

/*******************************************************************************
 * MAIN
 ***/

#ifdef CONFIG_DEBUG_QARRAY
int main(int argc, char * argv[])
{
  int * p;
  Queue * q; /* Pointer to Queue */
 
  q = malloc(sizeof(Queue));
  queue_init(q, NULL, 10); /* initialize */

  srand((unsigned)time(NULL));

  printf("===== Inserting ======\n");
  for (int i = 0; i < 10; i++) {
    p = malloc(sizeof(int));
    *p = rand() % 10;
    printf("int %d @ %p\n", *p, p);
    queue_enqueue(q, (void *)p); /* Queue up the pointer. */
  }
  
  /* Test to ensure that enqueueing on a full queue returns -1. */
  p = malloc(sizeof(int));
  *p = 0;
  assert(queue_enqueue(q, (void *)p) == -1);
  free(p);

  printf("====== Removing ======\n");
  while (!queue_isempty(q)) {
    queue_dequeue(q, (void **)&p);
    printf("int %d @ %p\n", *p, p);
    free(p);
  }

  /* Test to ensure that dequeueing from an empty queue returns -1. */
  assert(queue_dequeue(q, (void **)&p) == -1);

  /* Must call init() in order to be able to use the Queue again. */
  queue_dest(q);
  free(q);

  return 0;
}
#endif /* CONFIG_DEBUG_ARRAY */

/******************************************************************************/
