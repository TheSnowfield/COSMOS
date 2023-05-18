#ifndef _SYS_DISPLAY_H
#define _SYS_DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/status.h>
#include <hardware/ch1115.h>

typedef uint8_t pixel_t;

typedef struct rect {
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
} rect_t;

typedef struct font {
  rect_t symbol;
  rect_t texture;
  uint8_t* image;
  uint8_t space;
} font_t;

typedef struct {
  bool init;
  uint8_t caretx;
  uint8_t carety;
} display_t;

typedef enum {
  FONT_PIXEL_3X5 = 0,
  FONT_PIXEL_5X7 = 1,
  MAX_FONT_SLOTS
} font_id_t;

/**
 * @brief init display
 */
void display_init();

/**
 * @brief display initialized
 */
bool display_inited();

/**
 * @brief turn on display back light
 * @param en enabled
 */
status_t display_light(bool en);

/**
 * @brief bitblt in an area
 * @param dstx dest x
 * @param dsty dest y
 * @param srcx src x
 * @param srcy src y
 * @param srcw src width
 * @param srch src height
 * @param width src image width
 * @param data src image
 */
void display_bitblt_area(uint8_t dstx, uint8_t dsty,
                         uint16_t srcx, uint16_t srcy, uint16_t srcw, uint16_t srch,
                         uint16_t width, const uint8_t* data);

/**
 * @brief copy origin image to display
 * @param dstx destination x
 * @param dsty destination y
 * @param srcx source x
 * @param srcy source y
 * @param srcw image width
 * @param srch image height
 * @param data image buffer
 */
void display_bitblt(uint8_t dstx, uint8_t dsty,
                    uint8_t srcx, uint8_t srcy, uint8_t srcw, uint8_t srch,
                    const uint8_t* data);

/**
 * @brief fill the block with a color
 * @param x dest x
 * @param y dest y
 * @param width block width
 * @param height block height
 * @param color color
 */
void display_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint32_t color);

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

void display_draw_string(const char* str);

/**
 * @brief draw a string
 * @param x destination x
 * @param y destination y
 * @param str string
 */
void display_draw_string_ex(font_id_t slot, const char* str);

void display_draw_char(const char ch);
void display_draw_char_ex(font_id_t slot, const char ch);


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

void display_draw_char(const char ch);

void display_move_caret(uint8_t x, uint8_t y);

#endif // !_SYS_DISPLAY_H
