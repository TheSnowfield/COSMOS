#ifndef _SYS_SCHEDULER_H
#define _SYS_SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct task task_t;
typedef void (* cb_task_t)(task_t *);

struct task {
  uint32_t interval;
  void* userdat;
  bool sleep;
  cb_task_t taskcb;
};

#ifdef _TASK_INTERNAL

typedef struct task_node task_node_t;

struct task_node {
  task_node_t* next;
  task_t* task;
  int32_t remain;
};

typedef struct {
  uint32_t last_tick;
  uint32_t task_time;
  uint32_t last_task_time;
} task_idle_t;

typedef struct {
  task_node_t* head;
  task_node_t* tail;
  task_node_t* context;
  uint32_t tick;
  uint8_t size;
  bool changed;
  task_idle_t idle;
} task_list_t;

#endif // !_TASK_INTERNAL

/**
 * @brief init scheduler
 */
void scheduler_init();

/**
 * @brief scheduler tick increment
 */
void scheduler_tick_inc();

/**
 * @brief scheduler tasks handler
 */
void scheduler_handler();

/**
 * @brief add new task
 * @param interval interval milliseconds
 * @param cb task callback
 * @param userdat user data
 */
task_t* scheduler_add_task(uint32_t interval, cb_task_t cb, void* userdat);

/**
 * @brief remove task
 * @param task task instance
 */
bool scheduler_remove_task(task_t* task);

/**
 * @brief clear all tasks
 */
void scheduler_clear_task();

/**
 * @brief perform a task once
 * @param delay delay time
 * @param cb callback
 */
task_t* scheduler_run_task(uint32_t delay, cb_task_t cb);

/**
 * @brief perform a task once
 * @param proc proc address
 */
void scheduler_call_proc( void* proc);

/**
 * @brief perform a task once
 * @param delay delay time
 * @param proc proc address
 */
void scheduler_call_proc_ex(uint32_t delay, void* proc);

/**
 * @brief sleep milliseconds
 * @param milliseconds ms
 */
void scheduler_sleep(uint32_t milliseconds);

/**
 * @brief sleep milliseconds
 * @param t task instance
 * @param milliseconds ms
 */
void scheduler_sleep_ex(task_t* t, uint32_t milliseconds);

/**
 * @brief await a function
 * @param t task instance
 * @param func call proc
 */
void scheduler_await_ex(task_t* t, bool func());

/**
 * @brief await a function
 * @param func call proc
 */
void scheduler_await(bool func());

/**
 * @brief get current task
 * @return task_t* task instance
 */
task_t* scheduler_get_current_task();

/**
 * @brief get cpu usage
 * @return float usage ratio
 */
float scheduler_get_cpu_usage();

#endif /* _SYS_SCHEDULER_H */
