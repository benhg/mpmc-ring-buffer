/**
 * buf.c
 * The file that contains the implementation
 */

/**
 * The queue type uses void* right now
 * Modify it to use another type if you want
 * make sure either sizeof() works, or you set the size correctly with the init function
 * Or both
 */
typedef void* queue_entry_t;

typedef enum overwrite_behavior {
    FAIL = 0,
    OVERWRITE = 1,
    OVERWRITE_ENUM_MAX = 2
} overwrite_behavior_t;

typedef enum status_code {
    SUCCESS = 0,
    FAILURE = 1,
    FULL = 2,

    BUSY = 3,
    UNAVAILABLE = 4,
    STATUS_ENUM_MAX
} status_code_t;

/**
 * The buffer struct
 */
typedef struct mpmc_queue {
    queue_entry_t * array; //< The actual storage array
    bool * ready; //< A list of bools equal in length to the array. Used for synchronization
                  // These should be accessed atomically. If you use this in C++, you can make the variables atomic.
                  // In C, make sure you access them correctly.
                  // N.B. C++ atomics semantics bother me. A /variable/ cannot be atomic. An /operation/ on that variable is what's atomic. For example, a compare and swap or increment (both forms of read-modify-write) may be done atomically. The variable itself is just a variable. It's not atomic.
    uint32_t capacity;
    uint32_t head;
    uint32_t tail;
    uint32_t element_size;
    overwrite_behavior_t overwrite_behavior; // Overwrite if true, return failure if false (when at capacity)
} mpmc_queue_t;

status_code_t get(mpmc_queue_t* queue, queue_entry_t* entry){

    if (queue == NULL){
        return INVALID;
    }

    if (queue->head == queue->tail){
        return EMPTY;
    }

    if (!queue->ready[tail]){
        return BUSY;
    }

    // Entry must be allocated by caller
    if (queue_entry_t == NULL){
        return INVALID;
    }

    // Deassert ready on old tail
    queue->ready[tail] = false;

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
            printf("WARNING: Overwriting entries\n")
        } else if (queue->overwrite_behavior == FAIL){
            return FULL;
        }
    }

    if (!queue->ready[tail]){
        return BUSY;
    }

    // Entry must be allocated by caller
    if (queue_entry_t == NULL){
        return INVALID;
    }

    // Deassert ready on old tail
    queue->ready[tail] = false;

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
        queue = malloc(sizeof (mpmc_queue));
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
    if (overwrite_behavior == NULL){
        overwrite_behavior = FAIL;
    } else{
        overwrite_behavior = overwrite_behavior;
    }

    head = 0;
    tail = 0;
}

status_code_t destroy(mpmc_queue_t *queue){
    PTR_FREE(queue->array);
    PTR_FREE(queue->ready);
    PTR_FREE(queue); 
    return SUCCESS;
}

status_code_t set_overwrite_behavior(mpmc_queue_t *queue, bool overwrite_behavior){
    if (queue == NULL){
        return FAILURE;
    }

    queue->overwrite_behavior = overwrite_behavior;
}
