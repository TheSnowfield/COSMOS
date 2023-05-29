#ifndef _UI_WIDGETS_BASE_H
#define _UI_WIDGETS_BASE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
  void (* click)(size_t id, void* usrdat);
  void (* select)(size_t id, void* usrdat);
} widget_callback_t;

typedef struct {
  bool inited;
  uint8_t x;
  uint8_t y;
  uint8_t width;
  uint8_t height;
  bool visiable;
  widget_callback_t callback;
} widget_t;

#endif // !_UI_WIDGETS_BASE_H
