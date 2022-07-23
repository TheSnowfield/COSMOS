#define _BUTTON_INTERNAL
#include <sys/button.h>
#undef _BUTTON_INTERNAL

#include <config.h>

static task_t *button_task;
static uint16_t button_changed;
static button_list_t button_list[3];
void cb_task_check_button(task_t* task);
void cb_task_check_hold(task_t* task);

button_id_t __map_button_id(uint16_t pin);

void button_init() {

  // Init button ports
  HAL_GPIO_Init(BUTTON_PORT, &port_buttons); {
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
  }

  // Create task
  button_task = scheduler_add_task(50, cb_task_check_button, NULL);
}

void button_set_handler(button_id_t id, cb_button_t cb, void* userdata) {
  button_list[id].cb = cb;
  button_list[id].userdata = userdata;
  button_list[id].chkhold = NULL;
}

void cb_task_check_button(task_t* task) {
  
  // No button changed
  if(button_changed == 0) return;

  uint16_t io = button_changed;
  button_changed = 0;
  
  // Grab button id and state
  button_id_t id = __map_button_id(io);
  GPIO_PinState state = HAL_GPIO_ReadPin(BUTTON_PORT, io);

  // Low level is pressed
  if(!state) {

    // Update status
    button_list[id].status = button_down;

    // Create a task to check hold operation
    button_list[id].chkhold = scheduler_add_task(3000, cb_task_check_hold, io);

    // Call button callback
    if(button_list[id].cb)
      button_list[id].cb(button_down, button_list[id].userdata);
  }

  // High level is released
  else {

    // Update status
    button_list[id].status = button_up;

    // If task exists, remove it
    if(button_list[id].chkhold) {
      scheduler_remove_task(button_list[id].chkhold);
      button_list[id].chkhold = NULL;
    }

    // Call button callback
    if(button_list[id].cb)
      button_list[id].cb(button_up, button_list[id].userdata);
  }
}

void cb_task_check_hold(task_t* task) {
  uint16_t io = (uint16_t)task->userdat;
  GPIO_PinState state = HAL_GPIO_ReadPin(BUTTON_PORT, io); {
    
    // The button is not pressed
    // This case is not possible cuz the task is removed at button release.
    if(state) return;

    button_id_t id = __map_button_id(io); {

      // Update status
      button_list[id].status = button_up;

      // Remove this task
      scheduler_remove_task(task);
      button_list[id].chkhold = NULL;

      // Call button callback
      if(button_list[id].cb)
        button_list[id].cb(button_hold, button_list[id].userdata);
    }
  }
}

button_id_t __map_button_id(uint16_t pin) {
  switch(pin) {
    case BUTTON_PIN_KEY_A:
      return btnid_a;
    case BUTTON_PIN_KEY_B:
      return btnid_b;
    // case BUTTON_PIN_KEY_SHUTTER:
    //   return btnid_shutter;
    default:
      return btnid_none;
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  button_changed = GPIO_Pin;
}
