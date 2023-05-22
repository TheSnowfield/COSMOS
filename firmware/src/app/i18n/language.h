#ifndef _I18N_LANGUAGE_H
#define _I18N_LANGUAGE_H

#include <stdbool.h>

typedef enum {
  i18n_language_en_us,
  i18n_language_max
} i18n_language_t;

// i18 string definitions
#ifdef I18N_INTERNAL
  #define I18NSTRDEF(x) const char* STRING_##x
  typedef bool (* i18n_init_cb)();
#else
  #define I18NSTRDEF(x) extern const char* STRING_##x
#endif

I18NSTRDEF(EMPTY);
I18NSTRDEF(FOLDER);

/**
 * @brief initialize i18n string
 * @param lang language type
 */
bool i18n_init(i18n_language_t lang);

#endif /* _I18N_LANGUAGE_H */
