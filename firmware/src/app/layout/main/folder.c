#include <sys/display.h>
#include <sys/scheduler.h>
#include <hardware/ch9329.h>

#include <ui/stack.h>
#include <app/resource.h>

#include <stdio.h>

uint32_t folder_wndproc(stack_ctx_t *ctx, event_t event, param_t lp, param_t wp) {

  switch(event) {
    case event_surface_create: {

      break;
    }

    case event_surface_present: {

      // display_clear(clr_black);

      break;
    }
  }

  return 0;
}
