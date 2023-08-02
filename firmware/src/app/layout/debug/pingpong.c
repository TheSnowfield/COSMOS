#include <sys/button.h>
#include <sys/display.h>
#include <sys/scheduler.h>
#include <hardware/ch9329.h>

#include <ui/stack.h>
#include <app/resource.h>
#include <layout/main/keys.h>

uint32_t pingpong_wndproc(stack_ctx_t *ctx, event_t event, param_t lp, param_t wp) {

  switch(event) {

    case event_window_create: {
      break;
    }

    case event_window_present: {

      // display_clear(clr_black);

      static size_t x = 0, dx = -1;
      static size_t y = 0, dy = -1;
      static bool pp = false;

      if(x == 0) { dx = 1; pp = !pp; }
      if(x > 24) { dx = -1; pp = !pp; }

      if(y == 0) { dy = 1; pp = !pp; }
      if(y > 70) { dy = -1; pp = !pp; }

      display_draw_string(x, y, "DVD");

      x += dx;
      y += dy;

      break;
    }

  case event_button_up:
      if(wp == btnid_a) stack_back(ctx);
  }

  return 0;
}
