#ifndef _SYS_BUTTON_H
#define _SYS_BUTTON_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/scheduler.h>

typedef enum {
  button_up,
  button_down,
  button_hold,
  button_hold_click,
  button_release
} button_status_t;

typedef enum {
  btnid_none = -1,
  btnid_a,
  btnid_b,
  btnid_max
} button_id_t;

typedef void (* cb_button_t)(button_id_t id, button_status_t type, void* userdata);
extern void cb_task_check_button(task_t* task);

#ifdef _BUTTON_INTERNAL
#include <hardware/config.h>
#include <sys/scheduler.h>

typedef struct {
  bool intr_changed;
  uint32_t intr_status;
  cb_button_t cb;
  button_status_t status;
  uint32_t holdtick;
  task_t *chkhold;
  void* userdata;
} button_list_t;

extern bool button_changed;
extern button_list_t button_list[];

#endif

/**
 * @brief get button status
 * @param id button id
 * @param hw get immediate status
 * @return button_status_t
 */
button_status_t button_get_status(button_id_t id, bool immediate);

/**
 * @brief set button handler
 * @param id button id
 * @param cb event callback
 * @param userdata user data
 */
void button_set_eventcb(button_id_t id, cb_button_t cb, void* userdata);

#endif /* _SYS_BUTTON_H */
