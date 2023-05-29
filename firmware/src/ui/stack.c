#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <sys/display.h>
#include <sys/scheduler.h>

#include "stack.h"

stack_ctx_t* stack_create() {
  stack_ctx_t *temp = malloc(sizeof(stack_ctx_t)); {
    temp->deepth = 0;
    temp->entry = NULL;
    temp->final = NULL;
  }
  return temp;
}

uint8_t stack_push_window(stack_ctx_t* ctx, const window_def_t *def) {

  // allocate a window
  window_t *temp = malloc(sizeof(window_t)); {
    temp->def = def;
  }

  stack_node_t *node = malloc(sizeof(stack_node_t)); {

    // breath off display
    display_light(false, true);

    // setup new node
    node->next = NULL;
    node->last = ctx->final;
    node->window = temp;

    // call destroy
    if(ctx->final) {
      stack_call_event(ctx, event_window_destroy, 0, 0);
    }

    // setup node
    if(!ctx->entry) ctx->entry = node;
    if(!ctx->final) ctx->final = node; else ctx->final->next = node;

    ++ctx->deepth;
    ctx->final = node;
  }

  // setup window statbar
  if(def->type == wndtype_normal ||
     def->type == wndtype_fullscreen) {
    display_clear(clr_black); {
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
  stack_call_event(ctx, event_window_create, (param_t)temp, NULL);

  // breath on display
  display_light(true, true);

  return ctx->deepth;
}

uint8_t stack_set_window(stack_ctx_t* ctx, window_def_t *window) {

  // back to first
  while(stack_back(ctx));

  // push window to ahead
  stack_push_window(ctx, window);
  return ctx->deepth;
}

uint8_t stack_back(stack_ctx_t* ctx) {

  // check if there is a node
  if(!ctx->final) return 0;

  // breath off display
  display_light(false, true);

  // remove node
  stack_call_event(ctx, event_window_destroy, (param_t)ctx->final->window, NULL); {
    stack_node_t *temp = ctx->final;
    ctx->final = ctx->final->last;

    free(temp);
    free(temp->window);
  }

  // setup window statbar
  if(ctx->final->window->def->type == wndtype_normal ||
     ctx->final->window->def->type == wndtype_fullscreen) {
    display_clear(clr_black); {
      // statbar_set_title(*ctx->final->window->def->title);
      // statbar_set_visible(ctx->final->window->def->type != wndtype_fullscreen, true);
      // statbar_draw();
    }
  }

  // popup window
  else if(ctx->final->window->def->type == wndtype_popup) {
    
  }

  // back to prev window
  stack_call_event(ctx, event_window_create, (param_t)ctx->final->window, NULL);

  // breath on display
  display_light(true, true);

  // decrease deepth
  return --ctx->deepth;
}

uint8_t stack_get_deepth(stack_ctx_t* ctx) {
  return ctx->deepth;
}

int8_t stack_call_event(stack_ctx_t* ctx, event_t event, param_t lp, param_t wp) {
  return ctx->final->window->def->cb_wndproc(ctx, event, lp, wp);
}
