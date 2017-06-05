/*
 * NAME: queue,c -- a test file for the header in queue.h
 * AUTHOR: Ethan D. Twardy
 * CREATED: 04/14/17
 * LAST EDITED: 04/14/17
 */

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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "queuearray.h"

int main(int argc, char * argv[])
{
  int * p; // Use a pointer to the data type for reference.
  Queue * q; // Pointer to Queue
 
  q = malloc(sizeof(Queue));
  queue_init(q, NULL, 10); // initialize

  srand((unsigned)time(NULL));

  printf("===== Inserting ======\n");
  for (int i = 0; i < 10; i++) {
    p = malloc(sizeof(int)); // Allocate space on the heap.
    *p = rand() % 10; // Assign a value to the space.
    printf("int %d @ %p\n", *p, p);
    queue_enqueue(q, (void *)p); // Queue up the pointer.
  }
  
  // Test to ensure that enqueueing on a full queue returns -1.
  p = malloc(sizeof(int));
  *p = 0;
  assert(queue_enqueue(q, (void *)p) == -1);
  free(p);

  printf("====== Removing ======\n");
  while (!queue_isempty(q)) {
    queue_dequeue(q, (void **)&p); // Dequeue.
    printf("int %d @ %p\n", *p, p);
    free(p); // Free the memory that p points to.
  }

  // Test to ensure that dequeueing from an empty queue returns -1.
  assert(queue_dequeue(q, (void **)&p) == -1);

  // Set all bytes of the queue to 0.
  // Must call init() in order to be able to use the Queue again.
  queue_dest(q);
  free(q);

  return 0;
}
