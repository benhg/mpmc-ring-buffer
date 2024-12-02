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

int test_enqueue_full() {
  mpmc_queue_t queue;
  status_code_t status;

  // Initialize the queue with a small capacity
  status = init(&queue, 3, sizeof(int), FAIL);
  assert(status == SUCCESS);

  int values[] = {1, 2, 3};
  int value_to_enqueue = 4;

  // Fill the queue
  for (int i = 0; i < 3; i++) {
    status = put(&queue, &values[i]);
    printf("%d\n", status);
    assert(status == SUCCESS);
  }

  // Attempt to enqueue when the queue is full
  status = put(&queue, &value_to_enqueue);
  assert(status == FULL);
  printf("Test 3: Enqueue when full passed.\n");

  // Destroy the queue
  status = destroy(&queue);
  assert(status == SUCCESS);

  return 0;
}

int test_overwrite_behavior() {
  mpmc_queue_t queue;
  status_code_t status;

  // Initialize the queue with a small capacity and allow overwriting
  status = init(&queue, 2, sizeof(int), OVERWRITE);
  assert(status == SUCCESS);

  int values[] = {10, 20, 30};
  int dequeued_value = 0;

  // Fill the queue
  for (int i = 0; i < 2; i++) {
    status = put(&queue, &values[i]);
    assert(status == SUCCESS);
  }

  // Add another item to overwrite the oldest
  status = put(&queue, &values[2]);
  assert(status == SUCCESS);

  // Dequeue items and verify the oldest was overwritten
  status = get(&queue, &dequeued_value);
  assert(status == SUCCESS);
  assert(dequeued_value == 20); // 10 was overwritten
  printf("Test 4: Overwrite behavior passed.\n");

  // Destroy the queue
  status = destroy(&queue);
  assert(status == SUCCESS);

  return 0;
}

int main() {
  test_init_destroy();
  test_enqueue_dequeue();
  test_enqueue_full();
  test_overwrite_behavior();
  return 0;
}
