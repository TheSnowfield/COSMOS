#ifndef _SYS_BUTTON_H
#define _SYS_BUTTON_H

#include <stdint.h>

typedef enum {
  button_up,
  button_down,
  button_hold,
} button_status_t;

typedef enum {
  btnid_none = -1,
  btnid_a,
  btnid_b,
} button_id_t;

typedef void (* cb_button_t)(button_status_t type, void* userdata);

#ifdef _BUTTON_INTERNAL
#include <config.h>
#include <sys/scheduler.h>

typedef struct {
  cb_button_t cb;
  button_status_t status;
  task_t *chkhold;
  void* userdata;
} button_list_t;


void EXTI4_15_IRQHandler(void) {
  // HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN_KEY_B);
  // HAL_NVIC_DisableIRQ(EXTI2_IRQn);
}

#endif

/**
 * @brief Init buttons
 */
void button_init();

/**
 * @brief Set button handler
 * @param id button id
 * @param cb event callback
 * @param userdata user data
 */
void button_set_handler(button_id_t id, cb_button_t cb, void* userdata);

#endif // !_SYS_BUTTON_H
