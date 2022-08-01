#include <string.h>
#include <config.h>

#include <sys/tick.h>
#include <hardware/ch1115.h>

#include "display.h"

static display_t display;

// void __memset_pixel(pixel_t* dst, pixel_t val, uint32_t sz) {
//   for(pixel_t* i = dst, *j = i + sz; i < j; ++i) *i = val;
// }

void display_init(bool vsync) {

  ch1115_init(); {
    display_clear(clr_white);
    // display_set_readycb(__display_ready);
    // ch1115_init_dma(!vsync); {
    //   ch1115_write_bytes_dma((uint8_t *)gbuffer, sizeof(gbuffer));
    // }
  }

  display.init = true;
  display.vsync = false;
  display.vsync_en = vsync;
}

bool display_inited() {
  return display.init;
}

status_t display_light(bool en) {
  return en ? ch1115_light_on() : ch1115_light_off();
}

// void display_fill_rect(uint8_t x, uint8_t y, uint8_t width,
//                       uint8_t height, uint32_t color) {

//   for(int i = y; i < height + y; ++i)
//   for(int j = x; j < x + width;  ++j)
//     gbuffer[i * DISPLAY_H + j].clr = color;
// }

// void display_set_pixel(uint8_t x, uint8_t y, uint32_t color) {
//   gbuffer[y * DISPLAY_H + x] = color;
// }

// void display_reverse_color(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
//   for(int i = y; i < height + y; ++i)
//   for(int j = x; j < x + width;  ++j)
//     gbuffer[i * DISPLAY_H + j] = ~gbuffer[i * DISPLAY_H + j].clr;
// }

status_t display_clear(ch1115_color_t color) {
  return ch1115_clear(color);
}

static font_t _font = {0};
static bool _usefont = false;
bool display_usefont(uint8_t fontw, uint8_t fonth,
uint16_t imgw, uint16_t imgh, const uint8_t *img) {
  if(!img) return false;

  _usefont = true;
  _font.symbol.x = 0;
  _font.symbol.y = 0;
  _font.symbol.width = fontw;
  _font.symbol.height = fonth;
  _font.texture.x = 0;
  _font.texture.y = 0;
  _font.texture.width = imgw;
  _font.texture.height = imgh;
  _font.image = (uint8_t *)img;
  _font.space = 0;

  return true;
}

// void display_draw_string(uint8_t x, uint8_t y, const char* str) {
//   if(!_usefont) return;

//   uint8_t top = y;
//   uint8_t left = x;
//   for(; *str != 0; ++str) {

//     // check if we need to move to the next line
//     if(*str == '\n' || left >= (DISPLAY_W - _font.symbol.width)) {
//       top += _font.symbol.height;
//       left = x;
//       continue;
//     }

//     // draw character
//     display_bitblt_area(left, top, (*str - ' ') * _font.symbol.width, 0,
//                         _font.symbol.width, _font.symbol.height,
//                         _font.texture.width, _font.image);

//     left += _font.symbol.width + _font.space;
//   }
// }

// void display_scroll_left(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
//   for(int i = y; i < y + h; ++i) {
//     memcpy(&gbuffer[i * DISPLAY_W + x], &gbuffer[i * DISPLAY_W + (x + 1)], (w - 1) * sizeof(pixel_t));
//   }
// }

// void display_bitblt(uint8_t dstx, uint8_t dsty,
//                     uint8_t srcx, uint8_t srcy, uint8_t srcw, uint8_t srch,
//                     const uint8_t* data) {

//   display_bitblt_area(dstx, dsty, srcx, srcy, srcw, srch, srcw, data);
// }

// void display_bitblt_area(uint8_t dstx, uint8_t dsty,
//                     uint16_t srcx, uint16_t srcy, uint16_t srcw, uint16_t srch,
//                     uint16_t width, const uint8_t* data) {

//   pixel_t* pdata = (pixel_t *)data;
//   for (int i = dsty, j = srcy; j < srch; ++i, ++j)
//     memcpy(&gbuffer[DISPLAY_W * i + dstx], &pdata[width * j + srcx], srcw * sizeof(pixel_t));
// }
