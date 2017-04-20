/*
 * TITLE: queue.h -- my custom implementation of a circular queue, using arrays.
 * AUTHOR: Ethan D. Twardy
 * CREATED: 04/05/17
 * LAST EDITED: 04/05/17
 */

#ifndef __ET_QUEUE__
#define __ET_QUEUE__

#include <string.h>

/************* TYPE DEFINITIONS *************/
/********************************************/

typedef struct _Queue_ {

  void (*destroy)(void * data);
  int front;
  int back;
  int capacity;
  int size;
  void ** queue;

} Queue;

/************* MACRO DEFINITIONS ************/
/********************************************/

#define queue_size(queue) ((queue)->size)
#define queue_isempty(queue) (queue_size(queue) == 0 ? 1 : 0)
#define queue_isfull(queue) (queue_size(queue) == (queue)->capacity ? 1 : 0)

/************* PUBLIC INTERFACE ************/
/*******************************************/

// FUNCTION: init O(1) -- initializes the queue to the size specified in size.
// PARAMETERS: queue: (Queue *) -- the queue to be initialized.
//             destroy: void (*)(void *) -- user defined function to free data
// RETURN: void
void queue_init(Queue * queue, void (*destroy)(void *), int size)
{
  queue->queue = calloc(size, sizeof(void *));
  queue->destroy = destroy;
  queue->front = -1;
  queue->back = 0;
  queue->capacity = size;
  queue->size = 0;
}

// FUNCTION: enquque O(1) -- adds a new element to the queue
// PARAMETERS:
// RETURN: int -- -1 on failure, 0 on success
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

// FUNCTION: dequeue O(1) -- removes the next element from the queue.
// PARAMETERS:
// RETURN: int -- -1 on failure, 0 on success.
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

// FUNCTION: peek O(1) -- returns but does not remove the next element
//           in the queue.
// PARAMETERS:
// RETURN: (void *) -- the next element in the queue.
void * queue_peek(Queue * queue)
{
  if (queue_isempty(queue))
    return NULL;

  return queue->queue[queue->front];
}

// FUNCTION: dest O(n) -- removes all elements from the queue and sets
//           all bytes of memory in the queue to 0.
// PARAMETERS: queue: (Queue *) -- the queue to be operated on.
// RETURN: none.
void queue_dest(Queue * queue)
{
  /*int i = queue->front;
  while (i < queue->back) {
    if (queue->queue[i] != NULL && queue->destroy != NULL)
      queue->destroy(queue->queue[i]);
    else if (queue->queue[i] != NULL)
      free(queue->queue[i]);

    i = (i + 1) % queue->capacity;
    }
    
    free(queue->queue);*/
  memset(queue, 0, sizeof(Queue));
  return;
}

#endif // __ET_QUEUE__
