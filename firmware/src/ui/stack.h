#ifndef _WIDGETS_STACK_H
#define _WIDGETS_STACK_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/event.h>

typedef struct stack_ctx stack_ctx_t;
typedef struct stack_node stack_node_t;
typedef struct surface surface_t;
typedef struct surface_def surface_def_t;

typedef uint32_t (* cb_wndproc)(stack_ctx_t *ctx, event_t event, param_t lp, param_t wp);

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

typedef enum {
  wndtype_normal     = 0,
  wndtype_fullscreen = 1,
  wndtype_popup      = 2
} surface_type_t;

struct surface_def {
  const char** title;
  surface_type_t type;
  cb_wndproc cb_wndproc;
};

#define present_counter(x) ++(*((uint32_t *)(&(((surface_t *)x)->userdata))))
#define present_frame_scale(x, t) ((*((uint32_t *)(&(((surface_t *)x)->userdata)))) % t == 0)

/**
 * @brief create a surface stack
 * @return if success return a stack pointer
 */
stack_ctx_t* stack_create();

/**
 * @brief push surface to the front
 * @param ctx stack context
 * @param surface surface def
 * @return return the stack deepth
 */
uint8_t stack_push_surface(stack_ctx_t* ctx, const surface_def_t *surface);

/**
 * @brief replace the front surface
 * @param ctx stack context
 * @param surface surface def
 * @return the stack deepth
 */
uint8_t stack_set_surface(stack_ctx_t* ctx, surface_def_t *surface);

/**
 * @brief stack back
 * @param ctx stack context
 * @return the stack deepth
 */
uint8_t stack_back(stack_ctx_t* ctx);

/**
 * @brief get the deepth of the stack
 * @param ctx stack context
 * @return the stack deepth
 */
uint8_t stack_get_deepth(stack_ctx_t* ctx);

/**
 * @brief call event to the surface
 * @param ctx stack context
 * @param event event. @ref event_t
 * @param lp lparam
 * @param wp wparam
 * @return result
 */
int8_t stack_call_event(stack_ctx_t* ctx, event_t event, param_t lp, param_t wp);

#endif /* _WIDGETS_STACK_H */
