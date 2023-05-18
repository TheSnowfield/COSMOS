#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <sys/display.h>

#include "stack.h"

stack_ctx_t* stack_create() {
  stack_ctx_t *temp = malloc(sizeof(stack_ctx_t)); {
    temp->deepth = 0;
    temp->entry = NULL;
    temp->final = NULL;
  }
  return temp;
}

uint8_t stack_push_surface(stack_ctx_t* ctx, const surface_def_t *def) {

  // allocate a suface
  surface_t *temp = malloc(sizeof(surface_t)); {
    temp->def = def;
  }

  stack_node_t *node = malloc(sizeof(stack_node_t)); {

    // setup new node
    node->next = NULL;
    node->last = ctx->final;
    node->surface = temp;

    // call destroy
    if(ctx->final) {
      stack_call_event(ctx, event_surface_destroy, 0, 0);
    }

    // setup node
    if(!ctx->entry) ctx->entry = node;
    if(!ctx->final) ctx->final = node; else ctx->final->next = node;

    ++ctx->deepth;
    ctx->final = node;
  }

  // setup surface statbar
  if(def->type == wndtype_normal ||
     def->type == wndtype_fullscreen) {
    display_clear(0x000000); {
      // statbar_set_title(*def->title);
      // statbar_set_visible(def->type != wndtype_fullscreen, true);
      // statbar_draw();
    }
  }

  // popup window
  else if(def->type == wndtype_popup) {
    // statbar_set_title(*def->title);
    // statbar_set_visible(false, false);
  }

  // call on create
  stack_call_event(ctx, event_surface_create, (param_t)temp, NULL);
  return ctx->deepth;
}

uint8_t stack_set_surface(stack_ctx_t* ctx, surface_def_t *surface) {

  // back to first
  while(stack_back(ctx));

  // push surface to ahead
  stack_push_surface(ctx, surface);
  return ctx->deepth;
}

uint8_t stack_back(stack_ctx_t* ctx) {

  // check if there is a node
  if(!ctx->final) return 0;

  // remove node
  stack_call_event(ctx, event_surface_destroy, (param_t)ctx->final->surface, NULL); {
    stack_node_t *temp = ctx->final;
    ctx->final = ctx->final->last;

    free(temp);
    free(temp->surface);
  }

  // setup surface statbar
  if(ctx->final->surface->def->type == wndtype_normal ||
     ctx->final->surface->def->type == wndtype_fullscreen) {
    display_clear(0x000000); {
      // statbar_set_title(*ctx->final->surface->def->title);
      // statbar_set_visible(ctx->final->surface->def->type != wndtype_fullscreen, true);
      // statbar_draw();
    }
  }

  // popup window
  else if(ctx->final->surface->def->type == wndtype_popup) {
    
  }

  // back to prev surface
  stack_call_event(ctx, event_surface_create, (param_t)ctx->final->surface, NULL);

  // decrease deepth
  return --ctx->deepth;
}

uint8_t stack_get_deepth(stack_ctx_t* ctx) {
  return ctx->deepth;
}

int8_t stack_call_event(stack_ctx_t* ctx, event_t event, param_t lp, param_t wp) {
  return ctx->final->surface->def->cb_wndproc(ctx, event, lp, wp);
}
