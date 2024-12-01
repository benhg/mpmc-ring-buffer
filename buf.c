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

int get(mpmc_queue_t* queue, queue_entry_t* entry){

}

int put(mpmc_queue_t* queue, queue_entry_t* entry){

}

int init(mpmc_queue_t *queue, int capacity, uint32_t element_size, overwrite_behavior_t overwrite_behavior){

}

int destroy(mpmc_queue_t *queue){

}

int set_overwrite_behavior(mpmc_queue_t *queue, bool overwrite_behavior){

}
