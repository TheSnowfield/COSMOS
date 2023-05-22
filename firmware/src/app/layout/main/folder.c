#include <sys/display.h>
#include <ui/stack.h>
#include <app/resource.h>

uint32_t folder_wndproc(stack_ctx_t *ctx, event_t event, param_t lp, param_t wp) {

  switch(event) {
    case event_surface_create:


      display_draw_string(0, 0, "Hello world!");

      // display_move_caret(0, 0); {
      //   display_draw_string_ex(FONT_PIXEL_5X7, "The quick brown fox");
      //   display_move_caret(0, 1);
      //   display_draw_string_ex(FONT_PIXEL_5X7, "jumps over the");
      //   display_move_caret(0, 2);
      //   display_draw_string_ex(FONT_PIXEL_5X7, "lazy dog.");
      //   display_move_caret(0, 3);
      //   display_draw_string_ex(FONT_PIXEL_5X7, "         \\( > w <) p");
      // }

      break;

    case event_surface_present: {

      // display_clear(clr_black);

      
      break;
    }
  }

  return 0;
}
