#include <string.h>
#include <config.h>

#include <sys/tick.h>
#include <hardware/ch1115.h>

#include "display.h"

static display_t display;
static font_t _font[MAX_FONT_SLOTS] = {0};
static font_id_t _font_default;

// void __memset_pixel(pixel_t* dst, pixel_t val, uint32_t sz) {
//   for(pixel_t* i = dst, *j = i + sz; i < j; ++i) *i = val;
// }

void display_init() {
  ch1115_init();
  display.init = true;
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

bool display_usefont(font_id_t slot, uint8_t fontw, uint8_t fonth,
                     uint16_t imgw, uint16_t imgh, uint8_t space, const uint8_t *img) {
  if(!img) return false;

  _font[slot].symbol.x = 0;
  _font[slot].symbol.y = 0;
  _font[slot].symbol.width = fontw;
  _font[slot].symbol.height = fonth;
  _font[slot].texture.x = 0;
  _font[slot].texture.y = 0;
  _font[slot].texture.width = imgw;
  _font[slot].texture.height = imgh;
  _font[slot].image = (uint8_t *)img;
  _font[slot].space = space;

  return true;
}

bool display_default_font(font_id_t slot) {
  if(slot > MAX_FONT_SLOTS) return false;
  _font_default = slot;
  return true;
}

uint8_t display_get_default_font() {
  return _font_default;
}

void display_draw_string(const char* str) {
  return display_draw_string_ex(_font_default, str);
}

void display_draw_char_ex(font_id_t slot, const char ch) {

  size_t index = ch - ' ';
  size_t y = index * _font[slot].symbol.width;

  for(uint8_t column = 0; column < _font[slot].symbol.width; ++column)
    ch1115_set_pixel_column(_font[slot].image[y + column]);
}

void display_draw_string_ex(font_id_t slot, const char* str) {

  // uint8_t top = y;
  // uint8_t left = x;
  for(; *str != 0; ++str) {

    // check if we need to move to the next line
    // if(*str == '\n' || left >= (DISPLAY_W - _font.symbol.width)) {
    //   top += _font.symbol.height;
    //   left = x;
    //   continue;
    // }

    // draw character
    display_draw_char_ex(slot, *str);

    // apply font spaces
    for(size_t i = 0; i < _font[slot].space; ++i)
      ch1115_set_pixel_column(0x00);
  }
}

void display_draw_char(const char ch) {
  return display_draw_char_ex(_font_default, ch);
}

void display_bitblt(uint8_t dstx, uint8_t dsty,
                    uint8_t srcx, uint8_t srcy, uint8_t srcw, uint8_t srch,
                    const uint8_t* data) {
  display_bitblt_area(dstx, dsty, srcx, srcy, srcw, srch, srcw, data);
}

void display_bitblt_area(uint8_t dstx, uint8_t dsty,
                    uint16_t srcx, uint16_t srcy, uint16_t srcw, uint16_t srch,
                    uint16_t width, const uint8_t* data) {


  for(uint8_t page = 0; page < CH1115_HEIGHT_PAGE; ++page) {
    ch1115_set_page(page);
    ch1115_set_column(0);
    for(uint8_t column = 0; column < CH1115_WIDTH; ++column)
      ch1115_set_pixel_column(data[page * CH1115_WIDTH + column]);
  }
}

void display_move_caret(uint8_t x, uint8_t y) {
  y = 3 - y;
  display.caretx = x;
  display.carety = y;
  ch1115_set_page(y);
  ch1115_set_column(x);
}
