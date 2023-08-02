#ifndef _APP_RESOURCE_H
#define _APP_RESOURCE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

extern const uint8_t* RUCFSIMG;

#ifdef RES_INTERNAL
  #define RESDEF(x) const uint8_t* RES_##x
#else
  #define RESDEF(x) extern uint8_t* RES_##x
#endif

RESDEF(FONT_PIXEL3X5);
RESDEF(FONT_PIXEL5X7);
RESDEF(FONT_PIXEL7X5);
RESDEF(TEXTURE_COSMOS_SPLASH);
RESDEF(ICON_FOLDER);
RESDEF(ICON_LATCH);

/**
 * @brief resource initialization
 * @param address flash address
 */
bool resource_init(const uint8_t* address);

/**
 * @brief uninit
 */
void resource_uninit();

#endif /* _APP_RESOURCE_H */
