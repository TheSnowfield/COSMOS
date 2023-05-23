#include <sys/button.h>
#include <sys/display.h>
#include <sys/scheduler.h>
#include <hardware/ch9329.h>

#include <app/resource.h>
#include <ui/stack.h>
#include <layout/debug/pingpong.h>

uint32_t keys_wndproc(stack_ctx_t *ctx, event_t event, param_t lp, param_t wp) {

  switch(event) {
    case event_surface_create: {

      display_draw_string(13, 52, "Keys");

      break;
    }

    case event_surface_present: {

      break;
    }

    case event_button_up: {

      if(wp == btnid_a) {
        stack_push_surface(ctx, &surface_pingpong);
      }
      // display_clear(clr_black);

      break;
    }
  }

  return 0;
}
