#define _TASK_INTERNAL
#include <sys/scheduler.h>
#undef _TASK_INTERNAL

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint32_t tick;
static uint32_t last_tick;
static task_list_t task_list;

void scheduler_init() {
  task_list.head = NULL;
  task_list.tail = NULL;
  task_list.size = 0;

  tick = 0;
  last_tick = 0;
}

void scheduler_tick_inc() {
  ++tick;
}

void scheduler_handler() {
  uint32_t time = tick; tick = 0;
  if(!task_list.size) return;

  task_node_t* node = task_list.head;

  // Enum tasks
  while (node != NULL) {

    node->remain -= time;
    
    // Check the task remain
    if(node->remain <= 0) {
      node->remain = node->task->interval;
      node->task->taskcb(node->task);
    }

    // If task list is changed
    // restart the loop and drop the context
    if(task_list.changed) {
      task_list.changed = false;
      return;
    }

    node = node->next;
  }
}

task_t* scheduler_add_task(uint32_t interval, cb_task_t cb, void* userdat) {
  task_node_t* node = malloc(sizeof(task_node_t));

  // Create a task
  task_t *task = malloc(sizeof(task_t)); {
    task->interval = interval;
    task->userdat = userdat;
    task->taskcb = cb;
  };

  // Setup task node
  node->task = task;
  node->remain = interval;
  node->next = NULL;

  // Add task to list
  if(task_list.head == NULL) {
    task_list.head = node;
    task_list.tail = node;
  } else {
    task_list.tail->next = node;
    task_list.tail = node;
  }

  ++task_list.size;
  task_list.changed = true;

  return task;
}

bool scheduler_remove_task(task_t* task) {
  task_node_t* node = task_list.head;
  task_node_t* prev = NULL;

  while (node != NULL) {

    if(node->task == task) {

      // The task located in the first node
      if(prev == NULL) {
        task_list.head = node->next;

      // Other cases
      } else {
        prev->next = node->next;
        if(task_list.tail->task == task) {
          task_list.tail = prev;
        }
      }

      free(node);
      free(task);

      --task_list.size;
      task_list.changed = true;

      return true;
    }

    prev = node;
    node = node->next;
  }

  return false;
}

// void scheduler_postrun(uint32_t delay, cb_task_t cb, void* userdat) {

// }
