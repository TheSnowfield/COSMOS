#ifndef _UI_LAYOUT_H
#define _UI_LAYOUT_H

#include <ui/stack.h>
#include <sys/event.h>
#include <i18n/language.h>

#define DECLARE_LAYOUT(name, titl, wndtype) \
uint32_t surface_##name##_wndproc(stack_ctx_t *ctx, event_t event, param_t lp, param_t wp);   \
                                                          \
static const surface_def_t surface_##name = {             \
  .title = titl,                                         \
  .type = wndtype,                                        \
  .cb_wndproc = surface_##name##_wndproc                  \
};                                                        \

#endif /* _UI_LAYOUT_H */
