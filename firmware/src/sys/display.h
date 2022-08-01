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

  bool vsync;
  bool vsync_en;
  uint32_t vsync_tick;
  uint16_t vsync_counter;
  uint16_t vsync_rate;

} display_t;

/**
 * @brief Init display
 * @param vsync Enable vsync
 */
void display_init(bool vsync);

/**
 * @brief Display initialized
 */
bool display_inited();

bool display_vsync();
void display_update();

/**
 * @brief Turn on display back light
 * @param en enabled
 */
status_t display_light(bool en);

/**
 * @brief Bitblt in an area
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
 * @brief Copy origin image to display
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
 * @brief Fill the block with a color
 * @param x dest x
 * @param y dest y
 * @param width block width
 * @param height block height
 * @param color color
 */
void display_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint32_t color);

/**
 * @brief Set pixel color
 * @param x pixel x
 * @param y pixel y
 * @param color pixel color
 */
void display_set_pixel(uint8_t x, uint8_t y, uint32_t color);

/**
 * @brief Reverse color
 * @param x pixel x
 * @param y pixel y
 * @param width block width
 * @param height block height
 */
void display_reverse_color(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

/**
 * @brief Draw a string
 * @param x destination x
 * @param y destination y
 * @param str string
 */
void display_draw_string(uint8_t x, uint8_t y, const char* str);

/**
 * @brief Use a font
 * @param fontw font symbol width
 * @param fonth font symbol height
 * @param imgw font atlas width
 * @param imgh font atlas height
 * @param img font atlas image
 */
bool display_usefont(uint8_t fontw, uint8_t fonth, uint16_t imgw, uint16_t imgh, const uint8_t *img);

/**
 * @brief Clear with a color
 * @param color color
 */
status_t display_clear(ch1115_color_t color);

/**
 * @brief Scroll screen
 * @param x rect x
 * @param y rect y
 * @param w rect width
 * @param h rect height
 */
void display_scroll_left(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

/**
 * @brief Get display hardware refresh rate in Hz
 */
uint16_t display_refresh_rate();

#endif // !_SYS_DISPLAY_H
