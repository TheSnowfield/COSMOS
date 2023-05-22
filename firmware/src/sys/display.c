#include <string.h>
#include <stdint.h>

#include <sys/tick.h>
#include <sys/scheduler.h>

#include <config.h>
#include <hardware/ch1115.h>

#include "display.h"

#define PIXEL2BYTES(p) (p / 8)
#define PAGESTART(p) (p * DISPLAY_COLUMNS)

static display_t _display = { 0 };
static page_buffer_t _buffer[DISPLAY_PAGES];

// void __memset_pixel(pixel_t* dst, pixel_t val, uint32_t sz) {
//   for(pixel_t* i = dst, *j = i + sz; i < j; ++i) *i = val;
// }

void __display_ready() {

  // if transmit is disabled, return
  if(!_display.transmit_enable) return;

  // if all pages are transmitted, reset the page
  if(_display.transmit_page >= DISPLAY_PAGES) {

    _display.vsync = true;
    _display.transmit_page = 0;

    // // get hardware fps
    uint32_t tick = tick_get_time();
    if(_display.vsync_tick > tick) _display.vsync_tick = 0;
    if(tick - _display.vsync_tick >= 1000) {
      _display.vsync_rate = _display.vsync_counter;
      _display.vsync_counter = 0;
      _display.vsync_tick = tick;
    }
  }

  else _display.vsync = false;

  // transmit the next page
  ch1115_write_page_it(_display.transmit_page, (uint8_t *)(&_display.buffer[_display.transmit_page]), sizeof(page_buffer_t));
  ++_display.transmit_page;
}

void display_init() {
  _display.init = true;
  _display.buffer = _buffer;
  _display.default_font = 0;
  _display.vsync = true;
  _display.transmit_page = 0;
  _display.transmit_enable = true;

  // init page buffers
  memset(_buffer, 0x00, sizeof(_buffer));
  for (size_t i = 0; i < DISPLAY_PAGES; ++i) {
    _buffer[i].driver_cmd = 0x40;
  }

  ch1115_init(); {

    // clear with black
    display_clear(clr_black);

    // set column and page
    ch1115_set_page(0);
    ch1115_set_column(0);
    
    // start display content transmission
    ch1115_set_readycb(__display_ready);
    display_transmit(true);
  }
}

bool display_vsync() {
  return _display.vsync;
}

void display_transmit(bool enable) {
  _display.transmit_enable = enable;
  if(enable) __display_ready();
}

status_t display_light(bool en, bool breathing) {

  if(en && !breathing) {
    ch1115_light_on(false);
  }

  else if(!en && !breathing) {
    ch1115_light_off(false);
  }

  else if(en && breathing) {
    display_transmit(false); {
      scheduler_sleep(10);
      display_update();
      ch1115_light_on(true);
      display_transmit(true);
    }
  }

  else if(!en && breathing) {
    display_transmit(false); {
      scheduler_sleep(10);
      display_update();
      ch1115_light_off(true);
      display_transmit(true);
    }
  }

  return ok;
}

void display_update() {
  for(uint8_t i = 0; i < DISPLAY_PAGES; ++i)
    ch1115_write_page(i, (uint8_t *)(&_display.buffer[i]), sizeof(page_buffer_t));
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

// void display_update() {
//   ch1115_write_page(_display.transmit_page, 0, _display.buffer + PAGESTART(_display.transmit_page), DISPLAY_COLUMNS);
// }

// void display_invalidate() {
//   _display.vsync = false;
// }

status_t display_clear(ch1115_color_t color) {
  for (size_t i = 0; i < DISPLAY_PAGES; ++i) {
    memset(_display.buffer[i].columns, color, DISPLAY_COLUMNS);
  }
  return ok;
}

bool display_usefont(font_id_t slot, uint8_t fontw, uint8_t fonth,
                     uint16_t imgw, uint16_t imgh, uint8_t space, const uint8_t *img) {
  if(!img) return false;

  _display.fonts[slot].symbol.x = 0;
  _display.fonts[slot].symbol.y = 0;
  _display.fonts[slot].symbol.width = fontw;
  _display.fonts[slot].symbol.height = fonth;
  _display.fonts[slot].texture.x = 0;
  _display.fonts[slot].texture.y = 0;
  _display.fonts[slot].texture.width = imgw;
  _display.fonts[slot].texture.height = imgh;
  _display.fonts[slot].image = (uint8_t *)img;
  _display.fonts[slot].space = space;

  return true;
}

bool display_default_font(font_id_t slot) {
  if(slot > MAX_FONT_SLOTS) return false;
  _display.default_font = slot;
  return true;
}

uint8_t display_get_default_font() {
  return _display.default_font;
}

void display_draw_string(uint8_t x, uint8_t y, const char* str) {
  return display_draw_string_ex(x, y, _display.default_font, str);
}

// void display_draw_char_ex(font_id_t slot, const char ch) {

//   size_t index = ch - ' ';
//   size_t y = index * _display.fonts[slot].symbol.width;

//   for(uint8_t column = 0; column < _display.fonts[slot].symbol.width; ++column)
//     ch1115_set_pixel_column(_display.fonts[slot].image[y + column]);
// }

void display_draw_string_ex(uint8_t x, uint8_t y, font_id_t slot, const char* str) {

  uint8_t top = y;
  uint8_t left = x;
  for(; *str != 0; ++str) {

    // check if we need to move to the next line
    if(*str == '\n' || top >= (DISPLAY_H - _display.fonts[slot].symbol.height)) {
      left -= _display.fonts[slot].symbol.width;
      top = y;
      continue;
    }

    // draw character
    display_bitblt(left, top,
      _display.fonts[slot].symbol.width,
      _display.fonts[slot].symbol.height,
      _display.fonts[slot].texture.height,
      _display.fonts[slot].image + (*str - ' ') * _display.fonts[slot].symbol.height
    );

    top += _display.fonts[slot].symbol.width;
    top += _display.fonts[slot].space;

    // // apply font spaces
    // for(size_t i = 0; i < _display.fonts[slot].space; ++i)
    //   ch1115_set_pixel_column(0x00);
  }
}

// void display_draw_char(const char ch) {
//   return display_draw_char_ex(_display.default_font, ch);
// }

#define X2PAGE(x) ((x) / DISPLAY_BITS_PER_COLUMN)
#define MKMASK(width, var) {for(size_t i = 0; i < width; ++i) var = (var | 0b00000001) << 1; }

void display_bitblt(uint8_t dstx, uint8_t dsty, size_t srcw,
  size_t srch, size_t stride, const uint8_t* data) {
  


  // the height of source image must multiply with 8
  // if(srch % DISPLAY_BITS_PER_COLUMN != 0) return;

  // if destination address is aligned to the page
  if(dstx % DISPLAY_BITS_PER_COLUMN == 0) {

    // memcpy(_display.buffer[X2PAGE(dstx)].columns + dsty, data, srch);
    // memcpy(_display.buffer[X2PAGE(dstx) + 1].columns + dsty, data + srch, srch);

    // copy the data to the display buffer
    for(size_t page = 0; page < X2PAGE(srcw); ++page) {
      memcpy(_display.buffer[X2PAGE(dstx) + page].columns + dsty, data + page * stride, srch);
    }
  }

  // we have to bitwise the pixel in every column,
  // because the destination address is not aligned.
  else {

    for(size_t y = 0; y < srch; ++y) {

      uint32_t srccol = 0, dstcol = 0;
      column_t* dstbuf = (column_t *)&dstcol;
      uint32_t mask = 0; { MKMASK(srcw, mask); mask <<= dstx; }
      
      // read the display
      // dstcol = (dstcol | _display.buffer[0].columns[dsty + y]) << 8;
      // dstcol = (dstcol | _display.buffer[1].columns[dsty + y]) << 8;
      // dstcol = (dstcol | _display.buffer[2].columns[dsty + y]) << 8;
      // dstcol = (dstcol | _display.buffer[3].columns[dsty + y]);
      for (size_t i = 0; i < DISPLAY_PAGES; ++i)
        dstcol = (dstcol | _display.buffer[i].columns[dsty + y]) << (i == DISPLAY_PAGES - 1 ? 0 : 8);

      // read the source image
      // srccol = (data[1 * stride + y]);
      // srccol = (srccol << 8) | (data[0 * stride + y]);
      for (size_t i = 0; i <= X2PAGE(srcw) - 1; ++i)
        srccol = (srccol << (i == 0 ? 0 : 8)) | (data[(X2PAGE(srcw) - 1 - i) * stride + y]);
      srccol <<= dstx;

      // blend the pixels up
      dstcol ^= dstcol & mask;
      dstcol |= srccol;

      // write pixels back
      for (size_t i = 0; i < DISPLAY_PAGES; ++i)
        _display.buffer[i].columns[dsty + y] = dstbuf[i];
    }
  }
}
