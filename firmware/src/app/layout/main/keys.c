#include <sys/button.h>
#include <sys/display.h>
#include <sys/scheduler.h>
#include <hardware/ch9329.h>

#include <app/resource.h>
#include <ui/stack.h>

uint32_t keys_wndproc(stack_ctx_t *ctx, event_t event, param_t lp, param_t wp) {

  switch(event) {
    case event_window_create: {

      display_draw_string(13, 52, "Keys");

      break;
    }

    case event_window_present: {

      break;
    }

    case event_button_hold:
      if(wp == btnid_b) stack_back(ctx);
      break;
      
    default: break;
  }

  return 0;
}