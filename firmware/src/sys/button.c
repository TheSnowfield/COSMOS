#define _BUTTON_INTERNAL
#include <sys/button.h>
#undef _BUTTON_INTERNAL

#include <string.h>
#include <hardware/config.h>

bool button_changed;
button_list_t button_list[2] = {0};
cb_button_t button_cb;

button_id_t __map_button_id(uint16_t pin) {
  switch(pin) {
    case BUTTON_PIN_KEY_A:
      return btnid_a;
    case BUTTON_PIN_KEY_B:
      return btnid_b;
    default:
      return btnid_none;
  }
}

uint16_t __map_button_io(button_id_t id) {
  switch(id) {
    case btnid_a:
      return BUTTON_PIN_KEY_A;
    case btnid_b:
      return BUTTON_PIN_KEY_B;
    default:
      return 0;
  }
}

void button_set_eventcb(button_id_t id, cb_button_t cb, void* userdata) {
  button_list[id].cb = cb;
  button_list[id].userdata = userdata;
}

button_status_t button_get_status(button_id_t id, bool immediate) {
  return !immediate
    ? button_list[id].status
    : (button_status_t)HAL_GPIO_ReadPin(BUTTON_PORT, __map_button_io(id));
}

void cb_task_check_hold(task_t* task) {
  uint16_t io = (uint16_t)task->userdat;
  GPIO_PinState state = HAL_GPIO_ReadPin(BUTTON_PORT, io); {
    
    button_id_t id = __map_button_id(io); {

      // the button is not pressed but task runs to here
      // force clear button state
      if(state) {
        if(button_list[id].chkhold) {
          scheduler_remove_task(button_list[id].chkhold);
          button_list[id].chkhold = NULL;
          button_list[id].holdtick = 0;
          button_list[id].status = button_up;
        }
        return;
      }

      if(button_list[id].holdtick == 5) {

        // update button status
        button_list[id].status = button_hold;

        // call button hold
        if(button_list[id].cb)
          button_list[id].cb(id, button_hold, button_list[id].userdata);
      }

      // user hold the button for 100ms * 10
      else if(button_list[id].holdtick > 10) {
        // call button hold click
        if(button_list[id].cb)
          button_list[id].cb(id, button_hold_click, button_list[id].userdata);
      }

      // increase hold count
      button_list[id].holdtick++;
    }
  }
}

void cb_task_check_button(task_t* task) {
  
  // no button changed
  if(button_changed == 0) return;
  button_changed = 0;

  // scan all buttons
  for(size_t i = 0; i < btnid_b; ++i) {

    // this button is not changed
    if(button_list[i].intr_changed == false) continue;
    button_list[i].intr_changed = false;

    // grab button id and state
    button_id_t id = (button_id_t)i;
    uint16_t io = __map_button_io(id);
    GPIO_PinState state = HAL_GPIO_ReadPin(BUTTON_PORT, io);

    // low level is pressed
    if(!state) {

      // update status
      button_list[id].status = button_down;

      // create a task to check hold operation
      button_list[id].holdtick = 0;
      if(button_list[id].chkhold == NULL)
        button_list[id].chkhold = scheduler_add_task(100, cb_task_check_hold, io);

      // call button callback
      if(button_list[id].cb)
        button_list[id].cb(id, button_down, button_list[id].userdata);
    }

    // high level is released
    else {

      button_status_t notifystat = button_list[id].status ==
                                  button_down ? button_up : button_release;

      // Update status
      button_list[id].status = button_up;

      // If task exists, remove it
      if(button_list[id].chkhold) {
        scheduler_remove_task(button_list[id].chkhold);
        button_list[id].chkhold = NULL;
        button_list[id].holdtick = 0;
      }

      // call button callback
      if(button_list[id].cb)
        button_list[id].cb(id, notifystat, button_list[id].userdata);
    }
  }
}

