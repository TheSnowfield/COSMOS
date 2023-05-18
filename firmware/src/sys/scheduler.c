#define _TASK_INTERNAL
#include <sys/scheduler.h>
#undef _TASK_INTERNAL

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/tick.h>

task_list_t tasklist;

void scheduler_init() {
  tasklist.head = NULL;
  tasklist.tail = NULL;
  tasklist.size = 0;

  tasklist.tick = 0;
  tasklist.idle.last_tick = 0;
}

void scheduler_tick_inc() {
  ++tasklist.tick;
}

void scheduler_handler() {
  uint32_t time = tasklist.tick;
  if(time == 0) return; else tasklist.tick = 0;
  if(!tasklist.size) return;

  // performace counter
  uint32_t start = tick_get_time(); {
    
    // enum tasks
    task_node_t* node = tasklist.head;
    while (node != NULL) {

      // // perform a sleep
      // if(node->task->sleep) {
      //   node = node->next;
      //   continue;
      // }

      // do not call itself or call a sleep task
      if(node == tasklist.context || node->task->sleep) {
        node = node->next;
        continue;
      }

      // check the task remain
      node->remain -= time;
      if(node->remain <= 0) {

        // backup context and switch to new context
        task_node_t* oldctx = tasklist.context;
        tasklist.context = node; {
          node->remain = node->task->interval;
          node->task->taskcb(node->task);
        }

        // restore context
        tasklist.context = oldctx;
      }

      // if task list has changed
      // restart the loop and drop the context
      if(tasklist.changed) {
        tasklist.changed = false;
        return;
      }

      node = node->next;
    }
  }

  uint32_t endup = tick_get_time();

  // update idle time
  if(endup > start) {

    // reset last tick
    if(endup - tasklist.idle.last_tick >= 1000) {
      tasklist.idle.last_tick = endup;
      tasklist.idle.last_task_time = tasklist.idle.task_time;
      tasklist.idle.task_time = 0;
    }
    tasklist.idle.task_time += endup - start;
  }
}

task_t* scheduler_add_task(uint32_t interval, cb_task_t cb, void* userdat) {
  task_node_t* node = malloc(sizeof(task_node_t));
  memset(node, 0x00, sizeof(task_node_t));

  // create a task
  task_t *task = malloc(sizeof(task_t)); {
    memset(task, 0x00, sizeof(task_t));
    task->interval = interval;
    task->userdat = userdat;
    task->taskcb = cb;
  };

  // setup task node
  node->task = task;
  node->remain = interval;
  node->next = NULL;

  // add task to list
  if(tasklist.head == NULL) {
    tasklist.head = node;
    tasklist.tail = node;
  } else {
    tasklist.tail->next = node;
    tasklist.tail = node;
  }

  ++tasklist.size;
  tasklist.changed = true;

  return task;
}

bool scheduler_remove_task(task_t* task) {
  task_node_t* node = tasklist.head;
  task_node_t* prev = NULL;

  // check if task is null
  if(task == NULL) return false;

  while (node != NULL) {

    if(node->task == task) {

      // the task is located in the first node
      if(prev == NULL) {
        tasklist.head = node->next;

      // other cases
      } else {
        prev->next = node->next;
        if(tasklist.tail->task == task) {
          tasklist.tail = prev;
        }
      }

      free(node);
      free(task);

      --tasklist.size;
      tasklist.changed = true;

      return true;
    }

    prev = node;
    node = node->next;
  }

  return false;
}

static inline void __internal_task(task_t * t) {
  ((cb_task_t)t->userdat)(t); scheduler_remove_task(t);
}

static inline void __internal_task_proc(task_t * t) {
  ((void (*)())t->userdat)(); scheduler_remove_task(t);
}

task_t* scheduler_run_task(uint32_t delay, cb_task_t cb) {
  scheduler_add_task(delay, __internal_task, cb);
}

void scheduler_call_proc(void* proc) {
  scheduler_call_proc_ex(0, proc);
}

void scheduler_call_proc_ex(uint32_t delay, void* proc) {
  scheduler_add_task(delay, __internal_task_proc, proc);
}

void scheduler_sleep_ex(task_t* t, uint32_t milliseconds) {
  
  // lock the task
  if(t) t->sleep = true;

  // loop until time exceeds
  uint32_t start = tick_get_time();
  while(tick_get_time() - start < milliseconds)
    scheduler_handler();

  // unlock
  if(t) t->sleep = false;
}

void scheduler_sleep(uint32_t milliseconds) {
  return scheduler_sleep_ex(scheduler_get_current_task(), milliseconds);
}

void scheduler_await_ex(task_t* t, bool func()) {
  // lock the task
  if(t) t->sleep = true;

  // loop until function return true
  while(func()) scheduler_handler();

  // unlock
  if(t) t->sleep = false;
}

void scheduler_await(bool func()) {
  return scheduler_await_ex(scheduler_get_current_task(), func);
}

task_t* scheduler_get_current_task() {
  return tasklist.context->task;
}

float scheduler_get_cpu_usage() {
  return tasklist.idle.last_task_time / 1000;
}
