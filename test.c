/**
 * Test driver file
 */

#include "buf.h"

#include <assert.h>
#include <stdio.h>

int test_init_destroy() {
  mpmc_queue_t queue;
  status_code_t status;

  // Initialize the queue with a capacity of 10 and element size of 4 bytes
  // (e.g., int)
  status = init(&queue, 10, sizeof(int), FAIL);
  assert(status == SUCCESS);
  printf("Test 1: Initialization passed.\n");

  // Destroy the queue
  status = destroy(&queue);
  assert(status == SUCCESS);
  printf("Test 1: Destruction passed.\n");

  return 0;
}

int test_enqueue_dequeue() {
  mpmc_queue_t queue;
  status_code_t status;

  // Initialize the queue
  status = init(&queue, 5, sizeof(int), FAIL);
  assert(status == SUCCESS);

  printf("Test 2: Made queue\n");

  int value_to_enqueue = 42;
  int dequeued_value = 0;

  // Enqueue an item
  status = put(&queue, &value_to_enqueue);
  printf("%d\n", status);
  assert(status == SUCCESS);
  printf("Test 2: Enqueue passed.\n");

  // Dequeue the item
  status = get(&queue, &dequeued_value);
  assert(status == SUCCESS);
  assert(dequeued_value == value_to_enqueue);
  printf("Test 2: Dequeue passed.\n");

  // Destroy the queue
  status = destroy(&queue);
  assert(status == SUCCESS);

  return 0;
}

int main() {
  test_init_destroy();
  test_enqueue_dequeue();
  return 0;
}
