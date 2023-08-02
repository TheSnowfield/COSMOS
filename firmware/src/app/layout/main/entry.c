#include <sys/button.h>
#include <sys/display.h>
#include <sys/scheduler.h>
#include <hardware/ch9329.h>

#include <ui/stack.h>
#include <ui/widgets/menu.h>

#include <app/resource.h>
#include <app/i18n/language.h>
#include <layout/main/keys.h>
#include <layout/main/settings.h>
#include <layout/main/about.h>


#define MENU_ITEM_KEYS     0
#define MENU_ITEM_SETTINGS 1
#define MENU_ITEM_ABOUT    2
static menu_item_t _menu_items[] = {
  { MENU_ITEM_KEYS,     &RES_ICON_FOLDER, &STRING_ENTRY_MENU_KEYS     },
  { MENU_ITEM_SETTINGS, &RES_ICON_FOLDER, &STRING_ENTRY_MENU_SETTINGS },
  { MENU_ITEM_ABOUT,    &RES_ICON_FOLDER, &STRING_ENTRY_MENU_ABOUT    },
};

void menu_clicked(size_t id, stack_ctx_t *ctx) {
  switch(id) {
    case MENU_ITEM_KEYS:
      stack_push_window(ctx, &window_keys);
      break;
    case MENU_ITEM_SETTINGS:
      stack_push_window(ctx, &window_settings);
      break;
    case MENU_ITEM_ABOUT:
      stack_push_window(ctx, &window_about);
      break;
  }
}

uint32_t entry_wndproc(stack_ctx_t *ctx, event_t event, param_t lp, param_t wp) {

  switch(event) {
    case event_window_create: {

      // initialize menu
      menu_init(0, 12, DISPLAY_W, 116); {
        menu_set_onclick(menu_clicked);
        menu_set_items(_menu_items, sizeof(_menu_items) / sizeof(menu_item_t));
        menu_select(MENU_ITEM_SETTINGS);
        menu_draw();
      }

      break;
    }

    case event_window_destroy:
      menu_uninit();
      break;

    case event_button_hold:

      break;

    case event_button_down:

      break;

    default: break;
  }

  return 0;
}
