#ifndef _SYS_TASK_H
#define _SYS_TASK_H

#include <stdint.h>
#include <stdbool.h>

typedef struct task task_t;
typedef void (* cb_task_t)(task_t *);

struct task {
  uint32_t interval;
  void* userdat;
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
  task_node_t* head;
  task_node_t* tail;
  uint8_t size;
  bool changed;
} task_list_t;

#endif // !_TASK_INTERNAL

/**
 * @brief Init scheduler
 */
void scheduler_init();

/**
 * @brief Scheduler tick increment
 */
void scheduler_tick_inc();

/**
 * @brief Scheduler tasks handler
 */
void scheduler_handler();

/**
 * @brief Add new task
 * @param interval interval milliseconds
 * @param cb task callback
 * @param userdat user data
 */
task_t* scheduler_add_task(uint32_t interval, cb_task_t cb, void* userdat);

// void scheduler_run_once(uint32_t delay, cb_task_t cb, void* userdat);

/**
 * @brief Remove task
 * @param task task instance
 */
bool scheduler_remove_task(task_t* task);

/**
 * @brief Clear all tasks
 */
void scheduler_clear_task();

#endif // !_SYS_TASK_H
