#ifndef _SYS_DISPLAY_H
#define _SYS_DISPLAY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/status.h>
#include <sys/compiler/packed.h>

#include <hardware/ch1115.h>

#define DISPLAY_H 128
#define DISPLAY_W 32

#define DISPLAY_BITS_PER_COLUMN 8
#define DISPLAY_PAGES (DISPLAY_W / DISPLAY_BITS_PER_COLUMN)
#define DISPLAY_COLUMNS (DISPLAY_H)

// Example of displaying two 'A' characters
// on two different pages

//         0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 (pixel)
//         0               1               (page)
//
//         |
//  0   ---+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+---->  x (pixel and page)
//         |█|█|█|█|█| | | |█|█|█|█|█| | | |
//  1      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//         | | |█| | |█| | | | |█| | |█| | |
//  2      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//         | | |█| | | |█| | | |█| | | |█| |
//  3      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//         | | |█| | | |█| | | |█| | | |█| |
//  4      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//         | | |█| | |█| | | | |█| | |█| | |
//  5      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//         |█|█|█|█|█| | | |█|█|█|█|█| | | |
//  6      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// (col)   |
//         |
//         v
//
//     y (column)

typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
} rect_t;

typedef struct {
  rect_t symbol;
  rect_t texture;
  uint8_t* image;
  uint8_t space;
} font_t;

typedef enum {
  FONT_PIXEL_3X5 = 0,
  FONT_PIXEL_5X7 = 1,
  MAX_FONT_SLOTS
} font_id_t;

typedef uint8_t column_t;

typedef struct {
  uint8_t  driver_cmd;
  column_t columns[DISPLAY_COLUMNS];
} packed page_buffer_t;

typedef struct {
  bool init;
  page_buffer_t *buffer;
  font_t fonts[MAX_FONT_SLOTS];
  font_id_t default_font;
  uint8_t transmit_page;
  bool transmit_enable;
  bool vsync;
  uint32_t vsync_tick;
  uint16_t vsync_counter;
  uint16_t vsync_rate;
} display_t;

/**
 * @brief init display
 */
void display_init();

/**
 * @brief display initialized
 */
bool display_inited();

bool display_vsync();
void display_update();

void display_transmit(bool enable);

/**
 * @brief turn on display back light
 * @param en enabled
 */
status_t display_light(bool en, bool breathing);

/**
 * @brief copy origin image to display
 * @param dstx destination x
 * @param dsty destination y
 * @param srcw image width
 * @param srch image height
 * @param data image buffer
 */

void display_bitblt(uint8_t dstx, uint8_t dsty, size_t srcw,
  size_t srch, size_t stride, const uint8_t* data);

// /**
//  * @brief fill the block with a color
//  * @param x dest x
//  * @param y dest y
//  * @param width block width
//  * @param height block height
//  * @param color color
//  */
// void display_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint32_t color);

/**
 * @brief set pixel color
 * @param x pixel x
 * @param y pixel y
 * @param color pixel color
 */
void display_set_pixel(uint8_t x, uint8_t y, uint32_t color);

/**
 * @brief reverse color
 * @param x pixel x
 * @param y pixel y
 * @param width block width
 * @param height block height
 */
void display_reverse_color(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

void display_draw_string(uint8_t x, uint8_t y, const char* str);

/**
 * @brief draw a string
 * @param x destination x
 * @param y destination y
 * @param str string
 */
void display_draw_string_ex(uint8_t x, uint8_t y, font_id_t slot, const char* str);

/**
 * @brief use a font
 * @param slot font slot, condiser FONT_xxx macros
 * @param fontw font symbol width
 * @param fonth font symbol height
 * @param imgw font atlas width
 * @param imgh font atlas height
 * @param space character space
 * @param img font atlas image
 */
bool display_usefont(font_id_t slot, uint8_t fontw, uint8_t fonth, uint16_t imgw, uint16_t imgh, uint8_t space, const uint8_t *img);

/**
 * @brief set default font
 * @param slot font slot
 * @return if success return true
 */
bool display_default_font(font_id_t slot);

/**
 * @brief clear with a color
 * @param color color
 */
status_t display_clear(ch1115_color_t color);

/**
 * @brief get display hardware refresh rate in Hz
 */
uint16_t display_refresh_rate();

// void display_draw_char(const char ch);

#endif // !_SYS_DISPLAY_H
