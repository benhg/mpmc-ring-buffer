# Thread-Safe Lock-Free Queue

A high-performance, thread-safe, lock-free multi-producer, multi-consumer (MPMC) queue implemented as a ring buffer. This queue is designed for low-latency and high-concurrency scenarios, making it ideal for use in real-time systems, multithreaded applications, and other performance-critical environments.

# Features

- Thread-Safe: Supports concurrent access by multiple producers and consumers without requiring locks.
- Lock-Free: Optimized for minimal contention and maximum performance.
- Ring Buffer Implementation: Efficient use of memory with predictable performance.
- Customizable Overwrite Behavior: Choose between failure on full or overwriting the oldest entries.

# API Overview

The library provides the following functions:

## Initialization and Cleanup

- `status_code_t init(mpmc_queue_t *queue, uint32_t capacity, uint32_t element_size, overwrite_behavior_t overwrite_behavior);`
	- Initializes the queue with the specified capacity and behavior.
- `status_code_t destroy(mpmc_queue_t *queue);`
	- Cleans up resources associated with the queue.

## Data Operations

- `status_code_t put(mpmc_queue_t *queue, queue_entry_t *entry);`
	- Adds an entry to the queue. Behavior depends on the overwrite policy.
- `status_code_t get(mpmc_queue_t *queue, queue_entry_t *entry);`
	- Retrieves an entry from the queue.

## Configuration

- status_code_t `set_overwrite_behavior(mpmc_queue_t *queue, bool overwrite_behavior);`
	- Updates the overwrite behavior of the queue.

# Status Codes


The API functions return status_code_t values to indicate the result of operations:

Code	Description
SUCCESS	Operation completed successfully.
FAILURE	General error occurred.
FULL	Queue is full (and no overwrite allowed).
EMPTY	Queue is empty.
BUSY	Queue is busy (e.g., resource contention).
UNAVAILABLE	Resource is unavailable.
INVALID	Invalid arguments were provided.

# Usage

Example
Below is an example of basic usage:
```
#include "mpmc_queue.h"
#include <stdio.h>

int main() {
    mpmc_queue_t queue;
    int value = 42, result;

    // Initialize the queue
    if (init(&queue, 10, sizeof(int), FAIL) != SUCCESS) {
        printf("Failed to initialize queue.\n");
        return 1;
    }

    // Add an item to the queue
    if (put(&queue, &value) == SUCCESS) {
        printf("Successfully enqueued value: %d\n", value);
    }

    // Retrieve the item
    if (get(&queue, &result) == SUCCESS) {
        printf("Successfully dequeued value: %d\n", result);
    }

    // Cleanup
    destroy(&queue);
    return 0;
}
```

# Tests

The project includes a comprehensive set of test cases to validate functionality. Run the main.c file to execute all tests:

```
gcc main.c -o test_queue
./test_queue
```

## Included Test Cases

- Initialization and Destruction
- Basic Enqueue and Dequeue
- Enqueue Until Full
- Overwrite Behavior


# Directory Structure

```
mpmc-ring-buffer/
├── test.c            # Test cases
├── buf.c      		  # Queue implementation
├── buf.h             # Queue API header
├── README.md         # Project documentation
└── Makefile          # Build system
```

## Building

To build the library and test cases, use the provided Makefile:



