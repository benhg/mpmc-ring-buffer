#ifndef BUF_H
#define BUF_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * The header file for the MPMC ring buffer
 * The data structure and function declarations live here
 * 
 * This buffer behaves as a queue (first-in, first-out)
 * 
 * They are implemented in buf.c
 */

#define PTR_FREE(x) do { if ((x) != NULL) { free(x); (x) = NULL; } } while (0)

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
    EMPTY = 3,
    BUSY = 4,
    UNAVAILABLE = 5,
    INVALID= 6,
    STATUS_ENUM_MAX = 7
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

status_code_t get(mpmc_queue_t* queue, queue_entry_t* entry);

status_code_t put(mpmc_queue_t* queue, queue_entry_t* entry);

status_code_t init(mpmc_queue_t *queue, uint32_t capacity, uint32_t element_size, overwrite_behavior_t overwrite_behavior);

status_code_t destroy(mpmc_queue_t *queue);

status_code_t set_overwrite_behavior(mpmc_queue_t *queue, bool overwrite_behavior);

#endif