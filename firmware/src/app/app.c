#include <sys/tick.h>
#include <sys/display.h>
#include <sys/scheduler.h>
#include <sys/button.h>
#include <sys/event.h>
#include <sys/panic.h>

#include <hardware/ch9329.h>
#include <hardware/w25qx.h>
#include <hardware/ch1115.h>

#include <ui/stack.h>
#include <app/resource.h>
#include <layout/main/select.h>

#define PROGRESS(x) scheduler_sleep(100);
stack_ctx_t *stack;

void cb_button_event(button_id_t id, button_status_t type, void* p) {

  const static event_t tab[] = {
    event_button_up,
    event_button_down,
    event_button_hold,
    event_button_hold_click,
    event_button_release,
  };

  stack_call_event(stack, tab[type], (param_t)stack->final->surface, id);
}

void cb_task_present(task_t* task) {
  if(!stack->final) return;
  stack_call_event(stack, event_surface_present, (param_t)stack->final->surface, NULL);
}

void appmain() {

  // initialize resources
  if(!resource_init(RUCFSIMG)) {
    panic(NULL);
    return;
  }

  // initialize display
  display_init(false); {
    display_clear(clr_black);
    display_bitblt(0, 0, 0, 0, 128, 32, RES_TEXTURE_COSMOS_SPLASH);
    display_light(true);

    display_usefont(FONT_PIXEL_3X5, 3, 8, 8, 282, 1, RES_FONT_PIXEL3X5);
    display_usefont(FONT_PIXEL_5X7, 5, 8, 8, 475, 1, RES_FONT_PIXEL5X7);
    display_default_font(FONT_PIXEL_5X7);
  }

  // initialize peripherals
  PROGRESS(10); {
    w25qx_init();
    ch9329_init();
  }

  PROGRESS(100); {
    scheduler_sleep(1000);

    // create initial stack
    stack = stack_create(); {

      // set present timer at 30Hz
      scheduler_add_task(33, cb_task_present, NULL);

      // check button status every 50ms
      scheduler_add_task(50, cb_task_check_button, NULL); {
        button_set_eventcb(btnid_a, cb_button_event, NULL);
        button_set_eventcb(btnid_b, cb_button_event, NULL);
      }

      // goto main surface
      stack_push_surface(stack, &surface_select);
    }
  }
}
