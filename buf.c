/**
 * buf.c
 * The file that contains the implementation
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buf.h"

status_code_t get(mpmc_queue_t *queue, queue_entry_t *entry) {

  if (!queue->ready[queue->tail]) {
    return BUSY;
  }

  if (queue == NULL || entry == NULL) {
    return INVALID;
  }

  // Check if the slot at the head is available
  if (!queue->ready[queue->head]) {
    return BUSY;
  }

  // Calculate the new head
  uint32_t new_tail = (queue->tail + 1) % queue->capacity;

  // Check for empty
  if (abs((int)(new_tail - queue->tail)) == 0) {
    return EMPTY;
  }

  // Mark current tail as not ready
  queue->ready[queue->tail] = false;

  // Copy the new entry into the array
  memcpy(entry, (char *)queue->array + queue->tail * queue->element_size,
         queue->element_size);
  // Mark current tail as ready
  queue->ready[queue->tail] = true;

  // Mark new tail as ready and update the head pointer
  queue->tail = new_tail;
  queue->ready[new_tail] = true;

  return SUCCESS;
}

status_code_t put(mpmc_queue_t *queue, queue_entry_t *entry) {
  if (queue == NULL || entry == NULL) {
    return INVALID;
  }

  // Check if the slot at the head is available
  if (!queue->ready[queue->head]) {
    return BUSY;
  }

  // Calculate the new head
  uint32_t new_head = (queue->head + 1) % queue->capacity;

  // Check for capacity overflow
  if (abs((int)(new_head - queue->tail)) >= queue->capacity) {
    if (queue->overwrite_behavior == OVERWRITE) {
      printf("WARNING: Overwriting entr[(ies)|y]\n");
      // Advance the tail to make room
      queue->tail = (queue->tail + 1) % queue->capacity;
    } else if (queue->overwrite_behavior == FAIL) {
      return FULL;
    }
  }

  // Mark current head as not ready
  queue->ready[queue->head] = false;

  // Copy the new entry into the array
  memcpy((char *)queue->array + queue->head * queue->element_size, entry,
         queue->element_size);
  // Mark current head as ready
  queue->ready[queue->head] = true;

  // Mark new head as ready and update the head pointer
  queue->ready[new_head] = true;
  queue->head = new_head;

  return SUCCESS;
}

status_code_t init(mpmc_queue_t *queue, uint32_t capacity,
                   uint32_t element_size,
                   overwrite_behavior_t overwrite_behavior) {

  // If queue is NULL, malloc a new queue
  if (queue == NULL) {
    queue = malloc(sizeof(mpmc_queue_t));
    if (queue == NULL) {
      // Malloc() call failed.
      return FAILURE;
    }
  }

  queue->array = malloc(element_size * capacity);
  if (queue->array == NULL) {
    return FAILURE;
  }

  queue->ready = malloc(sizeof(bool) * capacity);
  if (queue->ready == NULL) {
    return FAILURE;
  }

  // Mark all as ready
  for (uint32_t i = 0; i < capacity; i++) {
    queue->ready[i] = true;
  }

  queue->element_size = element_size;
  queue->capacity = capacity;

  if ((int)overwrite_behavior >= (int)OVERWRITE_ENUM_MAX) {
    queue->overwrite_behavior = FAIL;
  } else {
    queue->overwrite_behavior = overwrite_behavior;
  }

  queue->head = 0;
  queue->tail = 0;

  return SUCCESS;
}

status_code_t destroy(mpmc_queue_t *queue) {
  PTR_FREE(queue->array);
  PTR_FREE(queue->ready);
  memset(queue, 0, sizeof(mpmc_queue_t));
  return SUCCESS;
}

status_code_t set_overwrite_behavior(mpmc_queue_t *queue,
                                     overwrite_behavior_t overwrite_behavior) {

  if (queue == NULL || (int)overwrite_behavior >= (int)OVERWRITE_ENUM_MAX) {
    return INVALID;
  }

  queue->overwrite_behavior = overwrite_behavior;

  return SUCCESS;
}
