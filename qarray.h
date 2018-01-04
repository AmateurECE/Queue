/******************************************************************************
 * TITLE:	    qarray.h
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    Header file containing macros and typedefs for the C code
 *		    in queuearray.c.
 *
 * CREATED:	    04/05/17
 *
 * LAST EDITED:	    01/04/2018
 ***/

/* ********************************* NOTE ********************************* */
/* Due to conflicts between function signatures for the public interfaces,  */
/* this structure and the queue using lists cannot be used in the same	    */
/* namespace. This will be patched in a future update.			    */
/* ************************************************************************ */

#ifndef __ET_QUEUEARRAY_H__
#define __ET_QUEUEARRAY_H__

/******************************************************************************
 * TYPE DEFINITIONS
 ***/

typedef struct _Queue_ {

  void (*destroy)(void *);
  int front;
  int back;
  int capacity;
  int size;
  void ** queue;

} Queue;

/******************************************************************************
 * MACRO DEFINITIONS
 ***/

#define queue_size(queue) ((queue)->size)
#define queue_isempty(queue) (queue_size(queue) == 0 ? 1 : 0)
#define queue_isfull(queue) (queue_size(queue) == (queue)->capacity ? 1 : 0)

/******************************************************************************
 * API FUNCTION PROTOTYPES
 ***/

extern void queue_init(Queue *, void (*)(void *), int);
extern int queue_enqueue(Queue *, void *);
extern int queue_dequeue(Queue *, void **);
extern void * queue_peek(Queue *);
extern void queue_dest(Queue *);

#endif /* __ET_QUEUEARRAY_H__ */

/*****************************************************************************/
