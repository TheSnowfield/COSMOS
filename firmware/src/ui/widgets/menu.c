#define _WIDGET_MENU_INTERNAL
#include "menu.h"
#undef _WIDGET_MENU_INTERNAL

#include <sys/display.h>

static menu_widget_t _widget;

void menu_init(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  _widget.base.x = x;
  _widget.base.y = y;
  _widget.base.width = width;
  _widget.base.height = height;
  _widget.base.visiable = true;
  _widget.base.inited = true;
  _widget.base.callback.click = NULL;
  _widget.base.callback.select = NULL;
}

void menu_uninit() {
  _widget.base.x = 0;
  _widget.base.y = 0;
  _widget.base.width = 0;
  _widget.base.height = 0;
  _widget.base.visiable = false;
  _widget.base.inited = false;
  _widget.items = NULL;
  _widget.items_count = 0;
  _widget.items_selected = 0;
}

void menu_set_items(const menu_item_t* items, uint8_t count) {
  if(items == NULL) return;
  _widget.items = items;
  _widget.items_count = count;
}

void menu_select(uint8_t index) {
  if(index < 0 || index >= _widget.items_count) return;
  _widget.items_selected = index;
}

void menu_draw() {

  // draw items
  for(size_t i = 0; i < _widget.items_count; ++i) {
    display_draw_string(_widget.base.x + _widget.base.width - (i+1) * 8, _widget.base.y, *_widget.items[i].i18nstr);
  }

  // display_reverse_color(0, 0, 18, 128);

}

bool menu_set_onclick(void* callback) {
  if(callback == NULL) return false;
  _widget.callback.click = callback;
  return true;
}

// uint32_t menu_widget_wndproc(stack_ctx_t *ctx, event_t event, param_t lp, param_t wp) {


// }
