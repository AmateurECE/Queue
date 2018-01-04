/******************************************************************************
 * TITLE:	    qarray.h
 *
 * AUTHOR:	    Ethan D. Twardy
 *
 * DESCRIPTION:	    Header file containing macros and typedefs for the C code
 *		    in qarray.c.
 *
 * CREATED:	    04/05/2017
 *
 * LAST EDITED:	    01/04/2018
 ***/

#ifndef __ET_QARRAY_H__
#define __ET_QARRAY_H__

/******************************************************************************
 * TYPE DEFINITIONS
 ***/

typedef struct {

  void (*destroy)(void *);
  int front;
  int back;
  int capacity;
  int size;
  void ** queue;

} qarray;

/******************************************************************************
 * MACRO DEFINITIONS
 ***/

#define qarray_size(queue) ((queue)->size)
#define qarray_isempty(queue) (qarray_size(queue) == 0 ? 1 : 0)
#define qarray_isfull(queue) (qarray_size(queue) == (queue)->capacity ? 1 : 0)

/******************************************************************************
 * API FUNCTION PROTOTYPES
 ***/

extern qarray * qarray_create(void (*destroy)(void *), int size);
extern int qarray_enqueue(qarray * queue, void * data);
extern int qarray_dequeue(qarray * queue, void ** pData);
extern void * qarray_peek(qarray * queue);
extern void qarray_destroy(qarray ** pQueue);

#endif /* __ET_QARRAY_H__ */

/*****************************************************************************/
