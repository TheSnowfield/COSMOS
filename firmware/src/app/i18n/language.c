#define I18N_INTERNAL
  #include "language.h"
  #include "en-us/lang.h"
#undef I18N_INTERNAL

bool i18n_init(i18n_language_t lang) {

  static i18n_init_cb list[] = {
    i18n_init_enus,
  };
  
  return list[lang]();
}
