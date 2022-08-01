#ifndef __APP_LAYOUT_BASE_H
#define __APP_LAYOUT_BASE_H

#define DECLARE_LAYOUT(name, full) \
__weak void surface_##name##_create(stack_ctx_t *stack, surface_t *ctx); \
__weak void surface_##name##_present(surface_t *ctx);                    \
__weak void surface_##name##_destroy(surface_t *ctx);                    \
                                                                \
static const surface_def_t surface_##name = {                   \
  .title = #name,                                               \
  .fullscreen = #full,                                          \
  .cb_create = surface_##name##_create,                         \
  .cb_present = surface_##name##_present,                       \
  .cb_destroy = surface_##name##_destroy                        \
};                                                              \

#endif /* __APP_LAYOUT_BASE_H */
