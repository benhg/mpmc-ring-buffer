/**
 * buf.c
 * The file that contains the implementation
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "buf.h"

status_code_t get(mpmc_queue_t* queue, queue_entry_t* entry){

    if (queue == NULL){
        return INVALID;
    }

    if (queue->head == queue->tail){
        return EMPTY;
    }

    if (!queue->ready[queue->tail]){
        return BUSY;
    }

    // Entry must be allocated by caller
    if (entry == NULL){
        return INVALID;
    }

    // Deassert ready on old tail
    queue->ready[queue->tail] = false;

    // Memcpy the entry
    // This is always an operation of size 1, so just use element size
    memcpy(entry, queue->array[queue->tail], queue->element_size);

    queue->tail = ((++queue->tail) % queue->capacity);

    // Assert ready on old tail
    queue->ready[--queue->tail] = true;

    // Assert ready on new tail
    queue->ready[queue->tail] = true;

    return SUCCESS;
}

status_code_t put(mpmc_queue_t* queue, queue_entry_t* entry){
    
    if (queue == NULL){
        return INVALID;
    }

    if (queue->head == queue->tail){
        if (queue->overwrite_behavior == OVERWRITE){
            printf("WARNING: Overwriting entr[(ies)|y]\n");
        } else if (queue->overwrite_behavior == FAIL){
            return FULL;
        }
    }

    if (!queue->ready[queue->tail]){
        return BUSY;
    }

    // Entry must be allocated by caller
    if (entry == NULL){
        return INVALID;
    }

    // Deassert ready on old tail
    queue->ready[queue->tail] = false;

    // Memcpy the entry
    // This is always an operation of size 1, so just use element size
    memcpy(entry, queue->array[queue->tail], queue->element_size);

    queue->tail = ((++queue->tail) % queue->capacity);

    // Assert ready on old tail
    queue->ready[--queue->tail] = true;

    // Assert ready on new tail
    queue->ready[queue->tail] = true;

    return SUCCESS;
}

status_code_t init(mpmc_queue_t *queue, uint32_t capacity, uint32_t element_size, overwrite_behavior_t overwrite_behavior){

    // If queue is NULL, malloc a new queue
    if (queue == NULL){
        queue = malloc(sizeof (mpmc_queue_t));
        if (queue == NULL){
            // Malloc() call failed.
            return FAILURE;
        }
    }

    queue->array = malloc(element_size * capacity);
    if (queue->array == NULL){
        return FAILURE;
    }

    queue->ready = malloc(sizeof(bool) * capacity);
    if (queue->ready == NULL){
        return FAILURE;
    }

    queue->element_size = element_size;
    queue->capacity = capacity;

    if ((int)overwrite_behavior >= (int)OVERWRITE_ENUM_MAX){
        queue->overwrite_behavior = FAIL;
    } else{
        queue->overwrite_behavior = overwrite_behavior;
    }

    queue->head = 0;
    queue->tail = 0;

    return SUCCESS;
}

status_code_t destroy(mpmc_queue_t *queue){
    PTR_FREE(queue->array);
    PTR_FREE(queue->ready);
    PTR_FREE(queue); 
    return SUCCESS;
}

status_code_t set_overwrite_behavior(mpmc_queue_t *queue, overwrite_behavior_t overwrite_behavior){
    
    if (queue == NULL || (int) overwrite_behavior >= (int) OVERWRITE_ENUM_MAX ){
        return INVALID;
    }

    queue->overwrite_behavior = overwrite_behavior;

    return SUCCESS;
}
