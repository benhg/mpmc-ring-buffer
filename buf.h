#ifndef BUF_H
#define BUF_H

/**
 * The header file for the MPMC ring buffer
 * The data structure and function declarations live here
 * 
 * This buffer behaves as a queue (first-in, first-out)
 * 
 * They are implemented in buf.c
 */

/**
 * The queue type uses void* right now
 * Modify it to use another type if you want
 * make sure either sizeof() works, or you set the size correctly with the init function
 * Or both
 */
typedef void* queue_entry_t;

/**
 * The buffer struct
 */
typedef struct mpmc_queue {
	queue_entry_t * array; //< The actual storage array
	bool * ready; //< A list of bools equal in length to the array. Used for synchronization
				  // These should be accessed atomically. If you use this in C++, you can make the variables atomic.
				  // In C, make sure you access them correctly.
				  // N.B. C atomics semantics bother me. A /variable/ cannot be atomic. An /operation/ on that variable is what's atomic. For example, a compare and swap or increment (both forms of read-modify-write) may be done atomically. The variable itself is just a variable. It's not atomic.
	uint32_t capacity;
	uint32_t head;
	uint32_t tail;
	uint32_t element_size;
} mpmc_queue_t;

int get(mpmc_queue_t* queue, queue_entry_t* entry);

int put(mpmc_queue_t* queue, queue_entry_t* entry);

int init(mpmc_queue_t *queue, int capacity, uint32_t element_size);

int destroy(mpmc_queue_t *queue);


#endif