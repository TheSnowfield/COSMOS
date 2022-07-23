#ifndef _LAYOUT_WAVE_H
#define _LAYOUT_WAVE_H

void surface_unlock_create(stack_ctx_t *stack, surface_t *ctx);
void surface_unlock_present(surface_t *ctx);
void surface_unlock_destroy(surface_t *ctx);

static const surface_def_t surface_wave = {
  .title = "Unlock",
  .fullscreen = false,
  .cb_create = surface_unlock_create,
  .cb_present = surface_unlock_present,
  .cb_destroy = surface_unlock_destroy
};

#endif /* _LAYOUT_WAVE_H */
