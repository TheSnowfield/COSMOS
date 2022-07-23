#ifndef _WIDGETS_STACK_H
#define _WIDGETS_STACK_H

#include <stdint.h>
#include <stdbool.h>

typedef struct stack_ctx stack_ctx_t;
typedef struct stack_node stack_node_t;
typedef struct surface surface_t;
typedef struct surface_def surface_def_t;

typedef void (* cb_create)(stack_ctx_t *stack, surface_t *ctx);
typedef void (* cb_present)(surface_t *ctx);
typedef void (* cb_destroy)(surface_t *ctx);

struct stack_node {
  stack_node_t* next;
  stack_node_t* last;
  surface_t* surface;
};

struct stack_ctx {
  uint8_t deepth;
  stack_node_t *entry;
  stack_node_t *final;
};

struct surface {
  void *userdata;
  const surface_def_t *def;
};

struct surface_def {
  char* title;
  bool  fullscreen;
  cb_create cb_create;
  cb_present cb_present;
  cb_destroy cb_destroy;
};

stack_ctx_t* stack_create();

uint8_t stack_push_surface(stack_ctx_t* ctx, const surface_def_t *surface);

uint8_t stack_set_surface(stack_ctx_t* ctx, surface_def_t *surface);

uint8_t stack_back(stack_ctx_t* ctx);

uint8_t stack_get_deepth(stack_ctx_t* ctx);


#endif /* _WIDGETS_STACK_H */
