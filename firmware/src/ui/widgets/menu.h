#ifndef _UI_WIDGETS_MENU_H

#include <stddef.h>
#include <stdbool.h>
#include <ui/widgets/base.h>

typedef struct {
  uint32_t id;
  const char** i18nstr;
} menu_item_t;

typedef enum {
  menu_callback_click,
} menu_callback_id_t;

#ifdef _WIDGET_MENU_INTERNAL
typedef struct {
  widget_t base;
  widget_callback_t callback;
  const menu_item_t* items;
  uint8_t items_count;
  uint8_t items_selected;
} menu_widget_t;

#endif /* _WIDGET_MENU_INTERNAL */

/**
 * @brief initialize menu widget
 * @param x
 * @param y
 * @param width
 * @param height
 */
void menu_init(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

/**
 * @brief uninitialize menu widget
 */
void menu_uninit();

/**
 * @brief set menu items
 * @param items
 * @param count
 */
void menu_set_items(const menu_item_t* items, uint8_t count);

/**
 * @brief draw menu
 */
void menu_draw();

/**
 * @brief select menu item
 * @param index
 */
void menu_select(uint8_t index);

bool menu_set_onclick(void* callback);


#endif /* _UI_WIDGETS_MENU_H */
