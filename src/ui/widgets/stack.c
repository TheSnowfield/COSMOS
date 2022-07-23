#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

// #include <sys/display.h>
#include <widgets/stack.h>
// #include <widgets/statbar.h>

stack_ctx_t* stack_create() {
  stack_ctx_t *temp = malloc(sizeof(stack_ctx_t)); {
    temp->deepth = 0;
    temp->entry = NULL;
    temp->final = NULL;
  }
  return temp;
}

uint8_t stack_push_surface(stack_ctx_t* ctx, const surface_def_t *def) {

  // Allocate a suface
  surface_t *temp = malloc(sizeof(surface_t)); {
    temp->def = def;
  }

  stack_node_t *node = malloc(sizeof(stack_node_t)); {

    // Setup new node
    node->next = NULL;
    node->last = ctx->final;
    node->surface = temp;

    // Call destroy
    if(ctx->final) {
      ctx->final->surface->def->cb_destroy(ctx->final->surface);
    }

    // Setup node
    if(!ctx->entry) ctx->entry = node;
    if(!ctx->final) ctx->final = node; else ctx->final->next = node;

    ++ctx->deepth;
    ctx->final = node;
  }

  // Setup surface statbar
  // display_clear(0x000000); {
  //   statbar_set_title(def->title);
  //   statbar_set_visible(!def->fullscreen);
  //   statbar_draw();
  // }

  // Call on presend
  def->cb_create(ctx, temp);

  return ctx->deepth;
}

uint8_t stack_set_surface(stack_ctx_t* ctx, surface_def_t *surface) {

  // Back to first
  while(stack_back(ctx));

  // Push surface to ahead
  stack_push_surface(ctx, surface);
  return ctx->deepth;
}

uint8_t stack_back(stack_ctx_t* ctx) {

  // Check if there is a node
  if(!ctx->final) return 0;

  // Remove node
  ctx->final->surface->def->cb_destroy(ctx->final->surface); {
    stack_node_t *temp = ctx->final;
    ctx->final = ctx->final->last;

    free(temp);
    free(temp->surface);
  }

  // Setup surface statbar
  // display_clear(0x000000); {
  //   statbar_set_title(ctx->final->surface->def->title);
  //   statbar_set_visible(!ctx->final->surface->def->fullscreen);
  //   statbar_draw();
  // }

  // Back to prev surface
  ctx->final->surface->def->cb_create(ctx, ctx->final->surface);

  // Decrease deepth
  return --ctx->deepth;
}

uint8_t stack_get_deepth(stack_ctx_t* ctx) {
  return ctx->deepth;
}

// surface_t* stack_create_surface(surface_def_t *def) {


//   return temp;
// }

// bool stack_destroy_surface(surface_t *surface) {

//   if(!surface) return false;

//   // Destroy a surface
//   lv_obj_del(surface->screen);
//   free(surface);

//   return true;
// }
